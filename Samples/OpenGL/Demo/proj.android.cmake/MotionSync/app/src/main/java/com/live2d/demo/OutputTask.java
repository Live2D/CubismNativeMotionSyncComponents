package com.live2d.demo;

public class OutputTask implements Runnable {
    private final AudioOutput audioOutput;

    /**
     * @brief コンストラクタ
     *
     * @param[in]       audioOutput          再生用
     */
    public OutputTask(AudioOutput audioOutput) {
        this.audioOutput = audioOutput;
    }

    /**
     * @brief スレッド処理
     */
    @Override
    public void run() {
        audioOutput.run();
    }
}
