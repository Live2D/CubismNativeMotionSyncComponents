/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#import <CubismFramework.hpp>
#import <Model/CubismUserModel.hpp>
#import "CubismMotionSync.hpp"
#import "CubismModelMotionSyncSettingJson.hpp"
#import "LAppAudioManager.h"

/**
  * @brief ユーザーが実際に使用するモデルの実装クラス<br>
  *         モデル生成、機能コンポーネント生成、更新処理とレンダリングの呼び出しを行う。
  *
  */
class LAppMotionSyncModel : public Csm::CubismUserModel
{
public:
    /**
     * @brief コンストラクタ
     */
    LAppMotionSyncModel();

    /**
     * @brief デストラクタ
     *
     */
    virtual ~LAppMotionSyncModel();

    /**
     * @brief model3.jsonが置かれたディレクトリとファイルパスからモデルを生成する
     *
     */
    void LoadAssets(const Csm::csmString fileName);

    /**
     * @brief モデルの更新
     *
     * モデルの状態や描画を更新する
     */
    void Update();

    /**
     * @brief 次の音声ファイルを再生する
     *
     */
    void ChangeNextIndexSound();

private:
    Csm::MotionSync::CubismModelMotionSyncSettingJson* _modelSetting; ///< モデルセッティング情報
    Csm::csmString _modelHomeDir; ///< モデルセッティングが置かれたディレクトリ
    Csm::csmFloat32 _userTimeSeconds; ///< デルタ時間の積算値[秒]
    Csm::MotionSync::CubismMotionSync* _motionSync; ///< モーションシンク
    LAppAudioManager _soundData; ///< モーションシンクで使用する音声データ
    Csm::csmVector<Csm::csmString> _soundFileList; ///< 音声データファイルのリスト
    Csm::csmInt32 _soundIndex; ///< 再生する音声データのインデックス値

    /**
     * @brief model3.jsonからモデルを生成する。<br>
     *         model3.jsonの記述に従ってモデル生成、モーション、物理演算などのコンポーネント生成を行う。
     *
     */
    void SetupModel();

    /**
     * @brief OpenGLのテクスチャユニットにテクスチャをロードする
     *
     */
    void SetupTextures();

    /**
     * @brief モデルのパラメータ情報の更新
     *
     * モデルのパラメータの情報を更新する
     */
    void UpdateModelParam();

    /**
     * @brief 現在のインデックスの音声ファイルを再生する
     *
     */
    void PlayIndexSound();

    /**
     * @brief モデルを描画する処理。モデルを描画する空間のView-Projection行列を渡す
     *
     * @param[in]  matrix  View-Projection行列
     */
    void Draw(Csm::CubismMatrix44& matrix);
};
