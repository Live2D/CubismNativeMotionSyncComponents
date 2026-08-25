/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppMotionSyncLinkView.hpp"
#include <cmath>
#include <string>
#include "LAppPal.hpp"
#include "LAppMotionSyncLinkDelegate.hpp"
#include "LAppDefine.hpp"
#include "LAppTextureManager.hpp"
#include "TouchManager_Common.hpp"
#include "LAppSprite.hpp"
#include "LAppMotionSyncLinkModel.hpp"
#include "LAppMotionSyncDefine.hpp"

#include <Rendering/OpenGL/CubismOffscreenSurface_OpenGLES2.hpp>
#include <Rendering/OpenGL/CubismRenderer_OpenGLES2.hpp>

#include "JniBridgeC.hpp"

using namespace std;
using namespace LAppDefine;
using namespace LAppMotionSyncDefine;
using namespace Csm;

namespace {
    int CompareCsmString(const void* a, const void* b)
    {
        return strcmp(reinterpret_cast<const Csm::csmString*>(a)->GetRawString(),
                      reinterpret_cast<const Csm::csmString*>(b)->GetRawString());
    }
}

LAppMotionSyncLinkView::LAppMotionSyncLinkView():
        _back(NULL),
        _fastForward(NULL),
        _gear(NULL),
        _power(NULL),
        _userModel(NULL),
        _modelIndex(0),
        _changeModel(false),
        _changeSound(false),
        _spriteShader(nullptr)
{
    // 表示するモデルの一覧を作成
    LoadModelName();
}

LAppMotionSyncLinkView::~LAppMotionSyncLinkView()
{
    delete _back;
    delete _fastForward;
    delete _gear;
    delete _power;
    if (_userModel)
    {
        _userModel->DeleteRenderer();
    }
    if (_spriteShader)
    {
        delete _spriteShader;
        _spriteShader = nullptr;
    }
    delete _userModel;
}

void LAppMotionSyncLinkView::Initialize()
{
    // モデルの初期化
    LoadModel();

    _spriteShader = new LAppSpriteShader();
}

void LAppMotionSyncLinkView::InitializeSprite()
{
    GLuint programId = _spriteShader->GetShaderId();

    int width = LAppMotionSyncLinkDelegate::GetInstance()->GetWindowWidth();
    int height = LAppMotionSyncLinkDelegate::GetInstance()->GetWindowHeight();

    LAppTextureManager* textureManager = LAppMotionSyncLinkDelegate::GetInstance()->GetTextureManager();
    const string resourcesPath = ResourcesPath;

    string imageName = BackImageName;
    LAppTextureManager::TextureInfo* backgroundTexture = textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

    float x = width * 0.5f;
    float y = height * 0.5f;
    float fWidth = (backgroundTexture->width * 2.0f);
    float fHeight = (height * 0.95f);

    if (_back == NULL)
    {
        _back = new LAppSprite(x, y, fWidth, fHeight, backgroundTexture->id, programId);
    }
    else
    {
        _back->ReSize(x, y, fWidth, fHeight);
    }

    imageName = FastForwardImageName;
    LAppTextureManager::TextureInfo* FastForwardTexture = textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

    x = (FastForwardTexture->width * 0.5f + 96.f);
    y = (height - FastForwardTexture->height * 0.5f);
    fWidth = static_cast<float>(FastForwardTexture->width);
    fHeight = static_cast<float>(FastForwardTexture->height);

    if (_fastForward == NULL)
    {
        _fastForward = new LAppSprite(x, y, fWidth, fHeight, FastForwardTexture->id, programId);
    }
    else
    {
        _fastForward->ReSize(x, y, fWidth, fHeight);
    }

    imageName = GearImageName;
    LAppTextureManager::TextureInfo* gearTexture = textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

    x = (width - gearTexture->width * 0.5f - 96.f);
    y = (height - gearTexture->height * 0.5f);
    fWidth = static_cast<float>(gearTexture->width);
    fHeight = static_cast<float>(gearTexture->height);

    if (_gear == NULL)
    {
        _gear = new LAppSprite(x, y, fWidth, fHeight, gearTexture->id, programId);
    }
    else
    {
        _gear->ReSize(x, y, fWidth, fHeight);
    }

    imageName = PowerImageName;
    LAppTextureManager::TextureInfo* powerTexture = textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

    x = (width - powerTexture->width * 0.5f - 96.f);
    y = (powerTexture->height * 0.5f);
    fWidth = static_cast<float>(powerTexture->width);
    fHeight = static_cast<float>(powerTexture->height);

    if (_power == NULL)
    {
        _power = new LAppSprite(x, y, fWidth, fHeight, powerTexture->id, programId);
    }
    else
    {
        _power->ReSize(x, y, fWidth, fHeight);
    }
}

void LAppMotionSyncLinkView::Render()
{
    // 画面サイズを取得する
    int maxWidth = LAppMotionSyncLinkDelegate::GetInstance()->GetWindowWidth();
    int maxHeight = LAppMotionSyncLinkDelegate::GetInstance()->GetWindowHeight();
    _back->SetWindowSize(maxWidth, maxHeight);
    _fastForward->SetWindowSize(maxWidth, maxHeight);
    _gear->SetWindowSize(maxWidth, maxHeight);
    _power->SetWindowSize(maxWidth, maxHeight);

    _back->Render();
    _fastForward->Render();
    _gear->Render();
    _power->Render();

    // 音声の切り替え
    if (_changeSound)
    {
        _changeSound = false;
        _userModel->ChangeNextMotion();
    }

    // モデルの切り替え
    if (_changeModel)
    {
        _changeModel = false;
        ChangeNextModel();
    }

    // モデルの描画
    _userModel->Update();
}

void LAppMotionSyncLinkView::OnTouchesBegan(float pointX, float pointY) const
{
}

void LAppMotionSyncLinkView::OnTouchesMoved(float pointX, float pointY) const
{
}

void LAppMotionSyncLinkView::OnTouchesEnded(float pointX, float pointY)
{
    // 早送りにタップしたか
    if (_fastForward->IsHit(pointX, pointY))
    {
        _changeSound = true;
    }

    // 歯車にタップしたか
    if (_gear->IsHit(pointX, pointY))
    {
        _changeModel = true;
    }

    // 電源ボタンにタップしたか
    if (_power->IsHit(pointX, pointY))
    {
        LAppMotionSyncLinkDelegate::GetInstance()->DeActivateApp();
    }
}

void LAppMotionSyncLinkView::LoadModelName()
{
    const char MODEL3_JSON[] = u8".model3.json";
    _modelNameList.Clear();
    Csm::csmVector<Csm::csmString> root = JniBridgeC::GetAssetList("");
    for (size_t i = 0; i < root.GetSize(); i++)
    {
        Csm::csmString target(root[i]);
        target.Append(MODEL3_JSON, sizeof(MODEL3_JSON) - 1);
        Csm::csmVector<Csm::csmString> sub = JniBridgeC::GetAssetList(root[i].GetRawString());
        for (size_t j = 0; j < sub.GetSize(); j++)
        {
            if (target == sub[j])
            {
                _modelNameList.PushBack(root[i]);
                break;
            }
        }
    }
    qsort(_modelNameList.GetPtr(), _modelNameList.GetSize(), sizeof(csmString), CompareCsmString);
}

void LAppMotionSyncLinkView::LoadModel()
{
    if (_userModel)
    {
        // レンダラの解放
        _userModel->DeleteRenderer();

        delete _userModel;
    }

    // モデルデータの新規生成
    _userModel = new LAppMotionSyncLinkModel();

    if (_modelIndex < _modelNameList.GetSize())
    {
        // モデルデータの読み込み及び生成とセットアップを行う
        _userModel->LoadAssets(_modelNameList[_modelIndex]);
    }
}

void LAppMotionSyncLinkView::ChangeNextModel()
{
    if (0 < _modelNameList.GetSize())
    {
        _modelIndex = (_modelIndex + 1) % _modelNameList.GetSize();
        LoadModel();
    }
}

LAppMotionSyncLinkModel* LAppMotionSyncLinkView::GetCurrentModel() const
{
    return _userModel;
}
