/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppMotionSyncLinkDelegate.hpp"
#include <iostream>
#include <GLES2/gl2.h>
#include "LAppMotionSyncLinkView.hpp"
#include "LAppPal.hpp"
#include "LAppDefine.hpp"
#include "LAppTextureManager.hpp"
#include "JniBridgeC.hpp"

using namespace Csm;
using namespace std;
using namespace LAppDefine;

namespace {
    LAppMotionSyncLinkDelegate* s_instance = nullptr;
}

LAppMotionSyncLinkDelegate* LAppMotionSyncLinkDelegate::GetInstance()
{
    if (!s_instance)
    {
        s_instance = new LAppMotionSyncLinkDelegate();
    }

    return s_instance;
}

void LAppMotionSyncLinkDelegate::ReleaseInstance()
{
    if (s_instance)
    {
        delete s_instance;
    }

    s_instance = nullptr;
}


void LAppMotionSyncLinkDelegate::OnStart()
{
    _textureManager = new LAppTextureManager();
    _view = new LAppMotionSyncLinkView();
    LAppPal::UpdateTime();
}

void LAppMotionSyncLinkDelegate::OnPause()
{
}

void LAppMotionSyncLinkDelegate::OnStop()
{
    if (_view)
    {
        delete _view;
        _view = nullptr;
    }
    if (_textureManager)
    {
        delete _textureManager;
        _textureManager = nullptr;
    }

    CubismFramework::Dispose();
}

void LAppMotionSyncLinkDelegate::OnDestroy()
{
    ReleaseInstance();
}

void LAppMotionSyncLinkDelegate::Run()
{
    // 時間更新
    LAppPal::UpdateTime();

    // 画面の初期化
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepthf(1.0f);

    //描画更新
    if (_view)
    {
        _view->Render();
    }

    if (!_isActive)
    {
        JniBridgeC::MoveTaskToBack();
    }
}

void LAppMotionSyncLinkDelegate::OnSurfaceCreate()
{
    //テクスチャサンプリング設定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //透過設定
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Initialize cubism
    CubismFramework::Initialize();
}

void LAppMotionSyncLinkDelegate::OnSurfaceChanged(float width, float height)
{
    glViewport(0, 0, width, height);
    _width = width;
    _height = height;

    //AppViewの初期化
    _view->Initialize();
    _view->InitializeSprite();

    _isActive = true;
}

LAppMotionSyncLinkDelegate::LAppMotionSyncLinkDelegate():
        _cubismOption(),
        _textureManager(nullptr),
        _view(nullptr),
        _width(0),
        _height(0),
        _captured(false),
        _isActive(true),
        _mouseY(0.0f),
        _mouseX(0.0f)
{
    // Setup Cubism
    _cubismOption.LogFunction = LAppPal::PrintMessageLn;
    _cubismOption.LoggingLevel = LAppDefine::CubismLoggingLevel;
    _cubismOption.LoadFileFunction = LAppPal::LoadFileAsBytes;
    _cubismOption.ReleaseBytesFunction = LAppPal::ReleaseBytes;
    CubismFramework::CleanUp();
    CubismFramework::StartUp(&_cubismAllocator, &_cubismOption);
}

LAppMotionSyncLinkDelegate::~LAppMotionSyncLinkDelegate()
{
}

void LAppMotionSyncLinkDelegate::OnTouchBegan(double x, double y)
{
    _mouseX = static_cast<float>(x);
    _mouseY = static_cast<float>(y);

    if (_view)
    {
        _captured = true;
        _view->OnTouchesBegan(_mouseX, _mouseY);
    }
}

void LAppMotionSyncLinkDelegate::OnTouchEnded(double x, double y)
{
    _mouseX = static_cast<float>(x);
    _mouseY = static_cast<float>(y);

    if (_view)
    {
        _captured = false;
        _view->OnTouchesEnded(_mouseX, _mouseY);
    }
}

void LAppMotionSyncLinkDelegate::OnTouchMoved(double x, double y)
{
    _mouseX = static_cast<float>(x);
    _mouseY = static_cast<float>(y);

    if (_captured && _view)
    {
        _view->OnTouchesMoved(_mouseX, _mouseY);
    }
}

