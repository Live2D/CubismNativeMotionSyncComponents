/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppPlaySound.hpp"
#include "LAppWavFileHandler.hpp"

using namespace Csm;

csmBool LAppPlaySound::LoadFile(csmString path, csmUint32 useChannel)
{
    // 初期化
    Release();
    
    AudioStreamBasicDescription format;
    AudioQueueBufferRef buffers[LAppMotionSyncDefine::AudioQueueBufferCount];
    OSStatus status;
     
    // WAVファイルをロード
    LAppWavFileHandler wavHandler;
    wavHandler.Start(path);
    LAppWavFileHandler::WavFileInfo wavHandlerInfo = wavHandler.GetWavFileInfo();
    _wavSamples = wavHandler.GetPcmData();
    
    // 再生設定
    format.mSampleRate = wavHandlerInfo._samplingRate;
    format.mFormatID = kAudioFormatLinearPCM;
    format.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
    format.mBitsPerChannel = sizeof(csmFloat32) * 8;
    format.mChannelsPerFrame = wavHandlerInfo._numberOfChannels;
    format.mBytesPerFrame = sizeof(csmFloat32) * format.mChannelsPerFrame;
    format.mFramesPerPacket = 1;
    format.mBytesPerPacket = format.mBytesPerFrame * format.mFramesPerPacket;
    format.mReserved = 0;
    
    _channels = wavHandlerInfo._numberOfChannels;
    _bitDepth = wavHandlerInfo._bitsPerSample;
    _queueBufferSize = LAppMotionSyncDefine::AudioQueueBufferSampleCount * format.mBytesPerFrame;
    _queueBufferSampleCount = LAppMotionSyncDefine::AudioQueueBufferSampleCount;
    _wavWritePos = 0;
    
    if (_useChannel < _channels)
    {
        _useChannel = useChannel;
    }
    else
    {
        _useChannel = _channels - 1;
    }
    
    
    status = AudioQueueNewOutput(&format, CallBack, this, CFRunLoopGetCurrent(), kCFRunLoopCommonModes, 0, &_queue);
    
    if (status != noErr)
    {
        return false;
    }
    
    
    for (csmUint32 i = 0; i < LAppMotionSyncDefine::AudioQueueBufferCount; i++)
    {
        status = AudioQueueAllocateBuffer(_queue, _queueBufferSize, &buffers[i]);
                
        if (status != noErr)
        {
            return false;
        }
        
        buffers[i]->mAudioDataByteSize = _queueBufferSize;
        
        // キューをキックして音を鳴らす。
        CallBack(this, _queue, buffers[i]);
    }
    
    // 音声再生
    status = AudioQueueStart(_queue, NULL);
    
    if (status != noErr)
    {
        return false;
    }
    
    
    return true;
}

csmVector<csmFloat32>* LAppPlaySound::GetBuffer()
{
    return &_buffer;
}

csmBool LAppPlaySound::IsPlay()
{
    return (_wavWritePos < _wavSamples.GetSize());
}

void LAppPlaySound::Release()
{
    AudioQueueStop(_queue, true);
    AudioQueueDispose(_queue, true);
    _buffer.Clear();
}

LAppPlaySound::LAppPlaySound() :
    _queueBufferSize(0),
    _queueBufferSampleCount(0),
    _wavWritePos(0),
    _useChannel(0),
    _channels(1),
    _bitDepth(8)
{
}

LAppPlaySound::~LAppPlaySound()
{
    Release();
}

void LAppPlaySound::CallBack(void* customData, AudioQueueRef queue, AudioQueueBufferRef buffer)
{
    LAppPlaySound* data = reinterpret_cast<LAppPlaySound*>(customData);
    csmFloat32 *samples = reinterpret_cast<csmFloat32*>(buffer->mAudioData);
    
    if (data->_wavSamples.GetSize() <= data->_wavWritePos)
    {
        return;
    }
    
    for (csmUint64 i = 0; i < data->_queueBufferSampleCount * data->_channels; i++)
    {
        if (data->_wavWritePos < data->_wavSamples.GetSize())
        {
            samples[i] = data->_wavSamples[data->_wavWritePos++];
        }
        else
        {
            samples[i] = 0.0f;
        }
        
        // 解析に指定しているチャンネルのサンプルを送る。
        if ((i % 2) == data->_useChannel)
        {
            data->_buffer.PushBack(samples[i]);
        }
    }
    
    AudioQueueEnqueueBuffer(queue, buffer, 0, NULL);
}

