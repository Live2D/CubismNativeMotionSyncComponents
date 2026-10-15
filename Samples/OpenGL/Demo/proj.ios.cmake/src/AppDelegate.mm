/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#import "AppDelegate.h"
#import "SceneDelegate.h"
#import "ViewController.h"
#import "LAppAllocator.h"
#import <iostream>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import "LAppPal.h"
#import "LAppDefine.h"


@interface AppDelegate ()

@property (nonatomic) LAppAllocator cubismAllocator; // Cubism SDK Allocator
@property (nonatomic) Csm::CubismFramework::Option cubismOption; // Cubism SDK Option
@property (nonatomic) bool isEnd; // APPを終了しているか

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    return YES;
}

- (UISceneConfiguration *)application:(UIApplication *)application
    configurationForConnectingSceneSession:(UISceneSession *)connectingSceneSession
    options:(UISceneConnectionOptions *)options API_AVAILABLE(ios(13.0))
{
    UISceneConfiguration *configuration = [[UISceneConfiguration alloc] initWithName:@"Default Configuration"
                                                                        sessionRole:connectingSceneSession.role];
    configuration.delegateClass = NSClassFromString(@"SceneDelegate");
    return configuration;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{

}

- (void)applicationWillTerminate:(UIApplication *)application {
    [self releaseApplicationResources];
}

- (void)initializeCubism
{
    _cubismOption.LogFunction = LAppPal::PrintMessageLn;
    _cubismOption.LoggingLevel = LAppDefine::CubismLoggingLevel;
    _cubismOption.LoadFileFunction = LAppPal::LoadFileAsBytes;
    _cubismOption.ReleaseBytesFunction = LAppPal::ReleaseBytes;

    Csm::CubismFramework::StartUp(&_cubismAllocator,&_cubismOption);

    Csm::CubismFramework::Initialize();

    LAppPal::UpdateTime();
}

- (bool)getIsEnd
{
    return _isEnd;
}

- (void)finishApplication
{
    [self releaseApplicationResources];

    exit(0);
}

- (void)releaseApplicationResources
{
    if (_isEnd)
    {
        return;
    }

    _isEnd = true;

    for (UIScene *scene in [UIApplication sharedApplication].connectedScenes)
    {
        if ([scene.delegate isKindOfClass:[SceneDelegate class]])
        {
            SceneDelegate *sceneDelegate = (SceneDelegate *)scene.delegate;
            [sceneDelegate releaseSceneResources];
        }
    }

    if (Csm::CubismFramework::IsInitialized())
    {
        Csm::CubismFramework::Dispose();
    }
}

@end
