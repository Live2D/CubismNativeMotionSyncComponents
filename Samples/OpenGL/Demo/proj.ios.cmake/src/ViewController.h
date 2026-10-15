/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#include <Type/csmVector.hpp>
#include <Type/csmString.hpp>
#include <Rendering/OpenGL/CubismRenderTarget_OpenGLES2.hpp>

@interface ViewController : GLKViewController <GLKViewDelegate>

#ifdef CSM_MOTIONSYNC_MOTION_DEMO
class LAppMotionSyncLinkModel;
#endif

@property (nonatomic, assign) bool mOpenGLRun;
@property (nonatomic) GLuint vertexBufferId;
@property (nonatomic) GLuint fragmentBufferId;
@property (nonatomic) GLuint programId;

/**
 * @brief 解放する。
 */
- (void)releaseView;

/**
 * @brief 画像の初期化を行う。
 */
- (void)initializeSprite;

/**
 * @brief モデルの初期化
 */
- (void)initializeModel;

/**
 * @brief モデル名リストの読み込み
 *
 * モデル名リストの読み込み処理を行う
 *
 */
- (void)LoadModelName;

/**
 * @brief モデルの読み込み
 *
 * モデルデータの読み込み処理を行う
 *
 */
- (void)LoadModel;

/**
 * @brief 次のモデルに切り替え
 *
 */
- (void)ChangeNextModel;

/**
 * @brief   ウインドウの幅を取得する。
 */
- (int)GetWindowWidth;

/**
 * @brief   ウインドウの高さを取得する。
 */
- (int)GetWindowHeight;

/**
 * @brief 画面初期化処理。
 *
 * viewDidLoad から1回だけ呼び出す。
 * UIWindowScene 経由でセーフエリアを取得するため、レイアウト確定前でも有効値が得られる。
 */
- (void)initializeScreen;

/**
 * @brief 画面リサイズ処理。
 *
 * viewDidLayoutSubviews から呼び出され、レイアウト確定後・回転・SplitView 等に追従する。
 */
- (void)resizeScreen;

#ifdef CSM_MOTIONSYNC_MOTION_DEMO
- (LAppMotionSyncLinkModel*)getCurrentModel;
#endif

@end
