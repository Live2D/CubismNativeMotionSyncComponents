/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppAudioManager.hpp"
#include "LAppWavFileHandler.hpp"
#include "JniBridgeC.hpp"
#include "LAppPal.hpp"

using namespace Csm;

csmBool LAppAudioManager::LoadFile(csmString path, csmUint32 useChannel)
{
    // 初期化
    Release();
    _isLoadFile = true;

    // WAVファイルをロード
    LAppWavFileHandler wavHandler;
    wavHandler.Start(path);
    LAppWavFileHandler::WavFileInfo wavHandlerInfo = wavHandler.GetWavFileInfo();
    csmInt32 channels = wavHandlerInfo._numberOfChannels;

    // リングバッファ確保
    _buffer.Resize(LAppMotionSyncDefine::CsmRingBufferSize * wavHandlerInfo._blockAlign);

    // 生の音声データを取得
    _dataSize = wavHandler.GetRawDataSize();
    _data = reinterpret_cast<csmByte*>(CSM_MALLOC(sizeof(csmByte) * _dataSize));
    if (!_data)
    {
        LAppPal::PrintLogLn("[APP]Failed malloc to '_data' in LAppAudioManager::LoadFile()");
        return false;
    }
    for (csmUint32 i = 0; i < _dataSize; i++)
    {
        _data[i] = wavHandler.GetRawData()[i];
    }

    // 口形に影響を与えたい音声のチャンネルを保存
    if (channels <= useChannel)
    {
        useChannel = channels - 1;
    }

    // 正規化されている音声データ
    _samplesPos = 0;
    _samplesSize = wavHandlerInfo._samplesPerChannel;
    _samples = reinterpret_cast<csmFloat32*>(CSM_MALLOC(sizeof(csmFloat32) * _samplesSize));
    if (!_samples)
    {
        LAppPal::PrintLogLn("[APP]Failed malloc to '_samples' in LAppAudioManager::LoadFile()");
        return false;
    }
    wavHandler.GetPcmDataChannel(_samples, useChannel);

    // 音声バッファ作成
    _audioOutputId = JniBridgeC::CreateAudioOutputBuffer(wavHandlerInfo._samplingRate, wavHandlerInfo._numberOfChannels, wavHandlerInfo._bitsPerSample);

    // データを書き込み
    JniBridgeC::WriteAudioOutputBuffer(_audioOutputId, 0, _data, _dataSize);

    return true;
}

csmBool LAppAudioManager::SetupMicrophone(csmUint32 channels, csmUint32 samplesRate, Csm::csmUint32 bitDepth, csmUint32 useChannel)
{
    // 初期化
    Release();
    _isLoadFile = false;

    _samplesPos = 0;
    _samplesSize -1;

    _channels = channels;
    _bitDepth = bitDepth;
    csmUint32 blockAlign = _channels * (_bitDepth / 8);

    // リングバッファ確保
    _buffer.Resize(LAppMotionSyncDefine::CsmRingBufferSize * blockAlign);

    // 口形に影響を与えたい音声のチャンネルを保存
    if (useChannel < _channels)
    {
        _useChannel = useChannel;
    }
    else
    {
        _useChannel = _channels - 1;
    }

    // 録音用のバッファ作成
    _audioInputId = JniBridgeC::CreateAudioInputBuffer(samplesRate, _channels, _bitDepth);

    // 再生用のバッファ作成
    _audioOutputId = JniBridgeC::CreateAudioOutputBuffer(samplesRate, _channels, _bitDepth);

    return true;
}

csmBool LAppAudioManager::Update()
{
    if (_isLoadFile)
    {
        return UpdateForAudioFile();
    }
    else
    {
        return UpdateForMicrophone();
    }

    return true;
}

MotionSync::CubismMotionSyncAudioBuffer<csmFloat32>* LAppAudioManager::GetBuffer()
{
    return &_buffer;
}

csmBool LAppAudioManager::IsPlay()
{
    if (!_isLoadFile)
    {
        return true;
    }

    return _samplesSize > _samplesPos;
}

void LAppAudioManager::Release()
{
    if (_data)
    {
        CSM_FREE(_data);
        _data = NULL;
    }
    if (_samples)
    {
        CSM_FREE(_samples);
        _samples = NULL;
    }
    JniBridgeC::DeleteAudioInputBuffer(_audioInputId);
    JniBridgeC::DeleteAudioOutputBuffer(_audioOutputId);
}

LAppAudioManager::LAppAudioManager() :
    _audioInputId(0),
    _audioOutputId(0),
    _data(NULL),
    _dataSize(0),
    _samples(NULL),
    _samplesSize(0),
    _samplesPos(0),
    _buffer(),
    _isLoadFile(false),
    _channels(1),
    _bitDepth(8),
    _useChannel(0)
{
}

LAppAudioManager::~LAppAudioManager()
{
    Release();
}

csmBool LAppAudioManager::UpdateForAudioFile()
{
    csmInt32 playPosition = JniBridgeC::GetAudioOutputBufferPosition(_audioOutputId);
    if (playPosition < 0)
    {
        LAppPal::PrintLogLn("[APP]Failed to GetAudioOutputBufferPosition() in LAppAudioManager::UpdateForAudioFile()");
        return false;
    }

    // 音声データを口形バッファに書き込み
    for (csmUint32 i = _samplesPos; i < playPosition; i++)
    {
        if (i < _samplesSize)
        {
            // MotionSyncへサンプルを送る。
            _buffer.AddValue(_samples[i]);
            _samplesPos++;
        }
        else
        {
            // 再生終了
            JniBridgeC::StopAudioOutputBuffer(_audioOutputId);
            break;
        }
    }

    return true;
}

csmBool LAppAudioManager::UpdateForMicrophone()
{
    csmUint32 dataSize;
    csmByte* data;

    // 入力データ取得
    data = JniBridgeC::ReadAudioInputBuffer(_audioInputId, &dataSize);

    // 録音データがなければ処理しない
    if (!dataSize)
    {
        delete[] data;
        return false;
    }

    // データを書き込み
    JniBridgeC::WriteAudioOutputBuffer(_audioOutputId, 0, data, dataSize);

    // リングバッファに正規化した値を格納
    csmUint32 byte = _bitDepth / 8;
    for (csmUint32 i = 0; i < dataSize; i += byte)
    {
        // 指定したチャンネルのみ格納
        if ((i / byte) % _channels == _useChannel)
        {
            _buffer.AddValue(LAppWavFileHandler::NormalizePcmSample(_bitDepth, &data[i], dataSize - i));
        }
    }

    // 入力データ解放
    delete[] data;

    return true;
}
