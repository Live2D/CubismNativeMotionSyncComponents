/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#pragma once

#include <CubismFramework.hpp>
#include <Model/CubismUserModel.hpp>
#include <Motion/CubismMotion.hpp>
#include "CubismMotionSync.hpp"
#include "CubismModelMotionSyncSettingJson.hpp"
#include "LAppAudioManager.hpp"

/**
  * @brief ユーザーが実際に使用するモデルの実装クラス<br>
  *         モデル生成、機能コンポーネント生成、更新処理とレンダリングの呼び出しを行う。
  *
  */
class LAppMotionSyncLinkModel : public Csm::CubismUserModel
{
public:
    /**
     * @brief コンストラクタ
     */
    LAppMotionSyncLinkModel();

    /**
     * @brief デストラクタ
     *
     */
    virtual ~LAppMotionSyncLinkModel();

    /**
     * @brief model3.jsonが置かれたディレクトリとファイルパスからモデルを生成する
     *
     */
    void LoadAssets(const Csm::csmString fileName);

    /**
     * @brief モデルの更新
     *
     * モデルの更新処理。モデルのパラメータから描画状態を決定する
     */
    void Update();

    /**
     * @brief 次の音声ファイルを再生する
     *
     */
    void ChangeNextMotion();

    /**
     * @brif 引数で指定したモーションの再生を開始する
     *
     * param[in] group                      モーショングループ名
     * param[in] no                         グループ内の番号
     * param[in] priority                   優先度
     * param[in] onFinishedMotionHandler    モーション再生終了時に呼び出されるコールバック関数。NULLの場合呼び出されない。
     * param[in] onBeganMotionHandler       モーション再生開始時に呼び出されるコールバック関数。NULLの場合呼び出されない。
     * @return                              開始したモーションの識別番号を返す。個別のモーションが終了したか否かを判定するIsFinished()の引数で使用する。開始できない場合は「-1」
     */
    Csm::CubismMotionQueueEntryHandle StartMotion(const Csm::csmChar* group, Csm::csmInt32 no, Csm::csmInt32 priority, Csm::ACubismMotion::FinishedMotionCallback onFinishedMotionHandler = NULL, Csm::ACubismMotion::BeganMotionCallback onBeganMotionHandler = NULL);

    /**
     * @brief モーション終了時のコールバック関数
     */
    void OnFinishedMotion(Csm::ACubismMotion* self);

private:
    Csm::MotionSync::CubismModelMotionSyncSettingJson* _modelSetting; ///< モデルセッティング情報
    Csm::csmString _modelHomeDir; ///< モデルセッティングが置かれたディレクトリ
    Csm::csmFloat32 _userTimeSeconds; ///< デルタ時間の積算値[秒]
    Csm::MotionSync::CubismMotionSync* _motionSync; ///< モーションシンク
    LAppAudioManager _soundData; ///< モーションシンクで使用する音声データ
    Csm::csmMap<Csm::csmString, Csm::csmString> _motionSyncSettingMap; ///< モーションシンク設定のリスト
    Csm::csmMap<Csm::csmString, Csm::csmString> _soundFileMap; ///< 音声データファイルのリスト
    Csm::csmInt32 _motionIndex; ///< 再生するモーションのインデックス値

    Csm::csmBool _isMotionSync; ///< モーションシンクを利用するモデルかどうか
    Csm::csmMap<Csm::csmString, Csm::ACubismMotion*> _motions; ///< 読み込まれているモーションのリスト
    Csm::csmInt32 _currentMotionSyncSettingIndex; ///< 現在再生中のモーションシンク設定のインデックス
    Csm::csmString _currentSoundFileName; ///< 現在再生中のサウンドファイルの名称
    Csm::csmBool _isEnableMotionSync; ///< モーションシンクを行うかどうか
    Csm::ACubismMotion* _currentMotionSyncMotion; ///< 現在のモーションシンクを再生しているモーション

    /**
     * @brief モーション開始時に呼び出す
    */
    void StartMotionSync();

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
     * @brief モーションデータをグループ名から一括でロードする
     *        モーションデータの名前は内部でModelSettingから取得する
     *
     * @param[in] group モーションデータのグループ名
     */
    void PreloadMotionGroup(const Csm::csmChar* group);

    /**
     * @brief すべてのモーションデータの解放
     *
     * すべてのモーションデータを解放する
     */
    void ReleaseMotions();

    /**
     * @brief モデルを描画する処理。モデルを描画する空間のView-Projection行列を渡す
     *
     * @param[in]  matrix  View-Projection行列
     */
    void Draw(Csm::CubismMatrix44& matrix);
};
