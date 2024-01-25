package com.live2d.demo;

public class InputTask implements Runnable {
    private final AudioInput audioInput;

    /**
     * @brief コンストラクタ
     *
     * @param[in]       audioInput          録音用
     */
    public InputTask(AudioInput audioInput) {
        this.audioInput = audioInput;
    }

    /**
     * @brief スレッド処理
     */
    @Override
    public void run() {
        audioInput.run();
    }
}