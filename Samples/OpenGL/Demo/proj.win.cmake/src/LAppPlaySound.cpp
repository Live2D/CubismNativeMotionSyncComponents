﻿/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppPlaySound.hpp"
#include <dsound.h>
#include "LAppWavFileHandler.hpp"

#pragma comment(lib, "dsound.lib")

using namespace Csm;

namespace
{
LPDIRECTSOUND8 _directSound = NULL;
LPDIRECTSOUNDBUFFER _primary = NULL;
}

csmBool LAppPlaySound::Init(HWND window, csmInt32 channels, csmInt32 samplesPerSec, csmInt32 bitDepth)
{
    HRESULT result;

    // COMの初期化
    result = CoInitialize(NULL);
    if (FAILED(result))
    {
        CubismLogError("[APP]Failed to CoInitialize() in LAppPlaySound::%s()", __func__);
        return false;
    }

    // DirectSound8を作成
    result = DirectSoundCreate8(NULL, &_directSound, NULL);
    if (FAILED(result))
    {
        CubismLogError("[APP]Failed to DirectSoundCreate8() in LAppPlaySound::%s()", __func__);
        return false;
    }

    // 強調モード
    result = _directSound->SetCooperativeLevel(window, DSSCL_EXCLUSIVE | DSSCL_PRIORITY);
    if (FAILED(result))
    {
        CubismLogError("[APP]Failed to SetCooperativeLevel() in LAppPlaySound::%s()", __func__);
        return false;
    }

    // プライマリサウンドバッファの作成
    WAVEFORMATEX waveFormat;
    DSBUFFERDESC dsdesc;
    ZeroMemory(&dsdesc, sizeof(DSBUFFERDESC));
    dsdesc.dwSize = sizeof(DSBUFFERDESC);
    dsdesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
    dsdesc.dwBufferBytes = 0;
    dsdesc.lpwfxFormat = NULL;
    result = _directSound->CreateSoundBuffer(&dsdesc, &_primary, NULL);
    if (FAILED(result))
    {
        CubismLogError("[APP]Failed to CreateSoundBuffer() in LAppPlaySound::%s()", __func__);
        return false;
    }

    // プライマリバッファのステータスを決定
    ZeroMemory(&waveFormat, sizeof(WAVEFORMATEX));
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = channels;
    waveFormat.nSamplesPerSec = samplesPerSec;
    waveFormat.wBitsPerSample = bitDepth;
    waveFormat.nBlockAlign = waveFormat.nChannels * waveFormat.wBitsPerSample / 8;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    result = _primary->SetFormat(&waveFormat);
    if (FAILED(result))
    {
        CubismLogError("[APP]Failed to SetFormat() in LAppPlaySound::%s()", __func__);
        return false;
    }

    return true;
}

csmBool LAppPlaySound::Close()
{
    if (_primary)
    {
        _primary->Release();
        _primary = NULL;
    }

    if (_directSound)
    {
        _directSound->Release();
        _directSound = NULL;
    }

    // COMの終了
    CoUninitialize();

    return true;
}

csmBool LAppPlaySound::LoadFile(csmString path, csmUint32 useChannel)
{
    // 初期化
    Release();

    HRESULT result;
    LPWAVEFORMATEX waveFormat;
    LAppWavFileHandler wavHandler;

    // WAVファイルをロード
    wavHandler.Start(path);
    LAppWavFileHandler::WavFileInfo wavHandlerInfo = wavHandler.GetWavFileInfo();

    // ヘッダメモリ確保と情報設定
    waveFormat = reinterpret_cast<LPWAVEFORMATEX>(CSM_MALLOC(sizeof(WAVEFORMATEX)));
    if (!waveFormat)
    {
        CubismLogError("[APP]Failed malloc to 'waveFormat' in LAppPlaySound::%s()", __func__);
        return false;
    }
    waveFormat->wFormatTag = WAVE_FORMAT_PCM;
    waveFormat->nChannels = wavHandlerInfo._numberOfChannels;
    _channels = wavHandlerInfo._numberOfChannels;
    waveFormat->nSamplesPerSec = wavHandlerInfo._samplingRate;
    waveFormat->nAvgBytesPerSec = wavHandlerInfo._avgBytesPerSec;
    _bufferSampleBytes = LAppMotionSyncDefine::DirectSoundBufferSampleCount * wavHandlerInfo._blockAlign;
    waveFormat->nBlockAlign = wavHandlerInfo._blockAlign;
    waveFormat->wBitsPerSample = wavHandlerInfo._bitsPerSample;
    _bitDepth = wavHandlerInfo._bitsPerSample;
    waveFormat->cbSize = 0;

    // 生の音声データを取得
    _dataSize = wavHandler.GetRawDataSize();
    _data = reinterpret_cast<csmByte*>(CSM_MALLOC(sizeof(csmByte) * _dataSize));
    if (!_data)
    {
        CubismLogError("[APP]Failed malloc to '_data' in LAppPlaySound::%s()", __func__);
        CSM_FREE(waveFormat);
        return false;
    }
    for (csmUint32 i = 0; i < _dataSize; i++)
    {
        _data[i] = wavHandler.GetRawData()[i];
    }

    // サウンドバッファの作成
    DSBUFFERDESC dsdesc;
    ZeroMemory(&dsdesc, sizeof(DSBUFFERDESC));
    dsdesc.dwSize = sizeof(DSBUFFERDESC);
    dsdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER;
    dsdesc.dwBufferBytes = _bufferSampleBytes;
    dsdesc.lpwfxFormat = waveFormat;
    dsdesc.guid3DAlgorithm = DS3DALG_DEFAULT;
    result = _directSound->CreateSoundBuffer(&dsdesc, &_secondary, NULL);
    if (FAILED(result))
    {
        CubismLogError("[APP]Failed to CreateSoundBuffer() in LAppPlaySound::%s()", __func__);
        CSM_FREE(waveFormat);
        return false;
    }

    // 初期データを前半だけ書き込み
    _writePosition = WritePosition_Front;
    _dataPos = 0;
    LPVOID soundBuffer1, soundBuffer2;
    DWORD soundBufferSize1, soundBufferSize2;
    csmUint32 bufferBytes = _bufferSampleBytes / 2;
    result = _secondary->Lock(0, bufferBytes, &soundBuffer1, &soundBufferSize1, &soundBuffer2, &soundBufferSize2, DSBLOCK_ENTIREBUFFER);
    if (FAILED(result))
    {
        CubismLogError("[APP]Failed to Lock() in LAppPlaySound::%s()", __func__);
        return false;
    }
    for (DWORD i = 0; i < soundBufferSize1 && _dataPos < _dataSize; i++)
    {
        reinterpret_cast<csmByte*>(soundBuffer1)[i] = _data[_dataPos];
        _dataPos++;
    }
    _secondary->Unlock(soundBuffer1, soundBufferSize1, soundBuffer2, soundBufferSize2);

    // 口形に影響を与えたい音声のチャンネルを保存
    if (_channels <= useChannel)
    {
        useChannel = _channels - 1;
    }
    _samplesSize = wavHandlerInfo._samplesPerChannel;
    _samples = reinterpret_cast<csmFloat32*>(CSM_MALLOC(sizeof(csmFloat32) * _samplesSize));
    if (!_samples)
    {
        CubismLogError("[APP]Failed malloc to '_samples' in LAppPlaySound::%s()", __func__);
        CSM_FREE(waveFormat);
        return false;
    }
    wavHandler.GetPcmDataChannel(_samples, useChannel);

    // ヘッダ用メモリを開放
    CSM_FREE(waveFormat);

    _secondary->Play(0, 0, DSBPLAY_LOOPING);

    return true;
}

csmBool LAppPlaySound::Update()
{
    HRESULT result;
    DWORD playCursor;
    LPVOID soundBuffer1, soundBuffer2;
    DWORD soundBufferSize1, soundBufferSize2;
    csmUint32 bufferBytes = _bufferSampleBytes / 2;

    result = _secondary->GetCurrentPosition(&playCursor, NULL);
    if (FAILED(result))
    {
        CubismLogError("[APP]Failed to GetCurrentPosition() in LAppPlaySound::%s()", __func__);
        return false;
    }

    // 音声データをオーディオ側に書き込み
    if (_writePosition == WritePosition_Back && bufferBytes <= playCursor)
    {
        // 前半書き込み
        _writePosition = WritePosition_Front;
        result = _secondary->Lock(0, bufferBytes, &soundBuffer1, &soundBufferSize1, &soundBuffer2, &soundBufferSize2, 0);
        if (FAILED(result))
        {
            CubismLogError("[APP]Failed to Lock() in LAppPlaySound::%s()", __func__);
            return false;
        }
        for (DWORD i = 0; i < soundBufferSize1 && _dataPos < _dataSize; i++)
        {
            reinterpret_cast<csmByte*>(soundBuffer1)[i] = _data[_dataPos];
            _dataPos++;
        }
        _secondary->Unlock(soundBuffer1, soundBufferSize1, soundBuffer2, soundBufferSize2);

        // 生の音声データがなくなったら再生終了
        if (_dataSize <= _dataPos)
        {
            _secondary->Stop();
        }
    }
    else if (_writePosition == WritePosition_Front && playCursor < bufferBytes)
    {
	    // 後半書き込み
        _writePosition = WritePosition_Back;
        result = _secondary->Lock(bufferBytes, bufferBytes * 2, &soundBuffer1, &soundBufferSize1, &soundBuffer2, &soundBufferSize2, 0);
        if (FAILED(result))
        {
            CubismLogError("[APP]Failed to Lock() in LAppPlaySound::%s()", __func__);
            return false;
        }
        for (DWORD i = 0; i < soundBufferSize1 && _dataPos < _dataSize; i++)
        {
            reinterpret_cast<csmByte*>(soundBuffer1)[i] = _data[_dataPos];
            _dataPos++;
        }
        _secondary->Unlock(soundBuffer1, soundBufferSize1, soundBuffer2, soundBufferSize2);

        // 生の音声データがなくなったら再生終了
        if (_dataSize <= _dataPos)
        {
            _secondary->Stop();
        }
    }

    // 音声データを口形バッファに書き込み
    csmUint32 pos = _dataPos / _channels / (_bitDepth / 8);
    for (csmUint32 i = _buffer.GetSize(); i < pos; i++)
    {
        if (i < _samplesSize)
        {
             // MotionSyncへサンプルを送る。
             _buffer.PushBack(_samples[i]);
        }
        else
        {
            // 再生終了
            _secondary->Stop();
            break;
        }
   }

    return true;
}

csmVector<csmFloat32>* LAppPlaySound::GetBuffer()
{
    return &_buffer;
}

csmBool LAppPlaySound::IsPlay()
{
    DWORD status;
    _secondary->GetStatus(&status);
    return status & DSBSTATUS_PLAYING;
}

void LAppPlaySound::Release()
{
    if (_secondary)
    {
        _secondary->Stop();
        _secondary->Release();
        _secondary = NULL;
    }
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
    _buffer.Clear();
}

LAppPlaySound::LAppPlaySound() :
    _secondary(NULL),
    _channels(1),
    _bitDepth(8),
    _data(NULL),
    _dataSize(0),
    _dataPos(0),
    _samples(NULL),
    _samplesSize(0)
{
}

LAppPlaySound::~LAppPlaySound()
{
    Release();
}
