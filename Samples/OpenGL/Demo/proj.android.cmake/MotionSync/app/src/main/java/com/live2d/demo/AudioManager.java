/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

package com.live2d.demo;

import java.util.HashMap;
import java.util.Map;

public class AudioManager {
    /**
     * @brief 録音用の音声バッファを作成
     *
     * @param[in]       sampleRate        サンプリング周波数
     * @param[in]       channels          チャンネル数
     * @param[in]       bitDepth          ビット深度
     *
     * @return 作成したid
     */
    public static int createAudioInput(int sampleRate, int channels, int bitDepth) {
        int key = createAudioInputKey();
        if (key == 0)
        {
            return 0;
        }

        AudioInput audioInput = AudioInput.create(sampleRate, channels, bitDepth);
        if (audioInput == null)
        {
            return 0;
        }
        audioInputMap.put(key, audioInput);

        return key;
    }

    /**
     * @brief 音声バッファからデータを受け取る
     *
     * @param[in]       id                id
     *
     * @return 録音したデータ
     */
    public static byte[] readAudioInput(int id) {
        if (!audioInputMap.containsKey(id)) {
            return new byte[0];
        }

        AudioInput audioInput = audioInputMap.get(id);
        return audioInput.readBuffer();
    }

    /**
     * @brief 音声バッファの録音を停止
     *
     * @param[in]       id                id
     *
     * @return 正しく処理されたか
     */
    public static boolean stopAudioInput(int id) {
        if (!audioInputMap.containsKey(id)) {
            return false;
        }

        AudioInput audioInput = audioInputMap.get(id);
        audioInput.stop();

        return true;
    }

    /**
     * @brief 録音用の音声バッファを削除
     *
     * @param[in]       id                id
     *
     * @return 正しく処理されたか
     */
    public static boolean deleteAudioInput(int id) {
        if (!audioInputMap.containsKey(id)) {
            return false;
        }

        AudioInput audioInput = audioInputMap.get(id);
        audioInput.delete();

        audioInputMap.remove(id);

        return true;
    }

    /**
     * @brief 再生用の音声バッファを作成
     *
     * @param[in]       sampleRate        サンプリング周波数
     * @param[in]       channels          チャンネル数
     * @param[in]       bitDepth          ビット深度
     *
     * @return 作成したid
     */
    public static int createAudioOutput(int sampleRate, int channels, int bitDepth) {
        int key = createAudioOutputKey();
        if (key == 0)
        {
            return 0;
        }

        AudioOutput audioOutput = AudioOutput.create(sampleRate, channels, bitDepth);
        if (audioOutput == null)
        {
            return 0;
        }
        audioOutputMap.put(key, audioOutput);

        return key;
    }

    /**
     * @brief 音声バッファにデータを送る
     *
     * @param[in]       id                id
     * @param[in]       writePos          書き込み開始位置
     * @param[in]       data              書き込むデータ
     * @param[in]       dataSize          書き込むデータの大きさ
     *
     * @return 正しく処理されたか
     */
    public static boolean writeAudioOutput(int id, int writePos, byte[] data, int dataSize) {
        if (!audioOutputMap.containsKey(id)) {
            return false;
        }

        AudioOutput audioOutput = audioOutputMap.get(id);
        audioOutput.writeBuffer(writePos, data, dataSize);

        return true;
    }

    /**
     * @brief 音声バッファの再生位置を取得
     *
     * @param[in]       id                id
     *
     * @return 再生位置
     */
    public static int getAudioOutputPosition(int id) {
        if (!audioOutputMap.containsKey(id)) {
            return -1;
        }

        AudioOutput audioOutput = audioOutputMap.get(id);

        return audioOutput.getPlayPosition();
    }

    /**
     * @brief 音声バッファの再生を停止
     *
     * @param[in]       id                id
     *
     * @return 正しく処理されたか
     */
    public static boolean stopAudioOutput(int id) {
        if (!audioOutputMap.containsKey(id)) {
            return false;
        }

        AudioOutput audioOutput = audioOutputMap.get(id);
        audioOutput.stop();

        return true;
    }

    /**
     * @brief 再生用の音声バッファを削除
     *
     * @param[in]       id                id
     *
     * @return 正しく処理されたか
     */
    public static boolean deleteAudioOutput(int id) {
        if (!audioOutputMap.containsKey(id)) {
            return false;
        }

        AudioOutput audioOutput = audioOutputMap.get(id);
        audioOutput.delete();

        audioOutputMap.remove(id);

        return true;
    }

    /**
     * @brief inputAudioMapに登録されていないキーを作成する
     *        keyの値は1～keyRangeまでの値を順番に割り振っていく
     *        0の場合は無効値
     *
     * @return 使用できるkey
     */
    private static int createAudioInputKey() {
        int count = 0;

        do {
            // 一周回っても空きがなければ抜ける
            if (KEY_RANGE == count) {
                return 0;
            }
            count++;

            if (keyInputIndex == KEY_RANGE) {
                keyInputIndex = 0;
            }
            keyInputIndex++;
        }
        while (audioInputMap.containsKey(keyInputIndex));

        return keyInputIndex;
    }

    /**
     * @brief outputAudioMapに登録されていないキーを作成する
     *        keyの値は1～keyRangeまでの値を順番に割り振っていく
     *        0の場合は無効値
     *
     * @return 使用できるkey
     */
    private static int createAudioOutputKey() {
        int count = 0;

        do {
            // 一周回っても空きがなければ抜ける
            if (KEY_RANGE == count) {
                return 0;
            }
            count++;

            if (keyOutputIndex == KEY_RANGE) {
                keyOutputIndex = 0;
            }
            keyOutputIndex++;
        }
        while (audioOutputMap.containsKey(keyOutputIndex));

        return keyOutputIndex;
    }

    // 割り振る予定のkeyの最大値
    private static final int KEY_RANGE = Integer.MAX_VALUE;
    // 音声録音を複数扱えるようにmapにする
    private static final Map<Integer, AudioInput> audioInputMap = new HashMap<>();
    // 音声再生を複数扱えるようにmapにする
    private static final Map<Integer, AudioOutput> audioOutputMap = new HashMap<>();
    // 録音用の割り振ったKeyの値の位置
    private static int keyInputIndex = 0;
    // 再生用の割り振ったKeyの値の位置
    private static int keyOutputIndex = 0;
}
