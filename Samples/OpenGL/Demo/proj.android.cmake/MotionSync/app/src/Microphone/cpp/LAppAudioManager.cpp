/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppAudioManager.hpp"
#include "LAppWavFileHandler.hpp"
#include "../../main/cpp/JniBridgeC.hpp"

using namespace Csm;

csmBool LAppAudioManager::SetupMicrophone(csmUint32 channels, csmUint32 samplesRate, Csm::csmUint32 bitDepth, csmUint32 useChannel)
{
    // 初期化
    Release();

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

MotionSync::CubismMotionSyncAudioBuffer<csmFloat32>* LAppAudioManager::GetBuffer()
{
    return &_buffer;
}

void LAppAudioManager::Release()
{
    JniBridgeC::DeleteAudioInputBuffer(_audioInputId);
    JniBridgeC::DeleteAudioOutputBuffer(_audioOutputId);
}

LAppAudioManager::LAppAudioManager() :
    _audioInputId(0),
    _audioOutputId(0),
    _channels(1),
    _bitDepth(8),
    _useChannel(0),
    _buffer()
{
}

LAppAudioManager::~LAppAudioManager()
{
    Release();
}
