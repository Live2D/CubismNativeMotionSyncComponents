/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#import "AppMicrophoneDelegate.h"
#import "MicrophoneViewController.h"
#import "LAppAllocator.h"
#import <iostream>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import "LAppPal.h"
#import "LAppDefine.h"
#import "LAppTextureManager.h"


@interface AppMicrophoneDelegate ()

@property (nonatomic) LAppAllocator cubismAllocator; // Cubism SDK Allocator
@property (nonatomic) Csm::CubismFramework::Option cubismOption; // Cubism SDK Option
@property (nonatomic) bool captured; // クリックしているか
@property (nonatomic) float mouseX; // マウスX座標
@property (nonatomic) float mouseY; // マウスY座標
@property (nonatomic) bool isEnd; // APPを終了しているか
@property (nonatomic, readwrite) LAppTextureManager *textureManager; // テクスチャマネージャー

@end

@implementation AppMicrophoneDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    _textureManager = [[LAppTextureManager alloc]init];

    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    self.viewController = [[MicrophoneViewController alloc] initWithNibName:nil bundle:nil];
    self.window.rootViewController = self.viewController;
    [self.window makeKeyAndVisible];

    [self initializeCubism];

    [self.viewController initializeSprite];


    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application
{
}


- (void)applicationDidEnterBackground:(UIApplication *)application
{
    self.viewController.mOpenGLRun = false;

    _textureManager = nil;
}


- (void)applicationWillEnterForeground:(UIApplication *)application
{
    self.viewController.mOpenGLRun = true;

    _textureManager = [[LAppTextureManager alloc]init];
}


- (void)applicationDidBecomeActive:(UIApplication *)application
{

}


- (void)applicationWillTerminate:(UIApplication *)application {
    self.viewController = nil;
}

- (void)initializeCubism
{
    _cubismOption.LogFunction = LAppPal::PrintMessageLn;
    _cubismOption.LoggingLevel = LAppDefine::CubismLoggingLevel;

    Csm::CubismFramework::StartUp(&_cubismAllocator,&_cubismOption);

    Csm::CubismFramework::Initialize();

    [self.viewController initializeModel];
    
    LAppPal::UpdateTime();

}

- (bool)getIsEnd
{
    return _isEnd;
}

- (void)finishApplication
{
    [self.viewController releaseView];

    _textureManager = nil;

    Csm::CubismFramework::Dispose();

    self.window = nil;

    self.viewController = nil;

    _isEnd = true;

    exit(0);
}

@end
