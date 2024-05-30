/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#import "LAppMicrophoneAudioManager.h"
#import <AVFoundation/AVAudioSession.h>
#import "LAppWavFileHandler.h"
#import "LAppPal.h"

using namespace Csm;

csmBool LAppMicrophoneAudioManager::SetupMicrophone(Csm::csmUint32 channels, Csm::csmUint32 samplesRate, Csm::csmUint32 bitDepth, Csm::csmUint32 useChannel)
{
    // 初期化
    Release();
    
    AudioStreamBasicDescription format;
    AudioQueueBufferRef inputBuffers[LAppMotionSyncDefine::AudioQueueBufferCount];
    AudioQueueBufferRef outputBuffers[LAppMotionSyncDefine::AudioQueueBufferCount];
    OSStatus status;

    AVAudioSession* audioSession = [AVAudioSession sharedInstance];
    BOOL success = [audioSession setCategory:AVAudioSessionCategoryPlayAndRecord
                                        mode:AVAudioSessionModeDefault
                                     options:AVAudioSessionCategoryOptionAllowBluetooth
                                       error:nil];
    if (!success)
    {
        CubismLogError("[APP]Failed to AVAudioSession.setCategory() in LAppMicrophoneAudioManager::SetupMicrophone()");
        return false;
    }

    csmUint32 blockAlign = channels * (bitDepth / 8);
    
    // オーディオ設定
    format.mSampleRate = samplesRate;
    format.mFormatID = kAudioFormatLinearPCM;
    format.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsPacked;
    format.mBitsPerChannel = sizeof(csmFloat32) * 8;
    format.mChannelsPerFrame = channels;
    format.mBytesPerFrame = sizeof(csmFloat32) * format.mChannelsPerFrame;
    format.mFramesPerPacket = 1;
    format.mBytesPerPacket = format.mBytesPerFrame * format.mFramesPerPacket;
    format.mReserved = 0;
    
    _channels = channels;
    _queueBufferSize = LAppMotionSyncDefine::AudioQueueBufferSampleCount * format.mBytesPerFrame;
    _queueBufferSampleCount = LAppMotionSyncDefine::AudioQueueBufferSampleCount;
    
    // 入力された音声データを保持しておくバッファを確保
    _inputDataList.Resize(LAppMotionSyncDefine::CsmInputBufferSize, false);
    
    // リングバッファ確保
    _buffer.Resize(LAppMotionSyncDefine::CsmRingBufferSize * blockAlign);
    
    // パラメータに反映させるチャンネルを決定
    if (_useChannel < _channels)
    {
        _useChannel = useChannel;
    }
    else
    {
        _useChannel = _channels - 1;
    }
    
    // 録音部分作成
    status = AudioQueueNewInput(&format, InputCallBack, this, NULL, NULL, 0, &_inputQueue);
    if (status != noErr)
    {
        LAppPal::PrintLogLn("[APP]Failed to AudioQueueNewInput() in LAppMicrophoneAudioManager::SetupMicrophone()");
        return false;
    }
    
    for (csmUint32 i = 0; i < LAppMotionSyncDefine::AudioQueueBufferCount; i++)
    {
        status = AudioQueueAllocateBuffer(_inputQueue, _queueBufferSize, &inputBuffers[i]);
        if (status != noErr)
        {
            LAppPal::PrintLogLn("[APP]Failed to AudioQueueAllocateBuffer() in LAppMicrophoneAudioManager::SetupMicrophone()");
            return false;
        }
        
        inputBuffers[i]->mAudioDataByteSize = _queueBufferSize;
        
        AudioQueueEnqueueBuffer(_inputQueue, inputBuffers[i], 0, NULL);
    }
        
    // 再生部分作成
    status = AudioQueueNewOutput(&format, OutputCallBack, this, CFRunLoopGetCurrent(), kCFRunLoopCommonModes, 0, &_outputQueue);
    if (status != noErr)
    {
        LAppPal::PrintLogLn("[APP]Failed to AudioQueueNewOutput() in LAppMicrophoneAudioManager::SetupMicrophone()");
        return false;
    }
    
    for (csmUint32 i = 0; i < LAppMotionSyncDefine::AudioQueueBufferCount; i++)
    {
        status = AudioQueueAllocateBuffer(_outputQueue, _queueBufferSize, &outputBuffers[i]);
        if (status != noErr)
        {
            LAppPal::PrintLogLn("[APP]Failed to AudioQueueAllocateBuffer() in LAppMicrophoneAudioManager::SetupMicrophone()");
            return false;
        }
        
        outputBuffers[i]->mAudioDataByteSize = _queueBufferSize;
        
        OutputCallBack(this, _outputQueue, outputBuffers[i]);
    }
    
    // 録音開始
    status = AudioQueueStart(_inputQueue, NULL);
    if (status != noErr)
    {
        LAppPal::PrintLogLn("[APP]Failed to AudioQueueStart() in LAppMicrophoneAudioManager::SetupMicrophone()");
        return false;
    }
    
    // 再生開始
    status = AudioQueueStart(_outputQueue, NULL);
    if (status != noErr)
    {
        LAppPal::PrintLogLn("[APP]Failed to AudioQueueStart() in LAppMicrophoneAudioManager::SetupMicrophone()");
        return false;
    }
    
    return true;
}

MotionSync::CubismMotionSyncAudioBuffer<Csm::csmFloat32>* LAppMicrophoneAudioManager::GetBuffer()
{
    return &_buffer;
}

void LAppMicrophoneAudioManager::Release()
{
    AudioQueueStop(_inputQueue, true);
    AudioQueueStop(_outputQueue, true);
    AudioQueueDispose(_inputQueue, true);
    AudioQueueDispose(_outputQueue, true);
}

LAppMicrophoneAudioManager::LAppMicrophoneAudioManager() :
    _queueBufferSize(0),
    _queueBufferSampleCount(0),
    _useChannel(0),
    _channels(1),
    _buffer()
{
}

LAppMicrophoneAudioManager::~LAppMicrophoneAudioManager()
{
    Release();
}

void LAppMicrophoneAudioManager::WriteInputBuffer(csmFloat32* samples)
{
    _mutex.Lock();
    
    if (_inputQueue)
    {
        // マイク入力の内容を受け取る。
        csmVector<csmFloat32> inputData;
        for (csmUint32 i = 0; i < _queueBufferSampleCount * _channels; i++)
        {
            inputData.PushBack(samples[i]);
        }
        _inputDataList.AddValue(inputData);
    }
    
    _mutex.Unlock();
}

void LAppMicrophoneAudioManager::ReadInputBuffer(csmFloat32* samples)
{
    _mutex.Lock();
    
    if (0 < _inputDataList.GetSize())
    {
        // 録音した音声を送る。
        for (csmUint32 i = 0; i < _queueBufferSampleCount * _channels && i < _inputDataList[0].GetSize(); i++)
        {
            samples[i] = _inputDataList[0][i];
            
            // 解析に指定しているチャンネルのサンプルを送る。
            if ((i % 2) == _useChannel)
            {
                _buffer.AddValue(samples[i]);
            }
        }
        _inputDataList.Remove(1);
    }
    else
    {
        for (csmUint32 i = 0; i < _queueBufferSampleCount * _channels; i++)
        {
            samples[i] = 0.0f;
        }
    }
    
    _mutex.Unlock();
}

void LAppMicrophoneAudioManager::InputCallBack(void* customData, AudioQueueRef queue, AudioQueueBufferRef buffer, const AudioTimeStamp *startTime, UInt32 packetNum, const AudioStreamPacketDescription *packetDesc)
{
    LAppMicrophoneAudioManager* data = reinterpret_cast<LAppMicrophoneAudioManager*>(customData);
    csmFloat32 *samples = reinterpret_cast<csmFloat32*>(buffer->mAudioData);
    if (0 < packetNum)
    {
        data->WriteInputBuffer(samples);
    }
    AudioQueueEnqueueBuffer(queue, buffer, 0, NULL);
}

void LAppMicrophoneAudioManager::OutputCallBack(void* customData, AudioQueueRef queue, AudioQueueBufferRef buffer)
{
    LAppMicrophoneAudioManager* data = reinterpret_cast<LAppMicrophoneAudioManager*>(customData);
    csmFloat32 *samples = reinterpret_cast<csmFloat32*>(buffer->mAudioData);

    data->ReadInputBuffer(samples);
    
    AudioQueueEnqueueBuffer(queue, buffer, 0, NULL);
}
