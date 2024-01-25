/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

package com.live2d.demo;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Build;
import android.util.Log;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class AudioOutput {
    /**
     * @brief 再生用の音声バッファを作成
     *
     * @param[in]       sampleRate        サンプリング周波数
     * @param[in]       channels          チャンネル数
     * @param[in]       bitDepth          ビット深度
     *
     * @return OutputAudio
     */
    public static AudioOutput create(int sampleRate, int channels, int bitDepth) {
        int channelConfig;
        int audioFormat;

        switch (channels) {
            case 1:
                channelConfig = AudioFormat.CHANNEL_OUT_MONO;
                break;
            case 2:
                channelConfig = AudioFormat.CHANNEL_OUT_STEREO;
                break;
            default:
                return null;
        }
        switch (bitDepth) {
            case 8:
                audioFormat = AudioFormat.ENCODING_PCM_8BIT;
                break;
            case 16:
                audioFormat = AudioFormat.ENCODING_PCM_16BIT;
                break;
            case 32:
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
                    audioFormat = AudioFormat.ENCODING_PCM_32BIT;
                }
                else {
                    // API31未満は32bitは非対応
                    Log.e("[APP]", "Less than API 31, 32bit is not supported");
                    return null;
                }
                break;
            default:
                return null;
        }

        return new AudioOutput(sampleRate, channelConfig, audioFormat);
    }

    /**
     * @brief 音声バッファにデータを書き込む
     *
     * @param[in]       writePos          書き込み開始位置
     * @param[in]       data              書き込みデータ
     * @param[in]       dataSize          書き込みデータの大きさ
     */
    public void writeBuffer(int writePos, byte[] data, int dataSize) {
        WriteData writeData = new WriteData(writePos, data, dataSize);
        writeDataBuffer.add(writeData);
    }

    /**
     * @brief 音声バッファの再生位置を取得
     *
     * @return 再生位置
     */
    public int getPlayPosition() {
        return audioTrack.getPlaybackHeadPosition();
    }

    /**
     * @brief 音声バッファの再生を停止
     */
    public void stop() {
        audioTrack.stop();
    }

    /**
     * @brief 再生用の音声バッファを削除
     */
    public void delete() {
        isLoop = false;
        audioTrack.stop();
        execThread.shutdown();
        audioTrack.release();
    }

    /**
     * @brief スレッド処理
     */
    public void run() {
        while (isLoop) {
            if (!writeDataBuffer.isEmpty()) {
                WriteData writeData = writeDataBuffer.get(0);
                audioTrack.write(writeData.data, writeData.writePos, writeData.dataSize);
                writeDataBuffer.remove(0);
            }
        }
    }

    /**
     * @brief コンストラクタ
     *
     * @param[in]       sampleRate        サンプリング周波数
     * @param[in]       channelConfig     チャンネルコンフィグ
     * @param[in]       audioFormat       オーディオフォーマット
     */
    private AudioOutput(int sampleRate, int channelConfig, int audioFormat) {
        int minBufferSize = AudioTrack.getMinBufferSize(sampleRate, channelConfig, audioFormat);

        audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate, channelConfig, audioFormat, minBufferSize, AudioTrack.MODE_STREAM);
        isLoop = true;
        execThread = Executors.newSingleThreadExecutor();

        audioTrack.play();
        execThread.submit(new OutputTask(this));
    }
    
    // 音声再生クラス
    private final AudioTrack audioTrack;
    // スレッド処理クラス
    private final ExecutorService execThread;
    // 書き込むデータのバッファ
    private final List<WriteData> writeDataBuffer = Collections.synchronizedList(new ArrayList<WriteData>());
    // スレッドループ用フラグ
    private boolean isLoop;

    /**
     * @brief 音声バッファに書き込むための情報
     */
    private static class WriteData {
        // 書き込み開始位置
        public int writePos;
        // 書き込むデータ
        public byte[] data;
        // 書き込むデータサイズ
        public int dataSize;

        /**
         * @brief コンストラクタ
         *
         * @param[in]       writePos          書き込み開始位置
         * @param[in]       data              書き込みデータ
         * @param[in]       dataSize          書き込みデータの大きさ
         */
        public WriteData(int writePos, byte[] data, int dataSize) {
            this.writePos = writePos;
            this.data = data;
            this.dataSize = dataSize;
        }
    }
}
