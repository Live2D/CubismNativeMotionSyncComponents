/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include "CubismFramework.hpp"

namespace Live2D { namespace Cubism { namespace Framework { namespace MotionSync {

/**
 * @brief モーションシンクで使用するリングバッファ
 *
 * モーションシンクで使用するリングバッファを提供。
 */
template<class T>
class CubismMotionSyncAudioBuffer
{
public:
    /**
     * @brief コンストラクタ
     *
     */
    CubismMotionSyncAudioBuffer();

    /**
     * @brief デストラクタ
     */
    virtual ~CubismMotionSyncAudioBuffer();

    /**
     * @brief リングバッファの大きさを再設定
     *
     * @param[in]   bufferSize  バッファの大きさ
     */
    void Resize(csmUint32 bufferSize, csmBool useOutputBuffer = true);

    /**
     * @brief リングバッファに値を追加する
     *
     * @param[in]   value リングバッファに追加する値
     */
    void AddValue(const T &value);

    /**
     * @brief リングバッファのデータを減らす
     *
     * @param[in]   num リングバッファのデータを指定数削除する
     */
    void Remove(csmUint32 num);

    /**
     * @brief 使用しているリングバッファの大きさを返す
     *
     */
    csmUint32 GetSize();

    /**
     * @brief リングバッファの値を出力用配列に更新する
     *
     */
    void UpdateOutputBuffer();

    /**
     * @brief 出力用配列を取得する
     *
     */
    const T* GetOutputBuffer();

    /**
     * @brief 出力用配列の大きさを取得する
     *
     */
    csmUint32 GetOutputBufferSize();

    /**
     * @brief   []演算子のオーバーロード(const)
     *
     */
    const T& operator[](csmInt32 index) const
    {
        return _ringBuffer[(_begin + index) % _ringBufferSize];
    }

private:
    /*
     * _begin == _endの時、空状態
     * _begin == (_end + 1) % _ringBufferSizeの時、満タン
     */

    // リングバッファ
    T* _ringBuffer;
    // リングバッファの大きさ
    csmUint32 _ringBufferSize;
    // リングバッファの始点
    csmUint32 _begin;
    // リングバッファの終点
    csmUint32 _end;
    // リングバッファの使用している大きさ
    csmUint32 _usingBufferSize;

    // 出力用配列
    T* _outputBuffer;
    // 出力用配列の大きさ
    csmUint32 _outputBufferSize;

    /**
     * @brief メモリの開放を行う
     *
     */
    void Release();
};

//========================テンプレートの定義==============================

template<class T>
CubismMotionSyncAudioBuffer<T>::CubismMotionSyncAudioBuffer() :
    _ringBuffer(NULL),
    _ringBufferSize(0),
    _begin(0),
    _end(0),
    _usingBufferSize(0),
    _outputBuffer(NULL),
    _outputBufferSize(0)
{
}

template<class T>
CubismMotionSyncAudioBuffer<T>::~CubismMotionSyncAudioBuffer()
{
    Release();
}

template<class T>
void CubismMotionSyncAudioBuffer<T>::Resize(csmUint32 bufferSize, csmBool useOutputBuffer)
{
    Release();

    if (0 < bufferSize)
    {
        _ringBufferSize = bufferSize + 1;

        // 満タン状態かどうか判断するためにbuffer + 1の大きさで確保する
        _ringBuffer = static_cast<T*>(CSM_MALLOC(sizeof(T) * (_ringBufferSize)));
        
        // メモリ確保確認
        if (!_ringBuffer)
        {
            _ringBufferSize = 0;
            return;
        }
        
        for (csmUint32 i = 0; i < _ringBufferSize; i++)
        {
            CSM_PLACEMENT_NEW(&_ringBuffer[i]) T();
        }

        if (useOutputBuffer)
        {
            // リングバッファで使用するバッファは必ず一つ空ける必要がある
            _outputBuffer = static_cast<T*>(CSM_MALLOC(sizeof(T) * _ringBufferSize - 1));
            
            // メモリ確保確認
            if (!_outputBuffer)
            {
                Release();
            }
            
            for (csmUint32 i = 0; i < _ringBufferSize - 1; i++)
            {
                CSM_PLACEMENT_NEW(&_outputBuffer[i]) T();
            }
        }
    }
}

template<class T>
void CubismMotionSyncAudioBuffer<T>::AddValue(const T &value)
{
    if (_ringBuffer)
    {
        csmUint32 nextBegin = (_begin + 1) % _ringBufferSize;
        csmUint32 nextEnd = (_end + 1) % _ringBufferSize;

        if (_begin != nextEnd)
        {
            // 空いている場合
            _ringBuffer[_end] = value;
            _end = nextEnd;
            _usingBufferSize++;
        }
        else
        {
            // 満タンの場合
            _ringBuffer[_end] = value;
            _begin = nextBegin;
            _end = nextEnd;
        }
    }
}

template<class T>
void CubismMotionSyncAudioBuffer<T>::Remove(csmUint32 num)
{
    if (num < _usingBufferSize)
    {
        // 一部消し
        _begin = (_begin + num) % _ringBufferSize;
        _usingBufferSize -= num;
    }
    else
    {
        // 全消し
        _begin = 0;
        _end = 0;
        _usingBufferSize = 0;
    }
}

template<class T>
csmUint32 CubismMotionSyncAudioBuffer<T>::GetSize()
{
    return _usingBufferSize;
}

template<class T>
void CubismMotionSyncAudioBuffer<T>::UpdateOutputBuffer()
{
    _outputBufferSize = 0;
    if (_outputBuffer)
    {
        for (csmUint32 iter = _begin; iter != _end; iter = (iter + 1) % _ringBufferSize)
        {
            _outputBuffer[_outputBufferSize++] = _ringBuffer[iter];
        }
    }
}

template<class T>
const T* CubismMotionSyncAudioBuffer<T>::GetOutputBuffer()
{
    return _outputBuffer;
}

template<class T>
csmUint32 CubismMotionSyncAudioBuffer<T>::GetOutputBufferSize()
{
    return _outputBufferSize;
}

template<class T>
void CubismMotionSyncAudioBuffer<T>::Release()
{
    if (_outputBuffer)
    {
        for (csmUint32 i = 0; i < _ringBufferSize - 1; i++)
        {
            _outputBuffer[i].~T();
        }
        CSM_FREE(_outputBuffer);
        _outputBuffer = NULL;
        _outputBufferSize = 0;
    }
    if (_ringBuffer)
    {
        for (csmUint32 i = 0; i < _ringBufferSize; i++)
        {
            _ringBuffer[i].~T();
        }
        CSM_FREE(_ringBuffer);
        _ringBuffer = NULL;
        _ringBufferSize = 0;
        _begin = 0;
        _end = 0;
        _usingBufferSize = 0;
    }
}

}}}}
