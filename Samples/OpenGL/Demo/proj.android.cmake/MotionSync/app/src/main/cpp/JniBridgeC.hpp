/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once
#include <Type/csmVector.hpp>
#include <Type/csmString.hpp>

/**
* @brief Jni Bridge Class
*/
class JniBridgeC
{
public:
    /**
    * @brief Assets 取得
    */
    static Csm::csmVector<Csm::csmString> GetAssetList(const Csm::csmString& path);

    /**
    * @brief Javaからファイル読み込み
    */
    static char* LoadFileAsBytesFromJava(const char* filePath, unsigned int* outSize);

    /**
    * @brief libのパスを取得する
    */
    static Csm::csmString GetJniLibPath();

    /**
    * @brief 録音用の音声バッファを作成
    */
    static Csm::csmInt32 CreateAudioInputBuffer(Csm::csmInt32 sampleRate, Csm::csmInt32 channels, Csm::csmInt32 bitDepth);

    /**
    * @brief 録音用の音声バッファに読み込み
    */
    static Csm::csmByte* ReadAudioInputBuffer(Csm::csmInt32 id, Csm::csmUint32* outSize);

    /**
    * @brief 録音用の音声バッファの録音を停止する
    */
    static Csm::csmBool StopAudioInputBuffer(Csm::csmInt32 id);

    /**
    * @brief 録音用の音声バッファを削除
    */
    static Csm::csmBool DeleteAudioInputBuffer(Csm::csmInt32 id);

    /**
    * @brief 再生用の音声バッファを作成
    */
    static Csm::csmInt32 CreateAudioOutputBuffer(Csm::csmInt32 sampleRate, Csm::csmInt32 channels, Csm::csmInt32 bitDepth);

    /**
    * @brief 再生用の音声バッファに書き込み
    */
    static Csm::csmBool WriteAudioOutputBuffer(Csm::csmInt32 id, Csm::csmInt32 writePos, Csm::csmByte* data, Csm::csmInt32 dataSize);

    /**
    * @brief 再生用の音声バッファの再生位置を取得
    */
    static Csm::csmInt32 GetAudioOutputBufferPosition(Csm::csmInt32 id);

    /**
    * @brief 再生用の音声バッファの再生を停止する
    */
    static Csm::csmBool StopAudioOutputBuffer(Csm::csmInt32 id);

    /**
    * @brief 再生用の音声バッファを削除
    */
    static Csm::csmBool DeleteAudioOutputBuffer(Csm::csmInt32 id);

    /**
    * @brief アプリをバックグラウンドに移動
    */
    static void MoveTaskToBack();

};
