/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#import <UIKit/UIKit.h>

@class MotionViewController;
@class LAppTextureManager;

@interface AppMotionSyncLinkDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) MotionViewController *viewController;
@property (nonatomic, readonly, getter=getTextureManager) LAppTextureManager *textureManager;

- (void)initializeCubism;
- (bool)getIsEnd;
- (void)finishApplication;

@end
