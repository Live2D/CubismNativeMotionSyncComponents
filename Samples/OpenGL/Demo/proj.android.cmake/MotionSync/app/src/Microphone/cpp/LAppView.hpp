/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <Math/CubismMatrix44.hpp>
#include <Math/CubismViewMatrix.hpp>
#include "CubismFramework.hpp"
#include <Rendering/OpenGL/CubismOffscreenSurface_OpenGLES2.hpp>

class LAppSprite;
class LAppMotionSyncModel;

/**
* @brief 描画クラス
*/
class LAppView
{
public:
    /**
    * @brief コンストラクタ
    */
    LAppView();

    /**
    * @brief デストラクタ
    */
    ~LAppView();

    /**
    * @brief 初期化する。
    */
    void Initialize();

    /**
    * @brief 描画する。
    */
    void Render();

    /**
    * @brief シェーダーの初期化を行う。
    */
    void InitializeShader();

    /**
    * @brief 画像の初期化を行う。
    */
    void InitializeSprite();

    /**
    * @brief タッチされたときに呼ばれる。
    *
    * @param[in]       pointX            スクリーンX座標
    * @param[in]       pointY            スクリーンY座標
    */
    void OnTouchesBegan(float pointX, float pointY) const;

    /**
    * @brief タッチしているときにポインタが動いたら呼ばれる。
    *
    * @param[in]       pointX            スクリーンX座標
    * @param[in]       pointY            スクリーンY座標
    */
    void OnTouchesMoved(float pointX, float pointY) const;

    /**
    * @brief タッチが終了したら呼ばれる。
    *
    * @param[in]       pointX            スクリーンX座標
    * @param[in]       pointY            スクリーンY座標
    */
    void OnTouchesEnded(float pointX, float pointY);

private:
    GLuint _programId; ///< シェーダID

    LAppSprite* _back; ///< 背景画像
    LAppSprite* _gear; ///< ギア画像
    LAppSprite* _power; ///< 電源画像
    LAppMotionSyncModel* _userModel; ///< ユーザーが実際に使用するモデル

    Csm::csmVector<Csm::csmString> _modelNameList; ///< モデルディレクトリ名のリスト
    Csm::csmInt32 _modelIndex; ///< 表示するシーンモデル名のインデックス値
    Csm::csmBool _changeModel; ///< モデル切り替えフラグ

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
};
