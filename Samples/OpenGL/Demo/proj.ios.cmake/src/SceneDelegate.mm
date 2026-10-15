/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#import "SceneDelegate.h"
#import "AppDelegate.h"
#import "ViewController.h"
#import "LAppTextureManager.h"


@interface SceneDelegate ()

@property (nonatomic, readwrite) LAppTextureManager *textureManager; // テクスチャマネージャー

@end

static SceneDelegate *s_sharedSceneDelegate = nil;

@implementation SceneDelegate

+ (SceneDelegate *)sharedSceneDelegate
{
    if (s_sharedSceneDelegate)
    {
        return s_sharedSceneDelegate;
    }

    for (UIScene *scene in [UIApplication sharedApplication].connectedScenes)
    {
        if ([scene.delegate isKindOfClass:[SceneDelegate class]])
        {
            return (SceneDelegate *)scene.delegate;
        }
    }
    return nil;
}

- (void)scene:(UIScene *)scene
    willConnectToSession:(UISceneSession *)session
    options:(UISceneConnectionOptions *)connectionOptions
{
    if (![scene isKindOfClass:[UIWindowScene class]])
    {
        return;
    }

    s_sharedSceneDelegate = self;

    UIWindow *window = [[UIWindow alloc] initWithWindowScene:(UIWindowScene *)scene];
    ViewController *viewController = [[ViewController alloc] initWithNibName:nil bundle:nil];

    self.window = window;
    self.viewController = viewController;
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];

    [self.viewController initializeScreen];
    [self getTextureManager];

    AppDelegate *appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    [appDelegate initializeCubism];
    [self.viewController initializeModel];
    [self.viewController initializeSprite];
}

- (void)sceneDidDisconnect:(UIScene *)scene
{
   [self releaseSceneResources];
}

- (void)sceneDidEnterBackground:(UIScene *)scene
{
    self.viewController.mOpenGLRun = false;
}

- (void)sceneWillEnterForeground:(UIScene *)scene
{
    self.viewController.mOpenGLRun = true;
}

- (LAppTextureManager *)getTextureManager
{
    if (!_textureManager)
    {
        _textureManager = [[LAppTextureManager alloc]init];
    }

    return _textureManager;
}

- (void)releaseSceneResources
{
    ViewController *viewController = self.viewController;

    [viewController releaseView];

    _textureManager = nil;
    self.window = nil;
    self.viewController = nil;

    GLKView *view = viewController.isViewLoaded ? (GLKView *)viewController.view : nil;
    EAGLContext *context = view.context;
    if (context)
    {
        [EAGLContext setCurrentContext:context];
    }

    if ([EAGLContext currentContext] == context)
    {
        [EAGLContext setCurrentContext:nil];
    }

    if (s_sharedSceneDelegate == self)
    {
        s_sharedSceneDelegate = nil;
    }
}

- (void)finishApplication
{
    AppDelegate *appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    [appDelegate finishApplication];
}

@end
