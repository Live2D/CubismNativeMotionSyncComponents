/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppAudioManager.hpp"
#include "LAppWavFileHandler.hpp"
#include "../../main/cpp/JniBridgeC.hpp"
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
    _audioId = JniBridgeC::CreateAudioOutputBuffer(wavHandlerInfo._samplingRate, wavHandlerInfo._numberOfChannels, wavHandlerInfo._bitsPerSample);

    // データを書き込み
    JniBridgeC::WriteAudioOutputBuffer(_audioId, 0, _data, _dataSize);

    return true;
}

csmBool LAppAudioManager::Update()
{
    if (!_isLoadFile)
    {
        // 早送りボタンが押されていないため音声の再生がない
        return true;
    }

    csmInt32 playPosition = JniBridgeC::GetAudioOutputBufferPosition(_audioId);
    if (playPosition < 0)
    {
        LAppPal::PrintLogLn("[APP]Failed to GetAudioOutputBufferPosition() in LAppAudioManager::Update()");
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
            JniBridgeC::StopAudioOutputBuffer(_audioId);
            break;
        }
    }

    return true;
}

MotionSync::CubismMotionSyncAudioBuffer<csmFloat32>* LAppAudioManager::GetBuffer()
{
    return &_buffer;
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
    JniBridgeC::DeleteAudioOutputBuffer(_audioId);
}

LAppAudioManager::LAppAudioManager() :
    _audioId(0),
    _data(NULL),
    _dataSize(0),
    _samples(NULL),
    _samplesSize(0),
    _samplesPos(0),
    _buffer(),
    _isLoadFile(false)
{
}

LAppAudioManager::~LAppAudioManager()
{
    Release();
}
