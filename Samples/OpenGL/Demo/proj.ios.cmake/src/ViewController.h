/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#include <Rendering/OpenGL/CubismOffscreenSurface_OpenGLES2.hpp>

@interface ViewController : GLKViewController <GLKViewDelegate>

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

@end
