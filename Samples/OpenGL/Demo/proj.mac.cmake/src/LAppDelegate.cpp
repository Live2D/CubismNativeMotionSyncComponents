﻿/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppDelegate.hpp"
#include <sstream>
#include <dirent.h>
#include <mach-o/dyld.h>
#include <libgen.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "CubismMotionSyncEngineController.hpp"
#include "LAppPal.hpp"
#include "LAppDefine.hpp"
#include "LAppMotionSyncDefine.hpp"
#include "LAppMotionSyncModel.hpp"
#include "LAppTextureManager.hpp"

using namespace Csm;
using namespace std;
using namespace LAppDefine;
using namespace LAppMotionSyncDefine;

namespace {
    LAppDelegate* s_instance = NULL;

    int CompareCsmString(const void* a, const void* b)
    {
        return strcmp(reinterpret_cast<const Csm::csmString*>(a)->GetRawString(),
            reinterpret_cast<const Csm::csmString*>(b)->GetRawString());
    }
}

LAppDelegate* LAppDelegate::GetInstance()
{
    if (s_instance == NULL)
    {
        s_instance = new LAppDelegate();
    }

    return s_instance;
}

void LAppDelegate::ReleaseInstance()
{
    if (s_instance != NULL)
    {
        delete s_instance;
    }

    s_instance = NULL;
}

bool LAppDelegate::Initialize()
{
    LAppPal::PrintLog("START");

    // GLFWの初期化
    if (glfwInit() == GL_FALSE)
    {
        LAppPal::PrintLog("Can't initilize GLFW");

        return GL_FALSE;
    }

    // Windowの生成
    _window = glfwCreateWindow(LAppDefine::RenderTargetWidth, LAppDefine::RenderTargetHeight, "SIMPLE_SAMPLE", NULL, NULL);
    if (_window == NULL)
    {
        LAppPal::PrintLog("Can't create GLFW window.");

        glfwTerminate();
        return GL_FALSE;
    }

    // Windowのコンテキストをカレントに設定
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        LAppPal::PrintLog("Can't initilize glew.");

        glfwTerminate();
        return GL_FALSE;
    }

    //テクスチャサンプリング設定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //透過設定
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // コールバック関数の登録
    glfwSetMouseButtonCallback(_window, EventHandler::OnMouseCallBack);
    glfwSetCursorPosCallback(_window, EventHandler::OnMouseCallBack);

    // ウィンドウサイズ記憶
    glfwGetWindowSize(_window, &_windowWidth, &_windowHeight);

    // Cubism SDK の初期化
    InitializeCubism();
    
    _textureManager = new LAppTextureManager();

    // 実行中のプログラムファイルパスを設定
    SetExecuteAbsolutePath();
    
    // モデルの初期化
    LoadModelName();
    _modelIndex = 0;
    LoadModel();

    // 画像設定
    LoadGraph();

    // ウインドウ設定
    _back->SetWindowSize(_windowWidth, _windowHeight);
    _fastForward->SetWindowSize(_windowWidth, _windowHeight);
    _gear->SetWindowSize(_windowWidth, _windowHeight);
    _power->SetWindowSize(_windowWidth, _windowHeight);
    
    return GL_TRUE;
}

void LAppDelegate::Release()
{
    // レンダラの解放
    _userModel->DeleteRenderer();

    // モデルデータの解放
    delete _userModel;

    //　ボタンや背景削除
    delete _back;
    delete _fastForward;
    delete _gear;
    delete _power;

    // テクスチャマネージャーの解放
    delete _textureManager;

    // Windowの削除
    glfwDestroyWindow(_window);

    // OpenGLの処理を終了
    glfwTerminate();

    // Cubism SDK の解放
    Csm::CubismFramework::Dispose();
}

void LAppDelegate::Run()
{
    //メインループ
    while (glfwWindowShouldClose(_window) == GL_FALSE && !_isEnd)
    {
        int width, height;

        // ウィンドウサイズ記憶
        glfwGetWindowSize(_window, &width, &height);
        if ((_windowWidth != width || _windowHeight != height) && width > 0 && height > 0)
        {
            // スプライトサイズを再設定
            ResizeSprite();
            // サイズを保存しておく
            _windowWidth = width;
            _windowHeight = height;
            
            // ウインドウ更新
            _back->SetWindowSize(_windowWidth, _windowHeight);
            _fastForward->SetWindowSize(_windowWidth, _windowHeight);
            _gear->SetWindowSize(_windowWidth, _windowHeight);
            _power->SetWindowSize(_windowWidth, _windowHeight);
        }

        // 時間更新
        LAppPal::UpdateTime();

        // 画面の初期化
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearDepth(1.0);

        // 画像描画
        _back->Render();
        _fastForward->Render();
        _gear->Render();
        _power->Render();

        // モデルの更新及び描画
        _userModel->Update();

        // バッファの入れ替え
        glfwSwapBuffers(_window);

        // Poll for and process events
        glfwPollEvents();
    }

    Release();

    ReleaseInstance();
}

LAppDelegate::LAppDelegate():
    _cubismOption(),
    _window(NULL),
    _captured(false),
    _mouseX(0.0f),
    _mouseY(0.0f),
    _isEnd(false),
    _back(NULL),
    _fastForward(NULL),
    _gear(NULL),
    _power(NULL),
    _userModel(NULL),
    _windowWidth(0),
    _windowHeight(0)
{
    _textureManager = new LAppTextureManager();
}

LAppDelegate::~LAppDelegate()
{
    MotionSync::CubismMotionSyncEngineController::DeleteAllEngine();
}

void LAppDelegate::InitializeCubism()
{
    //setup cubism
    _cubismOption.LogFunction = LAppPal::PrintMessage;
    _cubismOption.LoggingLevel = Csm::CubismFramework::Option::LogLevel_Verbose;
    Csm::CubismFramework::StartUp(&_cubismAllocator, &_cubismOption);

    //Initialize cubism
    Csm::CubismFramework::Initialize();
}

void LAppDelegate::OnMouseCallBack(GLFWwindow* window, int button, int action, int modify)
{
    if (GLFW_MOUSE_BUTTON_LEFT != button)
    {
        return;
    }

    if (GLFW_PRESS == action)
    {
        _captured = true;
        OnTouchesBegan(_mouseX, _mouseY);
    }
    else if (GLFW_RELEASE == action)
    {
        if (_captured)
        {
            _captured = false;
            OnTouchesEnded(_mouseX, _mouseY);
        }
    }
}

void LAppDelegate::OnMouseCallBack(GLFWwindow* window, double x, double y)
{
    _mouseX = static_cast<float>(x);
    _mouseY = static_cast<float>(y);

    if (!_captured)
    {
        return;
    }

    OnTouchesMoved(_mouseX, _mouseY);
}

void LAppDelegate::OnTouchesBegan(float px, float py)
{
}

void LAppDelegate::OnTouchesMoved(float px, float py)
{
}

void LAppDelegate::OnTouchesEnded(float px, float py)
{
    // 早送りにタップしたか
    if (_fastForward->IsHit(px, py))
    {
        _userModel->ChangeNextIndexSound();
    }

    // 歯車にタップしたか
    if (_gear->IsHit(px, py))
    {
        ChangeNextModel();
    }

    // 電源ボタンにタップしたか
    if (_power->IsHit(px, py))
    {
        AppEnd();
    }
}

GLuint LAppDelegate::CreateShader()
{
    //バーテックスシェーダのコンパイル
    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShader =
        "#version 120\n"
        "attribute vec3 position;"
        "attribute vec2 uv;"
        "varying vec2 vuv;"
        "void main(void){"
        "    gl_Position = vec4(position, 1.0);"
        "    vuv = uv;"
        "}";
    glShaderSource(vertexShaderId, 1, &vertexShader, NULL);
    glCompileShader(vertexShaderId);
    if (!CheckShader(vertexShaderId))
    {
        return 0;
    }

    //フラグメントシェーダのコンパイル
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShader =
        "#version 120\n"
        "varying vec2 vuv;"
        "uniform sampler2D texture;"
        "uniform vec4 baseColor;"
        "void main(void){"
        "    gl_FragColor = texture2D(texture, vuv) * baseColor;"
        "}";
    glShaderSource(fragmentShaderId, 1, &fragmentShader, NULL);
    glCompileShader(fragmentShaderId);
    if (!CheckShader(fragmentShaderId))
    {
        return 0;
    }

    //プログラムオブジェクトの作成
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertexShaderId);
    glAttachShader(programId, fragmentShaderId);

    // リンク
    glLinkProgram(programId);

    glUseProgram(programId);

    return programId;
}

bool LAppDelegate::CheckShader(GLuint shaderId)
{
    GLint status;
    GLint logLength;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar* log = reinterpret_cast<GLchar*>(CSM_MALLOC(logLength));
        glGetShaderInfoLog(shaderId, logLength, &logLength, log);
        CubismLogError("Shader compile log: %s", log);
        CSM_FREE(log);
    }

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glDeleteShader(shaderId);
        return false;
    }

    return true;
}

void LAppDelegate::LoadModelName()
{
    // ResourcesPathの中にあるフォルダ名を全てクロールし、モデルが存在するフォルダを定義する。
    // フォルダはあるが同名の.model3.jsonが見つからなかった場合はリストに含めない。
    struct dirent *dirent;

    csmString crawlPath(_executeAbsolutePath.c_str());
    crawlPath += ResourcesPath;
    
    DIR *pDir = opendir(crawlPath.GetRawString());
    if (pDir == NULL) return;

    _modelNameList.Clear();

    while ((dirent = readdir(pDir)) != NULL)
    {
        if ((dirent->d_type & DT_DIR) && strcmp(dirent->d_name, "..") != 0)
        {
            // フォルダと同名の.model3.jsonがあるか探索する
            struct dirent *dirent2;

            csmString modelName(dirent->d_name);

            csmString modelPath(crawlPath);
            modelPath += modelName;
            modelPath.Append(1, '/');

            csmString model3jsonName(modelName);
            model3jsonName += ".model3.json";

            DIR *pDir2 = opendir(modelPath.GetRawString());
            while ((dirent2 = readdir(pDir2)) != NULL)
            {
                if (strcmp(dirent2->d_name, model3jsonName.GetRawString()) == 0)
                {
                    _modelNameList.PushBack(csmString(dirent->d_name));
                }
            }
            closedir(pDir2);
        }
    }
    closedir(pDir);
    qsort(_modelNameList.GetPtr(), _modelNameList.GetSize(), sizeof(csmString), CompareCsmString);
}

void LAppDelegate::LoadModel()
{
    if (_userModel)
    {
        // レンダラの解放
        _userModel->DeleteRenderer();

        delete _userModel;
    }

    // モデルデータの新規生成
    _userModel = new LAppMotionSyncModel(_executeAbsolutePath.c_str());

    if (_modelIndex < _modelNameList.GetSize())
    {
        // モデルデータの読み込み及び生成とセットアップを行う
        _userModel->LoadAssets(_modelNameList[_modelIndex]);
    }
}

void LAppDelegate::ChangeNextModel()
{
    if (0 < _modelNameList.GetSize())
    {
        _modelIndex = (_modelIndex + 1) % _modelNameList.GetSize();
        LoadModel();
    }
}

void LAppDelegate::LoadGraph()
{
    GLuint programId = CreateShader();

    int width, height;
    glfwGetWindowSize(_window, &width, &height);

    const std::string resourcesPath = _executeAbsolutePath + std::string(LAppDefine::ResourcesPath);

    std::string imageName = LAppDefine::BackImageName;
    LAppTextureManager::TextureInfo* backgroundTexture = _textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

    float x = width * 0.5f;
    float y = height * 0.5f;
    float fWidth = static_cast<float>(backgroundTexture->width * 2.0f);
    float fHeight = static_cast<float>(height * 0.95f);
    _back = new LAppSprite(x, y, fWidth, fHeight, backgroundTexture->id, programId);

    imageName = LAppMotionSyncDefine::FastForwardImageName;
    LAppTextureManager::TextureInfo* fastForwardTexture = _textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

    x = static_cast<float>(fastForwardTexture->width * 0.5f);
    y = static_cast<float>(height - fastForwardTexture->height * 0.5f);
    fWidth = static_cast<float>(fastForwardTexture->width);
    fHeight = static_cast<float>(fastForwardTexture->height);
    _fastForward = new LAppSprite(x, y, fWidth, fHeight, fastForwardTexture->id, programId);

    imageName = LAppDefine::GearImageName;
    LAppTextureManager::TextureInfo* gearTexture = _textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

    x = static_cast<float>(width - gearTexture->width * 0.5f);
    y = static_cast<float>(height - gearTexture->height * 0.5f);
    fWidth = static_cast<float>(gearTexture->width);
    fHeight = static_cast<float>(gearTexture->height);
    _gear = new LAppSprite(x, y, fWidth, fHeight, gearTexture->id, programId);

    imageName = LAppDefine::PowerImageName;
    LAppTextureManager::TextureInfo* powerTexture = _textureManager->CreateTextureFromPngFile(resourcesPath + imageName);

    x = static_cast<float>(width - powerTexture->width * 0.5f);
    y = static_cast<float>(powerTexture->height * 0.5f);
    fWidth = static_cast<float>(powerTexture->width);
    fHeight = static_cast<float>(powerTexture->height);
    _power = new LAppSprite(x, y, fWidth, fHeight, powerTexture->id, programId);
}

void LAppDelegate::ResizeSprite()
{
    if (!_textureManager)
    {
        return;
    }

    // 描画領域サイズ
    int width, height;
    glfwGetWindowSize(_window, &width, &height);

    float x = 0.0f;
    float y = 0.0f;
    float fWidth = 0.0f;
    float fHeight = 0.0f;

    if (_back)
    {
        GLuint id = _back->GetTextureId();
        LAppTextureManager::TextureInfo* texInfo = _textureManager->GetTextureInfoById(id);
        if (texInfo)
        {
            x = width * 0.5f;
            y = height * 0.5f;
            fWidth = static_cast<float>(texInfo->width * 2);
            fHeight = static_cast<float>(height) * 0.95f;
            _back->ResetRect(x, y, fWidth, fHeight);
        }
    }

    if (_power)
    {
        GLuint id = _power->GetTextureId();
        LAppTextureManager::TextureInfo* texInfo = _textureManager->GetTextureInfoById(id);
        if (texInfo)
        {
            x = static_cast<float>(width - texInfo->width * 0.5f);
            y = static_cast<float>(texInfo->height * 0.5f);
            fWidth = static_cast<float>(texInfo->width);
            fHeight = static_cast<float>(texInfo->height);
            _power->ResetRect(x, y, fWidth, fHeight);
        }
    }

    if (_gear)
    {
        GLuint id = _gear->GetTextureId();
        LAppTextureManager::TextureInfo* texInfo = _textureManager->GetTextureInfoById(id);
        if (texInfo)
        {
            x = static_cast<float>(width - texInfo->width * 0.5f);
            y = static_cast<float>(height - texInfo->height * 0.5f);
            fWidth = static_cast<float>(texInfo->width);
            fHeight = static_cast<float>(texInfo->height);
            _gear->ResetRect(x, y, fWidth, fHeight);
        }
    }

    if (_fastForward)
    {
        GLuint id = _fastForward->GetTextureId();
        LAppTextureManager::TextureInfo* texInfo = _textureManager->GetTextureInfoById(id);
        if (texInfo)
        {
            x = static_cast<float>(texInfo->width * 0.5f);
            y = static_cast<float>(height - texInfo->height * 0.5f);
            fWidth = static_cast<float>(texInfo->width);
            fHeight = static_cast<float>(texInfo->height);
            _fastForward->ResetRect(x, y, fWidth, fHeight);
        }
    }
}

void LAppDelegate::SetExecuteAbsolutePath()
{
    char path[1024];
    uint32_t size = sizeof(path);
    _NSGetExecutablePath(path, &size);
    this->_executeAbsolutePath = dirname(path);
    this->_executeAbsolutePath += "/";
}
