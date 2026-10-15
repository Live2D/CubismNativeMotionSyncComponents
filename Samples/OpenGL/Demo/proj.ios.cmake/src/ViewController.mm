/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#import "ViewController.h"
#import <math.h>
#import <string>
#import <QuartzCore/QuartzCore.h>
#import <AVFoundation/AVFoundation.h>
#import "CubismFramework.hpp"
#import <Math/CubismMatrix44.hpp>
#import <Math/CubismViewMatrix.hpp>
#import <GLKit/GLKit.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import "SceneDelegate.h"
#import "LAppSprite.h"
#import "LAppDefine.h"
#import "LAppMotionSyncDefine.h"
#import "LAppTextureManager.h"
#import "LAppPal.h"
#ifdef CSM_MOTIONSYNC_MOTION_DEMO
#import "LAppMotionSyncLinkModel.h"
#else
#import "LAppMotionSyncModel.h"
#endif

#define BUFFER_OFFSET(bytes) ((GLubyte *)NULL + (bytes))

using namespace std;
using namespace LAppDefine;
using namespace LAppMotionSyncDefine;

@interface ViewController ()
{
    int _windowWidth;  ///< ウィンドウサイズ幅の保存
    int _windowHeight; ///< ウィンドウサイズ高さの保存
}
@property (nonatomic) LAppSprite *back; //背景画像
@property (nonatomic) LAppSprite *fastForward; //早送り画像
@property (nonatomic) LAppSprite *gear; //歯車画像
@property (nonatomic) LAppSprite *power; //電源画像

#ifdef CSM_MOTIONSYNC_MOTION_DEMO
@property (nonatomic) LAppMotionSyncLinkModel *userModel; ///< ユーザーが実際に使用するモデル
#else
@property (nonatomic) LAppMotionSyncModel *userModel; ///< ユーザーが実際に使用するモデル
#endif
@property (nonatomic) Csm::csmVector<Csm::csmString> modelNameList; ///< モデルディレクトリ名のリスト
@property (nonatomic) Csm::csmInt32 modelIndex; ///< 表示するシーンモデル名のインデックス値
@end

@implementation ViewController
@synthesize mOpenGLRun;

int CompareCsmString(const void* a, const void* b)
{
    return strcmp(reinterpret_cast<const Csm::csmString*>(a)->GetRawString(),
        reinterpret_cast<const Csm::csmString*>(b)->GetRawString());
}

- (void)releaseView
{
    _back = nil;
    _fastForward = nil;
    _gear = nil;
    _power = nil;

    if (_userModel)
    {
        _userModel->DeleteRenderer();
        delete(_userModel);
        _userModel = nil;
    }

    GLKView *view = (GLKView*)self.view;

    view = nil;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    mOpenGLRun = true;

    [self initializeScreen];

    GLKView *view = (GLKView*)self.view;

    // GL描画周期を60FPSに設定
    self.preferredFramesPerSecond = 60;

    // OpenGL ES2を指定
    view.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    // set context
    [EAGLContext setCurrentContext:view.context];

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glGenBuffers(1, &_vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);

    glGenBuffers(1, &_fragmentBufferId);
    glBindBuffer(GL_ARRAY_BUFFER,  _fragmentBufferId);
}

- (void)viewDidLayoutSubviews
{
    [super viewDidLayoutSubviews];
    [self resizeScreen];
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    //時間更新
    LAppPal::UpdateTime();

    if (mOpenGLRun)
    {
        // 画面クリア
        glClear(GL_COLOR_BUFFER_BIT);

        // セーフエリアのみ
        UIEdgeInsets insets = self.view.safeAreaInsets;
        CGFloat scale = self.traitCollection.displayScale;
        GLint vpX = (GLint)(insets.left * scale);
        GLint vpY = (GLint)(insets.bottom * scale);
        GLsizei vpW = (GLsizei)((self.view.frame.size.width - insets.left - insets.right) * scale);
        GLsizei vpH = (GLsizei)((self.view.frame.size.height - insets.top - insets.bottom) * scale);
        glViewport(vpX, vpY, vpW, vpH);

        [_back render:_vertexBufferId fragmentBufferID:_fragmentBufferId];

        [_fastForward render:_vertexBufferId fragmentBufferID:_fragmentBufferId];

        [_gear render:_vertexBufferId fragmentBufferID:_fragmentBufferId];

        [_power render:_vertexBufferId fragmentBufferID:_fragmentBufferId];

        // モデルの描画
        _userModel->Update();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

- (void)initializeSprite
{
    UIWindowScene *windowScene = (UIWindowScene *)[UIApplication sharedApplication].connectedScenes.anyObject;
    if (!windowScene)
    {
        return;
    }
    UIEdgeInsets insets = windowScene.windows.firstObject.safeAreaInsets;
    int width = windowScene.screen.bounds.size.width - insets.left - insets.right;
    int height = windowScene.screen.bounds.size.height - insets.top - insets.bottom;

    SceneDelegate *delegate = [SceneDelegate sharedSceneDelegate];
    LAppTextureManager* textureManager = [delegate getTextureManager];
    const string resourcesPath = ResourcesPath;

    string imageName = BackImageName;
    TextureInfo* backgroundTexture = [textureManager createTextureFromPngFile:resourcesPath+imageName];
    float x = width * 0.5f;
    float y = height * 0.5f;
    float fHeight = static_cast<float>(height * 0.95f);
    float ratio = fHeight / static_cast<float>(backgroundTexture->height);
    float fWidth = backgroundTexture->width * ratio;

    _back = [[LAppSprite alloc] initWithMyVar:x Y:y Width:fWidth Height:fHeight MaxWidth:width MaxHeight:height TextureId:backgroundTexture->id];

    imageName = FastForwardImageName;
    TextureInfo* fastForwardTexture = [textureManager createTextureFromPngFile:resourcesPath+imageName];
    x = static_cast<float>(fastForwardTexture->width * 0.5f);
    y = static_cast<float>(height - fastForwardTexture->height);
    fWidth = static_cast<float>(fastForwardTexture->width);
    fHeight = static_cast<float>(fastForwardTexture->height);
    _fastForward = [[LAppSprite alloc] initWithMyVar:x Y:y Width:fWidth Height:fHeight MaxWidth:width MaxHeight:height TextureId:fastForwardTexture->id];

    imageName = GearImageName;
    TextureInfo* gearTexture = [textureManager createTextureFromPngFile:resourcesPath+imageName];
    x = static_cast<float>(width - gearTexture->width * 0.5f);
    y = static_cast<float>(height - gearTexture->height);
    fWidth = static_cast<float>(gearTexture->width);
    fHeight = static_cast<float>(gearTexture->height);
    _gear = [[LAppSprite alloc] initWithMyVar:x Y:y Width:fWidth Height:fHeight MaxWidth:width MaxHeight:height TextureId:gearTexture->id];

    imageName = PowerImageName;
    TextureInfo* powerTexture = [textureManager createTextureFromPngFile:resourcesPath+imageName];
    x = static_cast<float>(width - powerTexture->width * 0.5f);
    y = static_cast<float>(powerTexture->height * 0.5f);
    fWidth = static_cast<float>(powerTexture->width);
    fHeight = static_cast<float>(powerTexture->height);
    _power = [[LAppSprite alloc] initWithMyVar:x Y:y Width:fWidth Height:fHeight MaxWidth:width MaxHeight:height TextureId:powerTexture->id];
}

- (void)initializeModel
{
    // モデル関係
    [self LoadModelName];
    _modelIndex = 0;
    [self LoadModel];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    NSLog(@"%@", touch.view);

    CGPoint point = [touch locationInView:self.view];
    UIEdgeInsets insets = self.view.safeAreaInsets;
    point.x -= insets.left;
    point.y -= insets.top;
    float pointY = [self transformTapY:point.y];

    // 音声の切り替え
    if ([_fastForward isHit:point.x PointY:pointY])
    {
#ifdef CSM_MOTIONSYNC_MOTION_DEMO
        _userModel->ChangeNextMotion();
#else
        _userModel->ChangeNextIndexSound();
#endif
    }

    // 歯車にタップしたか
    if ([_gear isHit:point.x PointY:pointY])
    {
        [self ChangeNextModel];
    }

    // 電源ボタンにタップしたか
    if ([_power isHit:point.x PointY:pointY])
    {
        SceneDelegate *delegate = [SceneDelegate sharedSceneDelegate];
        [delegate finishApplication];
    }
}

- (float)transformTapY:(float)deviceY
{
    UIEdgeInsets insets = self.view.safeAreaInsets;
    float height = self.view.frame.size.height - insets.top - insets.bottom;
    return deviceY * -1 + height;
}

- (void)LoadModelName
{
    _modelNameList.Clear();

    NSBundle* bundle = [NSBundle mainBundle];
    NSString* resPath = [NSString stringWithUTF8String:LAppDefine::ResourcesPath];
    NSArray* resArr = [bundle pathsForResourcesOfType:NULL inDirectory:resPath];
    NSUInteger cnt = [resArr count];

    for (NSUInteger i = 0; i < cnt; i++)
    {
        NSString* modelName = [[resArr objectAtIndex:i] lastPathComponent];
        NSMutableString* modelDirPath = [NSMutableString stringWithString:resPath];
        [modelDirPath appendString:@"/"];
        [modelDirPath appendString:modelName];
        NSArray* model3json = [bundle pathsForResourcesOfType:@".model3.json" inDirectory:modelDirPath];
        if ([model3json count] == 1)
        {
            _modelNameList.PushBack(Csm::csmString([modelName UTF8String]));
        }
    }
    qsort(_modelNameList.GetPtr(), _modelNameList.GetSize(), sizeof(Csm::csmString), CompareCsmString);
}

- (void)LoadModel
{
    if (_userModel)
    {
        // レンダラの解放
        _userModel->DeleteRenderer();

        delete(_userModel);
    }

    // モデルデータの新規生成
#ifdef CSM_MOTIONSYNC_MOTION_DEMO
    _userModel = new LAppMotionSyncLinkModel();
#else
    _userModel = new LAppMotionSyncModel();
#endif

    if (_modelIndex < _modelNameList.GetSize())
    {
        // モデルデータの読み込み及び生成とセットアップを行う
        _userModel->LoadAssets(_modelNameList[_modelIndex]);
    }
}

- (void)ChangeNextModel
{
    if (0 < _modelNameList.GetSize())
    {
        _modelIndex = (_modelIndex + 1) % _modelNameList.GetSize();
        [self LoadModel];
    }
}

- (int)GetWindowWidth
{
    return _windowWidth;
}

- (int)GetWindowHeight
{
    return _windowHeight;
}

- (void)initializeScreen
{
    // viewDidLoad 時点では self.view.safeAreaInsets が常にゼロを返すため、
    // UIWindowScene 経由でセーフエリアを取得し、初回サイズを算出する。
    UIWindowScene *windowScene =
        (UIWindowScene *)[UIApplication sharedApplication].connectedScenes.anyObject;
    if (!windowScene)
    {
        return;
    }

    UIEdgeInsets insets = windowScene.windows.firstObject.safeAreaInsets;
    CGSize size = windowScene.screen.bounds.size;
    if (size.width <= 0 || size.height <= 0)
    {
        return;
    }

    int width = size.width - insets.left - insets.right;
    int height = size.height - insets.top - insets.bottom;

    const CGFloat retinaScale = self.traitCollection.displayScale;
    _windowWidth = width * retinaScale;
    _windowHeight = height * retinaScale;
}

- (void)resizeScreen
{
    UIEdgeInsets insets = self.view.safeAreaInsets;
    int width = self.view.frame.size.width - insets.left - insets.right;
    int height = self.view.frame.size.height - insets.top - insets.bottom;

    if (width == 0 || height == 0)
    {
        return;
    }

    const CGFloat retinaScale = self.traitCollection.displayScale;
    int newWindowWidth = width * retinaScale;
    int newWindowHeight = height * retinaScale;
    if (newWindowWidth == _windowWidth && newWindowHeight == _windowHeight)
    {
        return;
    }
    _windowWidth = newWindowWidth;
    _windowHeight = newWindowHeight;

    if (_userModel)
    {
        _userModel->SetRenderTargetSize(newWindowWidth, newWindowHeight);
    }

    [self resizeSprite:width height:height];
}

- (void)resizeSprite:(float)width height:(float)height
{
    UIEdgeInsets insets = self.view.safeAreaInsets;
    float maxWidth = self.view.frame.size.width - insets.left - insets.right;
    float maxHeight = self.view.frame.size.height - insets.top - insets.bottom;

    SceneDelegate *delegate = [SceneDelegate sharedSceneDelegate];
    LAppTextureManager* textureManager = [delegate getTextureManager];

    const string resourcesPath = ResourcesPath;

    float x;
    float y;
    float fHeight;
    float ratio;
    float fWidth;

    if (_back)
    {
        TextureInfo* backgroundTexture = [textureManager createTextureFromPngFile:resourcesPath+BackImageName];
        x = width * 0.5f;
        y = height * 0.5f;
        fHeight = static_cast<float>(height * 0.95f);
        ratio = fHeight / static_cast<float>(backgroundTexture->height);
        fWidth = static_cast<float>(backgroundTexture->width * ratio);
        [_back resizeImmidiate:x Y:y Width:fWidth Height:fHeight MaxWidth:maxWidth MaxHeight:maxHeight];
    }

    if (_fastForward)
    {
        TextureInfo* fastForwardTexture = [textureManager createTextureFromPngFile:resourcesPath+FastForwardImageName];
        fWidth = static_cast<float>(fastForwardTexture->width);
        fHeight = static_cast<float>(fastForwardTexture->height);
        x = static_cast<float>(fWidth * 0.5f);
        y = static_cast<float>(height - fHeight);
        [_fastForward resizeImmidiate:x Y:y Width:fWidth Height:fHeight MaxWidth:maxWidth MaxHeight:maxHeight];
    }

    if (_gear)
    {
        TextureInfo* gearTexture = [textureManager createTextureFromPngFile:resourcesPath+GearImageName];
        int gearWidth = gearTexture->width;
        int gearHeight = gearTexture->height;
        x = static_cast<float>(width - gearWidth * 0.5f);
        y = static_cast<float>(height - gearHeight);
        fHeight = static_cast<float>(gearHeight);
        fWidth = static_cast<float>(gearWidth);
        [_gear resizeImmidiate:x Y:y Width:fWidth Height:fHeight MaxWidth:maxWidth MaxHeight:maxHeight];
    }

    if (_power)
    {
        TextureInfo* powerTexture = [textureManager createTextureFromPngFile:resourcesPath+PowerImageName];
        int powerWidth = powerTexture->width;
        int powerHeight = powerTexture->height;
        x = static_cast<float>(width - powerWidth * 0.5f);
        y = static_cast<float>(powerHeight * 0.5f);
        fWidth = static_cast<float>(powerWidth);
        fHeight = static_cast<float>(powerHeight);
        [_power resizeImmidiate:x Y:y Width:fWidth Height:fHeight MaxWidth:maxWidth MaxHeight:maxHeight];
    }
}

#ifdef CSM_MOTIONSYNC_MOTION_DEMO
- (LAppMotionSyncLinkModel*)getCurrentModel
{
    return _userModel;
}
#endif

@end
