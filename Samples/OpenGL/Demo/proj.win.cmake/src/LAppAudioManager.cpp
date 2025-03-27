/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppAudioManager.hpp"
#include <dsound.h>
#include "LAppWavFileHandler.hpp"
#include "LAppPal.hpp"

#pragma comment(lib, "dsound.lib")

using namespace Csm;

namespace
{
LPDIRECTSOUND8 _directSound = NULL;
LPDIRECTSOUNDBUFFER _primary = NULL;
}

csmBool LAppAudioManager::Init(HWND window, csmInt32 channels, csmInt32 samplesPerSec, csmInt32 bitDepth)
{
    HRESULT result;

    // COMの初期化
    result = CoInitialize(NULL);
    if (FAILED(result))
    {
        LAppPal::PrintLogLn("[APP]Failed to CoInitialize() in LAppAudioManager::Init()");
        return false;
    }

    // DirectSound8を作成
    result = DirectSoundCreate8(NULL, &_directSound, NULL);
    if (FAILED(result))
    {
        LAppPal::PrintLogLn("[APP]Failed to DirectSoundCreate8() in LAppAudioManager::Init()");
        return false;
    }

    // 強調モード
    result = _directSound->SetCooperativeLevel(window, DSSCL_EXCLUSIVE | DSSCL_PRIORITY);
    if (FAILED(result))
    {
        LAppPal::PrintLogLn("[APP]Failed to SetCooperativeLevel() in LAppAudioManager::Init()");
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
        LAppPal::PrintLogLn("[APP]Failed to CreateSoundBuffer() in LAppAudioManager::Init()");
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
        LAppPal::PrintLogLn("[APP]Failed to SetFormat() in LAppAudioManager::Init()");
        return false;
    }

    return true;
}

csmBool LAppAudioManager::Close()
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

csmBool LAppAudioManager::SetupMicrophone(csmUint32 channels, csmUint32 samplesRate, Csm::csmUint32 bitDepth, csmUint32 useChannel)
{
    Release();
    _isLoadFile = false;

    HRESULT result;

    _channels = channels;
    _bitDepth = bitDepth;

    // 口形に影響を与えたい音声のチャンネルを保存
    if (useChannel < _channels)
    {
        _useChannel = useChannel;
    }
    else
    {
        _useChannel = _channels - 1;
    }

    // 録音用のバッファの作成
    result = DirectSoundCaptureCreate8(NULL, &_soundCapture, NULL);
    if (FAILED(result))
    {
        LAppPal::PrintLogLn("[APP]Failed to DirectSoundCaptureCreate8() in LAppAudioManager::SetupMicrophone()");
        return false;
    }

    // 録音用のフォーマットの設定
    WAVEFORMATEX inputWaveFormat;
    ZeroMemory(&inputWaveFormat, sizeof(WAVEFORMATEX));
    inputWaveFormat.wFormatTag = WAVE_FORMAT_PCM;
    inputWaveFormat.nChannels = _channels;
    inputWaveFormat.nSamplesPerSec = samplesRate;
    inputWaveFormat.wBitsPerSample = _bitDepth;
    inputWaveFormat.nBlockAlign = (_channels * _bitDepth) / 8;
    inputWaveFormat.nAvgBytesPerSec = samplesRate * inputWaveFormat.nBlockAlign;

    // 確保するバッファサイズ
    _bufferSampleBytes = LAppMotionSyncDefine::DirectSoundBufferSoundFileSampleCount * inputWaveFormat.nBlockAlign;

    // リングバッファ確保
    _buffer.Resize(LAppMotionSyncDefine::CsmRingBufferSize * inputWaveFormat.nBlockAlign);

    // DirectSoundCaptureBufferオブジェクトの作成
    DSCBUFFERDESC inputBufferSetting;
    ZeroMemory(&inputBufferSetting, sizeof(DSCBUFFERDESC));
    inputBufferSetting.dwSize = sizeof(DSCBUFFERDESC);
    inputBufferSetting.dwBufferBytes = _bufferSampleBytes;
    inputBufferSetting.lpwfxFormat = &inputWaveFormat;

    result = _soundCapture->CreateCaptureBuffer(&inputBufferSetting, &_captureBuffer, NULL);
    if (FAILED(result))
    {
        LAppPal::PrintLogLn("[APP]Failed to CreateCaptureBuffer() in LAppAudioManager::SetupMicrophone()");
        return false;
    }

    // 再生用のフォーマットの設定
    LPWAVEFORMATEX outputWaveFormat;
    outputWaveFormat = reinterpret_cast<LPWAVEFORMATEX>(CSM_MALLOC(sizeof(WAVEFORMATEX)));
    if (!outputWaveFormat)
    {
        LAppPal::PrintLogLn("[APP]Failed malloc to 'waveFormat' in LAppAudioManager::SetupMicrophone()");
        return false;
    }
    outputWaveFormat->wFormatTag = WAVE_FORMAT_PCM;
    outputWaveFormat->nChannels = _channels;
    outputWaveFormat->nSamplesPerSec = samplesRate;
    outputWaveFormat->nAvgBytesPerSec = samplesRate * inputWaveFormat.nBlockAlign;
    outputWaveFormat->nBlockAlign = inputWaveFormat.nBlockAlign;
    outputWaveFormat->wBitsPerSample = _bitDepth;
    outputWaveFormat->cbSize = 0;

    // サウンドバッファの作成
    DSBUFFERDESC outputBufferSetting;
    ZeroMemory(&outputBufferSetting, sizeof(DSBUFFERDESC));
    outputBufferSetting.dwSize = sizeof(DSBUFFERDESC);
    outputBufferSetting.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER;
    outputBufferSetting.dwBufferBytes = _bufferSampleBytes;
    outputBufferSetting.lpwfxFormat = outputWaveFormat;
    outputBufferSetting.guid3DAlgorithm = DS3DALG_DEFAULT;
    result = _directSound->CreateSoundBuffer(&outputBufferSetting, &_secondary, NULL);
    if (FAILED(result))
    {
        LAppPal::PrintLogLn("[APP]Failed to CreateSoundBuffer() in LAppAudioManager::SetupMicrophone()");
        CSM_FREE(outputWaveFormat);
        return false;
    }

    // ヘッダ用メモリを開放
    CSM_FREE(outputWaveFormat);

    // 録音開始
    _captureBuffer->Start(DSCBSTART_LOOPING);

    // 音声再生
    _secondary->Play(0, 0, DSBPLAY_LOOPING);

    // 録音時の読み込み位置
    _inputPos = 0;

    // 再生時の書き込み位置設定
    _outputPos = 0;
    result = _secondary->SetCurrentPosition(_outputPos);
    if (FAILED(result))
    {
        LAppPal::PrintLogLn("[APP]Failed to SetCurrentPosition() in LAppAudioManager::SetupMicrophone()");
    }

    return true;
}

csmBool LAppAudioManager::LoadFile(csmString path, csmUint32 useChannel)
{
    // 初期化
    Release();
    _isLoadFile = true;

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
        LAppPal::PrintLogLn("[APP]Failed malloc to 'waveFormat' in LAppAudioManager::LoadFile()");
        return false;
    }
    waveFormat->wFormatTag = WAVE_FORMAT_PCM;
    waveFormat->nChannels = wavHandlerInfo._numberOfChannels;
    _channels = wavHandlerInfo._numberOfChannels;
    waveFormat->nSamplesPerSec = wavHandlerInfo._samplingRate;
    waveFormat->nAvgBytesPerSec = wavHandlerInfo._avgBytesPerSec;
    _bufferSampleBytes = LAppMotionSyncDefine::DirectSoundBufferSoundFileSampleCount * wavHandlerInfo._blockAlign;
    waveFormat->nBlockAlign = wavHandlerInfo._blockAlign;
    waveFormat->wBitsPerSample = wavHandlerInfo._bitsPerSample;
    _bitDepth = wavHandlerInfo._bitsPerSample;
    waveFormat->cbSize = 0;

    // リングバッファ確保
    _buffer.Resize(LAppMotionSyncDefine::CsmRingBufferSize * wavHandlerInfo._blockAlign);

    // 生の音声データを取得
    _dataSize = wavHandler.GetRawDataSize();
    _data = reinterpret_cast<csmByte*>(CSM_MALLOC(sizeof(csmByte) * _dataSize));
    if (!_data)
    {
        LAppPal::PrintLogLn("[APP]Failed malloc to '_data' in LAppAudioManager::LoadFile()");
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
        LAppPal::PrintLogLn("[APP]Failed to CreateSoundBuffer() in LAppAudioManager::LoadFile()");
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
        LAppPal::PrintLogLn("[APP]Failed to Lock() in LAppAudioManager::LoadFile()");
        return false;
    }
    for (DWORD i = 0; i < soundBufferSize1 && _dataPos < _dataSize; i++)
    {
        reinterpret_cast<csmByte*>(soundBuffer1)[i] = _data[_dataPos];
        _dataPos++;
    }
    for (DWORD i = 0; i < soundBufferSize2 && _dataPos < _dataSize; i++)
    {
        reinterpret_cast<csmByte*>(soundBuffer2)[i] = _data[_dataPos];
        _dataPos++;
    }
    result = _secondary->Unlock(soundBuffer1, soundBufferSize1, soundBuffer2, soundBufferSize2);
    if (FAILED(result))
    {
        LAppPal::PrintLogLn("[APP]Failed to Unlock() in LAppAudioManager::Update()");
        return false;
    }

    // 口形に影響を与えたい音声のチャンネルを保存
    if (_channels <= useChannel)
    {
        useChannel = _channels - 1;
    }
    _samplesPos = 0;
    _samplesSize = wavHandlerInfo._samplesPerChannel;
    _samples = reinterpret_cast<csmFloat32*>(CSM_MALLOC(sizeof(csmFloat32) * _samplesSize));
    if (!_samples)
    {
        LAppPal::PrintLogLn("[APP]Failed malloc to '_samples' in LAppAudioManager::LoadFile()");
        CSM_FREE(waveFormat);
        return false;
    }
    wavHandler.GetPcmDataChannel(_samples, useChannel);

    // ヘッダ用メモリを開放
    CSM_FREE(waveFormat);

    _secondary->Play(0, 0, DSBPLAY_LOOPING);

    result = _secondary->SetCurrentPosition(0);
    if (FAILED(result))
    {
        LAppPal::PrintLogLn("[APP]Failed to SetCurrentPosition() in LAppAudioManager::LoadFile()");
    }

    return true;
}

csmBool LAppAudioManager::UpdateForMicrophone()
{
    HRESULT result;

    DWORD inputReadPos;
    LONG inputSize;
    LPVOID inputSoundBuffer1, inputSoundBuffer2;
    DWORD inputSoundBufferSize1, inputSoundBufferSize2;

    if (!_captureBuffer)
    {
        LAppPal::PrintLogLn("[APP]_captureBuffer is null in LAppAudioManager::UpdateForMicrophone()");
        return false;
    }

    // 録音の書き込み位置取得
    result = _captureBuffer->GetCurrentPosition(NULL, &inputReadPos);
    if (FAILED(result))
    {
        LAppPal::PrintLogLn("[APP]Failed to GetCurrentPosition() in LAppAudioManager::UpdateForMicrophone()");
        return false;
    }

    inputSize = inputReadPos - _inputPos;
    if (inputSize < 0)
    {
        // 負の値の場合一周しているためバッファの大きさを足す
        inputSize += _bufferSampleBytes;
    }
    else if (inputSize == 0)
    {
        //　書き込まれていないため処理しない
        return false;
    }

    // 録音側のロック
    result = _captureBuffer->Lock(_inputPos, inputSize, &inputSoundBuffer1, &inputSoundBufferSize1, &inputSoundBuffer2, &inputSoundBufferSize2, NULL);
    if (FAILED(result))
    {
        LAppPal::PrintLogLn("[APP]Failed to Lock() in LAppAudioManager::UpdateForMicrophone()");
        return false;
    }

    // 録音側の次回の読み取り位置を更新
    _inputPos = (_inputPos + inputSoundBufferSize1 + inputSoundBufferSize2) % _bufferSampleBytes;

    // 音声データをオーディオ側に書き込み
    LPVOID outputSoundBuffer1, outputSoundBuffer2;
    DWORD outputSoundBufferSize1, outputSoundBufferSize2;

    if (!_secondary)
    {
        LAppPal::PrintLogLn("[APP]_secondary is null in LAppAudioManager::UpdateForMicrophone()");
        return false;
    }

    // 再生側のロック
    result = _secondary->Lock(_outputPos, inputSoundBufferSize1 + inputSoundBufferSize2, &outputSoundBuffer1, &outputSoundBufferSize1, &outputSoundBuffer2, &outputSoundBufferSize2, 0);
    if (FAILED(result))
    {
        LAppPal::PrintLogLn("[APP]Failed to Lock() in LAppAudioManager::UpdateForMicrophone()");
        return false;
    }

    // 録音側のバッファを再生側のバッファに書き込み
    for (DWORD i = 0; i < outputSoundBufferSize1 && i < inputSoundBufferSize1; i++)
    {
        reinterpret_cast<csmByte*>(outputSoundBuffer1)[i] = reinterpret_cast<csmByte*>(inputSoundBuffer1)[i];
    }
    for (DWORD i = 0; i < outputSoundBufferSize2 && i < inputSoundBufferSize2; i++)
    {
        reinterpret_cast<csmByte*>(outputSoundBuffer2)[i] = reinterpret_cast<csmByte*>(inputSoundBuffer2)[i];
    }

    // 再生側のロックの解除
    result = _secondary->Unlock(outputSoundBuffer1, outputSoundBufferSize1, outputSoundBuffer2, outputSoundBufferSize2);
    if (FAILED(result))
    {
        LAppPal::PrintLogLn("[APP]Failed to Unlock() in LAppAudioManager::UpdateForMicrophone()");
        return false;
    }

    // リングバッファに正規化した値を格納
    csmUint32 byte = _bitDepth / 8;
    for (DWORD i = 0; i < inputSoundBufferSize1; i += byte)
    {
        // 指定したチャンネルのみ格納
        if ((i / byte) % _channels == _useChannel)
        {
            _buffer.AddValue(LAppWavFileHandler::NormalizePcmSample(_bitDepth, &reinterpret_cast<csmByte*>(inputSoundBuffer1)[i], inputSoundBufferSize1 - i));
        }
    }
    for (DWORD i = 0; i < inputSoundBufferSize2; i += byte)
    {
        // 指定したチャンネルのみ格納
        if ((i / byte) % _channels == _useChannel)
        {
            _buffer.AddValue(LAppWavFileHandler::NormalizePcmSample(_bitDepth, &reinterpret_cast<csmByte*>(inputSoundBuffer2)[i], inputSoundBufferSize2 - i));
        }
    }

    // 再生側の次の書き込み位置を決める
    _outputPos = (_outputPos + inputSoundBufferSize1 + inputSoundBufferSize2) % _bufferSampleBytes;

    // 録音側のロックの解除
    result = _captureBuffer->Unlock(inputSoundBuffer1, inputSoundBufferSize1, inputSoundBuffer2, inputSoundBufferSize2);
    if (FAILED(result))
    {
        LAppPal::PrintLogLn("[APP]Failed to Unlock() in LAppAudioManager::UpdateForMicrophone()");
        return false;
    }

    return true;
}

csmBool LAppAudioManager::UpdateForAudioFile()
{
    HRESULT result;
    DWORD playCursor;
    LPVOID soundBuffer1, soundBuffer2;
    DWORD soundBufferSize1, soundBufferSize2;
    csmUint32 bufferBytes = _bufferSampleBytes / 2;

    if (!_isLoadFile)
    {
        // 早送りボタンが押されていないため音声の再生がない
        return true;
    }

    if (!_secondary)
    {
        LAppPal::PrintLogLn("[APP]_secondary is null in LAppAudioManager::UpdateForAudioFile()");
        return false;
    }

    result = _secondary->GetCurrentPosition(&playCursor, NULL);
    if (FAILED(result))
    {
        LAppPal::PrintLogLn("[APP]Failed to GetCurrentPosition() in LAppAudioManager::UpdateForAudioFile()");
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
            LAppPal::PrintLogLn("[APP]Failed to Lock() in LAppAudioManager::UpdateForAudioFile()");
            return false;
        }
        for (DWORD i = 0; i < soundBufferSize1 && _dataPos < _dataSize; i++)
        {
            reinterpret_cast<csmByte*>(soundBuffer1)[i] = _data[_dataPos];
            _dataPos++;
        }
        for (DWORD i = 0; i < soundBufferSize2 && _dataPos < _dataSize; i++)
        {
            reinterpret_cast<csmByte*>(soundBuffer2)[i] = _data[_dataPos];
            _dataPos++;
        }
        result = _secondary->Unlock(soundBuffer1, soundBufferSize1, soundBuffer2, soundBufferSize2);
        if (FAILED(result))
        {
            LAppPal::PrintLogLn("[APP]Failed to Unlock() in LAppAudioManager::UpdateForAudioFile()");
            return false;
        }

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
        result = _secondary->Lock(bufferBytes, bufferBytes, &soundBuffer1, &soundBufferSize1, &soundBuffer2, &soundBufferSize2, 0);
        if (FAILED(result))
        {
            LAppPal::PrintLogLn("[APP]Failed to Lock() in LAppAudioManager::UpdateForAudioFile()");
            return false;
        }
        for (DWORD i = 0; i < soundBufferSize1 && _dataPos < _dataSize; i++)
        {
            reinterpret_cast<csmByte*>(soundBuffer1)[i] = _data[_dataPos];
            _dataPos++;
        }
        for (DWORD i = 0; i < soundBufferSize2 && _dataPos < _dataSize; i++)
        {
            reinterpret_cast<csmByte*>(soundBuffer2)[i] = _data[_dataPos];
            _dataPos++;
        }
        result = _secondary->Unlock(soundBuffer1, soundBufferSize1, soundBuffer2, soundBufferSize2);
        if (FAILED(result))
        {
            LAppPal::PrintLogLn("[APP]Failed to Unlock() in LAppAudioManager::UpdateForAudioFile()");
            return false;
        }

        // 生の音声データがなくなったら再生終了
        if (_dataSize <= _dataPos)
        {
            _secondary->Stop();
        }
    }

    // 音声データを口形バッファに書き込み
    csmUint32 pos = _dataPos / _channels / (_bitDepth / 8);
    for (csmUint32 i = _samplesPos; i < pos; i++)
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
            _secondary->Stop();
            break;
        }
   }

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
    if (_secondary)
    {
        DWORD status;
        _secondary->GetStatus(&status);
        return status & DSBSTATUS_PLAYING;
    }
    return false;
}

void LAppAudioManager::Release()
{
    // 録音部分の解放
    if (_captureBuffer)
    {
        _captureBuffer->Release();
        _captureBuffer = NULL;
    }
    if (_soundCapture)
    {
        _soundCapture->Release();
        _soundCapture = NULL;
    }
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
}

LAppAudioManager::LAppAudioManager() :
    _soundCapture(NULL),
    _captureBuffer(NULL),
    _secondary(NULL),
    _channels(1),
    _bitDepth(8),
    _data(NULL),
    _dataSize(0),
    _dataPos(0),
    _samples(NULL),
    _samplesSize(0),
    _samplesPos(0),
    _buffer(),
    _isLoadFile(false),
    _bufferSampleBytes(0),
    _useChannel(0),
    _inputPos(0),
    _outputPos(0)
{
}

LAppAudioManager::~LAppAudioManager()
{
    Release();
}
