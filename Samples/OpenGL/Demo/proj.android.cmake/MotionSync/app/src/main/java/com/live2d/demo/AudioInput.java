/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

package com.live2d.demo;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.Context;
import android.content.pm.PackageManager;
import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;
import android.os.Build;
import android.util.Log;

import androidx.core.app.ActivityCompat;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class AudioInput {
    public static void SetContext(Context context) {
        AudioInput.context = context;
    }

    /**
     * @brief 録音用の音声バッファを作成
     *
     * @param[in]       sampleRate        サンプリング周波数
     * @param[in]       channels          チャンネル数
     * @param[in]       bitDepth          ビット深度
     *
     * @return InputAudio
     */
    public static AudioInput create(int sampleRate, int channels, int bitDepth) {
        int channelConfig;
        int audioFormat;

        // ここでパーミッションの確認
        if (ActivityCompat.checkSelfPermission(context, Manifest.permission.RECORD_AUDIO) != PackageManager.PERMISSION_GRANTED) {
            return null;
        }

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

        return new AudioInput(sampleRate, channelConfig, audioFormat);
    }

    /**
     * @brief 音声バッファにデータを読み込む
     *
     */
    public byte[] readBuffer() {
        int size = readDataBuffer.size();
        byte[] data = new byte[size * bufferSize];

        for (int i = 0; i < size; i++) {
            byte[] buffer = readDataBuffer.get(0);
            for (int j = 0; j < bufferSize; j++) {
                data[j + i * bufferSize] = buffer[j];
            }
            readDataBuffer.remove(0);
        }

        return data;
    }

    /**
     * @brief 音声バッファの録音を停止
     */
    public void stop() {
        audioRecord.stop();
    }

    /**
     * @brief 録音用の音声バッファを削除
     */
    public void delete() {
        isLoop = false;
        audioRecord.stop();
        execThread.shutdown();
        audioRecord.release();
    }

    /**
     * @brief スレッド処理
     */
    public void run() {
        byte[] data = new byte[bufferSize];

        while (isLoop) {
            audioRecord.read(data, 0, bufferSize);
            readDataBuffer.add(data);
        }
    }

    /**
     * @brief コンストラクタ
     *
     * @param[in] sampleRate        サンプリング周波数
     * @param[in] channelConfig     チャンネルコンフィグ
     * @param[in] audioFormat       オーディオフォーマット
     */
    @SuppressLint("MissingPermission")
    private AudioInput(int sampleRate, int channelConfig, int audioFormat) {
        bufferSize = AudioRecord.getMinBufferSize(sampleRate, channelConfig, audioFormat);

        // パーミッションがなければcreate時にnullを返したいので、ここではパーミッションの確認を行わない
        audioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC, sampleRate, channelConfig, audioFormat, bufferSize);
        isLoop = true;
        execThread = Executors.newSingleThreadExecutor();

        audioRecord.startRecording();
        execThread.submit(new InputTask(this));
    }

    private static Context context;
    // 音声録音クラス
    private final AudioRecord audioRecord;
    // スレッド処理クラス
    private final ExecutorService execThread;
    // 読み込むデータのバッファ
    private final List<byte[]> readDataBuffer = Collections.synchronizedList(new ArrayList<byte[]>());
    // バッファサイズ
    private final int bufferSize;
    // スレッドループ用フラグ
    private boolean isLoop;
}
