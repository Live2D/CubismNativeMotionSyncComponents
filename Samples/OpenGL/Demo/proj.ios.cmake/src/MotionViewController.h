/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

#include <Rendering/OpenGL/CubismOffscreenSurface_OpenGLES2.hpp>

class LAppMotionSyncLinkModel;

@interface MotionViewController : GLKViewController <GLKViewDelegate>

@property (nonatomic, assign) bool mOpenGLRun;
@property (nonatomic) GLuint vertexBufferId;
@property (nonatomic) GLuint fragmentBufferId;
@property (nonatomic) GLuint programId;

- (void)releaseView;
- (void)initializeSprite;
- (void)initializeModel;
- (void)LoadModelName;
- (void)LoadModel;
- (void)ChangeNextModel;
- (LAppMotionSyncLinkModel*)getCurrentModel;

@end
