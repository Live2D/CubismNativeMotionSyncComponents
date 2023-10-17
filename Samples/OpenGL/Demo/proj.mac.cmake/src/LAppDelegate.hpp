/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "LAppAllocator.hpp"
#include "LAppMotionSyncModel.hpp"
#include "LAppSprite.hpp"

class LAppView;
class LAppTextureManager;

/**
* @brief   アプリケーションクラス。
*   Cubism SDK の管理を行う。
*/
class LAppDelegate
{
public:
    /**
    * @brief   クラスのインスタンス（シングルトン）を返す。<br>
    *           インスタンスが生成されていない場合は内部でインスタンを生成する。
    *
    * @return  クラスのインスタンス
    */
    static LAppDelegate* GetInstance();

    /**
    * @brief   クラスのインスタンス（シングルトン）を解放する。
    *
    */
    static void ReleaseInstance();

    /**
    * @brief   APPに必要なものを初期化する。
    */
    bool Initialize();

    /**
    * @brief   解放する。
    */
    void Release();

    /**
    * @brief   実行処理。
    */
    void Run();

    /**
    * @brief   OpenGL用 glfwSetMouseButtonCallback用関数。
    *
    * @param[in]       window            コールバックを呼んだWindow情報
    * @param[in]       button            ボタン種類
    * @param[in]       action            実行結果
    * @param[in]       modify
    */
    void OnMouseCallBack(GLFWwindow* window, int button, int action, int modify);

    /**
    * @brief   OpenGL用 glfwSetCursorPosCallback用関数。
    *
    * @param[in]       window            コールバックを呼んだWindow情報
    * @param[in]       x                 x座標
    * @param[in]       y                 y座標
    */
    void OnMouseCallBack(GLFWwindow* window, double x, double y);

    /**
    * @brief   Window情報を取得する。
    */
    GLFWwindow* GetWindow() { return _window; }

    /**
    * @brief   アプリケーションを終了するかどうか。
    */
    bool GetIsEnd() { return _isEnd; }

    /**
    * @brief   アプリケーションを終了する。
    */
    void AppEnd() { _isEnd = true; }

    LAppTextureManager* GetTextureManager() { return _textureManager; }
    
    /**
     * @brief   実行中のプログラムファイルパスを取得する。
     */
    std::string GetExecuteAbsolutePath(){ return _executeAbsolutePath;}
    
private:
    /**
    * @brief   コンストラクタ
    */
    LAppDelegate();

    /**
    * @brief   デストラクタ
    */
    ~LAppDelegate();

    /**
    * @brief   Cubism SDK の初期化
    */
    void InitializeCubism();

    /**
     * @brief タッチされたときに呼ばれる。
     *
     * @param[in]       pointX            スクリーンX座標
     * @param[in]       pointY            スクリーンY座標
     */
    void OnTouchesBegan(float pointX, float pointY);

    /**
    * @brief タッチしているときにポインタが動いたら呼ばれる。
    *
    * @param[in]       pointX            スクリーンX座標
    * @param[in]       pointY            スクリーンY座標
    */
    void OnTouchesMoved(float pointX, float pointY);

    /**
    * @brief タッチが終了したら呼ばれる。
    *
    * @param[in]       pointX            スクリーンX座標
    * @param[in]       pointY            スクリーンY座標
    */
    void OnTouchesEnded(float pointX, float pointY);

    /**
     * @brief   CreateShader内部関数 エラーチェック
     */
    bool CheckShader(GLuint shaderId);

    /**
     * @brief モデル名リストの読み込み
     *
     * モデル名リストの読み込み処理を行う
     *
     */
    void LoadModelName();

    /**
     * @brief モデルの読み込み
     *
     * モデルデータの読み込み処理を行う
     *
     */
    void LoadModel();

    /**
     * @brief 次のモデルに切り替え
     *
     */
    void ChangeNextModel();

    /**
     * @brief 背景とボタンの読み込み
     *
     * 背景とボタンの読み込み処理を行う
     *
     */
    void LoadGraph();

    /**
     * @brief スプライト系のサイズ再設定
     */
    void ResizeSprite();

    /**
    * @brief シェーダーを登録する。
    */
    GLuint CreateShader();
    
    /**
     * @brief   実行中のプログラムファイルパスを設定する
     */
    void SetExecuteAbsolutePath();
      
    GLFWwindow* _window; ///< OpenGL ウィンドウ
    int _windowWidth; ///< ウィンドウサイズ幅の保存
    int _windowHeight; ///< ウィンドウサイズ高さの保存
    LAppTextureManager* _textureManager; ///< テクスチャマネージャー

    LAppAllocator _cubismAllocator; ///< Cubism SDK Allocator
    Csm::CubismFramework::Option _cubismOption; ///< Cubism SDK Option
    bool _captured; ///< クリックしているか
    float _mouseX; ///< マウスX座標
    float _mouseY; ///< マウスY座標
    bool _isEnd; ///< APP終了しているか

    std::string _executeAbsolutePath; ///< 実行中のプログラムファイルパス
    LAppSprite* _back; ///< 背景画像
    LAppSprite* _fastForward; ///< 早送り画像
    LAppSprite* _gear; ///< ギア画像
    LAppSprite* _power; ///< 電源画像
    LAppMotionSyncModel* _userModel; ///< ユーザーが実際に使用するモデル

    Csm::csmVector<Csm::csmString> _modelNameList; ///< モデルディレクトリ名のリスト
    Csm::csmInt32 _modelIndex; ///< 表示するシーンモデル名のインデックス値
};

class EventHandler
{
public:
    /**
    * @brief   glfwSetMouseButtonCallback用コールバック関数。
    */
    static void OnMouseCallBack(GLFWwindow* window, int button, int action, int modify)
    {
        LAppDelegate::GetInstance()->OnMouseCallBack(window, button, action, modify);
    }

    /**
    * @brief   glfwSetCursorPosCallback用コールバック関数。
    */
    static void OnMouseCallBack(GLFWwindow* window, double x, double y)
    {
         LAppDelegate::GetInstance()->OnMouseCallBack(window, x, y);
    }

};
