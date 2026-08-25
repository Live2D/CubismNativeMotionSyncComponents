/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#import "MotionViewController.h"
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
#import "AppMotionSyncLinkDelegate.h"
#import "LAppSprite.h"
#import "LAppDefine.h"
#import "LAppMotionSyncDefine.h"
#import "LAppTextureManager.h"
#import "LAppPal.h"
#import "LAppMotionSyncLinkModel.h"

#define BUFFER_OFFSET(bytes) ((GLubyte *)NULL + (bytes))

using namespace std;
using namespace LAppDefine;
using namespace LAppMotionSyncDefine;

@interface MotionViewController ()
@property (nonatomic) LAppSprite *back;
@property (nonatomic) LAppSprite *fastForward;
@property (nonatomic) LAppSprite *gear;
@property (nonatomic) LAppSprite *power;
@property (nonatomic) LAppMotionSyncLinkModel *userModel;
@property (nonatomic) Csm::csmVector<Csm::csmString> modelNameList;
@property (nonatomic) Csm::csmInt32 modelIndex;
@end

@implementation MotionViewController
@synthesize mOpenGLRun;

int CompareMotionCsmString(const void* a, const void* b)
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

    GLKView *view = (GLKView*)self.view;

    self.preferredFramesPerSecond = 60;
    view.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    [EAGLContext setCurrentContext:view.context];

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenBuffers(1, &_vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId);

    glGenBuffers(1, &_fragmentBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, _fragmentBufferId);
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    LAppPal::UpdateTime();

    if (mOpenGLRun)
    {
        glClear(GL_COLOR_BUFFER_BIT);

        [_back render:_vertexBufferId fragmentBufferID:_fragmentBufferId];
        [_fastForward render:_vertexBufferId fragmentBufferID:_fragmentBufferId];
        [_gear render:_vertexBufferId fragmentBufferID:_fragmentBufferId];
        [_power render:_vertexBufferId fragmentBufferID:_fragmentBufferId];

        _userModel->Update();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    }
}

- (void)initializeSprite
{
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    int width = screenRect.size.width;
    int height = screenRect.size.height;

    AppMotionSyncLinkDelegate *delegate = (AppMotionSyncLinkDelegate *)[[UIApplication sharedApplication] delegate];
    LAppTextureManager* textureManager = [delegate getTextureManager];
    const string resourcesPath = ResourcesPath;

    string imageName = BackImageName;
    TextureInfo* backgroundTexture = [textureManager createTextureFromPngFile:resourcesPath+imageName];
    float x = width * 0.5f;
    float y = height * 0.5f;
    float fWidth = static_cast<float>(width * 0.75f);
    float fHeight = static_cast<float>(height * 0.95f);
    _back = [[LAppSprite alloc] initWithMyVar:x Y:y Width:fWidth Height:fHeight TextureId:backgroundTexture->id];

    imageName = FastForwardImageName;
    TextureInfo* fastForwardTexture = [textureManager createTextureFromPngFile:resourcesPath+imageName];
    x = static_cast<float>(fastForwardTexture->width * 0.5f);
    y = static_cast<float>(height - fastForwardTexture->height * 0.5f);
    fWidth = static_cast<float>(fastForwardTexture->width);
    fHeight = static_cast<float>(fastForwardTexture->height);
    _fastForward = [[LAppSprite alloc] initWithMyVar:x Y:y Width:fWidth Height:fHeight TextureId:fastForwardTexture->id];

    imageName = GearImageName;
    TextureInfo* gearTexture = [textureManager createTextureFromPngFile:resourcesPath+imageName];
    x = static_cast<float>(width - gearTexture->width * 0.5f);
    y = static_cast<float>(height - gearTexture->height * 0.5f);
    fWidth = static_cast<float>(gearTexture->width);
    fHeight = static_cast<float>(gearTexture->height);
    _gear = [[LAppSprite alloc] initWithMyVar:x Y:y Width:fWidth Height:fHeight TextureId:gearTexture->id];

    imageName = PowerImageName;
    TextureInfo* powerTexture = [textureManager createTextureFromPngFile:resourcesPath+imageName];
    x = static_cast<float>(width - powerTexture->width * 0.5f);
    y = static_cast<float>(powerTexture->height * 0.5f);
    fWidth = static_cast<float>(powerTexture->width);
    fHeight = static_cast<float>(powerTexture->height);
    _power = [[LAppSprite alloc] initWithMyVar:x Y:y Width:fWidth Height:fHeight TextureId:powerTexture->id];
}

- (void)initializeModel
{
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
    CGPoint point = [touch locationInView:self.view];
    float pointY = [self transformTapY:point.y];

    if ([_fastForward isHit:point.x PointY:pointY])
    {
        _userModel->ChangeNextMotion();
    }

    if ([_gear isHit:point.x PointY:pointY])
    {
        [self ChangeNextModel];
    }

    if ([_power isHit:point.x PointY:pointY])
    {
        AppMotionSyncLinkDelegate *delegate = (AppMotionSyncLinkDelegate *)[[UIApplication sharedApplication] delegate];
        [delegate finishApplication];
    }
}

- (float)transformTapY:(float)deviceY
{
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    int height = screenRect.size.height;
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
    qsort(_modelNameList.GetPtr(), _modelNameList.GetSize(), sizeof(Csm::csmString), CompareMotionCsmString);
}

- (void)LoadModel
{
    if (_userModel)
    {
        _userModel->DeleteRenderer();
        delete(_userModel);
    }

    _userModel = new LAppMotionSyncLinkModel();

    if (_modelIndex < _modelNameList.GetSize())
    {
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

- (LAppMotionSyncLinkModel*)getCurrentModel
{
    return _userModel;
}

@end
