/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "JniBridgeC.hpp"
#include <algorithm>
#include <jni.h>
#include "LAppDelegate.hpp"
#include "LAppPal.hpp"

using namespace Csm;

static JavaVM* g_JVM; // JavaVM is valid for all threads, so just save it globally
static jclass  g_JniBridgeJavaClass;
static jclass  g_AudioManagerJavaClass;
static jmethodID g_GetAssetsMethodId;
static jmethodID g_LoadFileMethodId;
static jmethodID g_GetJniLibPathMethodId;
// 音声周り
static jmethodID g_CreateAudioInputMethodId;
static jmethodID g_ReadAudioInputMethodId;
static jmethodID g_StopAudioInputMethodId;
static jmethodID g_DeleteAudioInputMethodId;
static jmethodID g_CreateAudioOutputMethodId;
static jmethodID g_WriteAudioOutputMethodId;
static jmethodID g_GetAudioOutputPositionMethodId;
static jmethodID g_StopAudioOutputMethodId;
static jmethodID g_DeleteAudioOutputMethodId;
// 終了処理
static jmethodID g_MoveTaskToBackMethodId;

JNIEnv* GetEnv()
{
    JNIEnv* env = NULL;
    g_JVM->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);
    return env;
}

// The VM calls JNI_OnLoad when the native library is loaded
jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
    g_JVM = vm;

    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK)
    {
        return JNI_ERR;
    }

    jclass clazz = env->FindClass("com/live2d/demo/JniBridgeJava");
    g_JniBridgeJavaClass = reinterpret_cast<jclass>(env->NewGlobalRef(clazz));
    clazz = env->FindClass("com/live2d/demo/AudioManager");
    g_AudioManagerJavaClass = reinterpret_cast<jclass>(env->NewGlobalRef(clazz));
    g_GetAssetsMethodId = env->GetStaticMethodID(g_JniBridgeJavaClass, "getAssetList", "(Ljava/lang/String;)[Ljava/lang/String;");
    g_LoadFileMethodId = env->GetStaticMethodID(g_JniBridgeJavaClass, "loadFile", "(Ljava/lang/String;)[B");
    g_GetJniLibPathMethodId = env->GetStaticMethodID(g_JniBridgeJavaClass, "getJniLibPath", "()Ljava/lang/String;");
    g_CreateAudioInputMethodId = env->GetStaticMethodID(g_AudioManagerJavaClass, "createAudioInput", "(III)I");
    g_ReadAudioInputMethodId = env->GetStaticMethodID(g_AudioManagerJavaClass, "readAudioInput", "(I)[B");
    g_StopAudioInputMethodId = env->GetStaticMethodID(g_AudioManagerJavaClass, "stopAudioInput", "(I)Z");
    g_DeleteAudioInputMethodId = env->GetStaticMethodID(g_AudioManagerJavaClass, "deleteAudioInput", "(I)Z");
    g_CreateAudioOutputMethodId = env->GetStaticMethodID(g_AudioManagerJavaClass, "createAudioOutput", "(III)I");
    g_WriteAudioOutputMethodId = env->GetStaticMethodID(g_AudioManagerJavaClass, "writeAudioOutput", "(II[BI)Z");
    g_GetAudioOutputPositionMethodId = env->GetStaticMethodID(g_AudioManagerJavaClass, "getAudioOutputPosition", "(I)I");
    g_StopAudioOutputMethodId = env->GetStaticMethodID(g_AudioManagerJavaClass, "stopAudioOutput", "(I)Z");
    g_DeleteAudioOutputMethodId = env->GetStaticMethodID(g_AudioManagerJavaClass, "deleteAudioOutput", "(I)Z");
    g_MoveTaskToBackMethodId = env->GetStaticMethodID(g_JniBridgeJavaClass, "moveTaskToBack", "()V");

    return JNI_VERSION_1_6;
}

void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved)
{
    JNIEnv *env = GetEnv();
    env->DeleteGlobalRef(g_JniBridgeJavaClass);
}

Csm::csmVector<Csm::csmString>JniBridgeC::GetAssetList(const Csm::csmString& path)
{
    JNIEnv *env = GetEnv();
    jobjectArray obj = reinterpret_cast<jobjectArray>(env->CallStaticObjectMethod(g_JniBridgeJavaClass, g_GetAssetsMethodId, env->NewStringUTF(path.GetRawString())));
    unsigned int size = static_cast<unsigned int>(env->GetArrayLength(obj));
    Csm::csmVector<Csm::csmString> list(size);
    for (unsigned int i = 0; i < size; i++)
    {
        jstring jstr = reinterpret_cast<jstring>(env->GetObjectArrayElement(obj, i));
        const char* chars = env->GetStringUTFChars(jstr, nullptr);
        list.PushBack(Csm::csmString(chars));
        env->ReleaseStringUTFChars(jstr, chars);
        env->DeleteLocalRef(jstr);
    }
    return list;
}

char* JniBridgeC::LoadFileAsBytesFromJava(const char* filePath, unsigned int* outSize)
{
    JNIEnv *env = GetEnv();

    // ファイルロード
    jbyteArray obj = (jbyteArray)env->CallStaticObjectMethod(g_JniBridgeJavaClass, g_LoadFileMethodId, env->NewStringUTF(filePath));

    // ファイルが見つからなかったらnullが返ってくるためチェック
    if (!obj)
    {
        return NULL;
    }

    unsigned int arraySize = static_cast<unsigned int>(env->GetArrayLength(obj));

    char* buffer = new char[arraySize];
    env->GetByteArrayRegion(obj, 0, arraySize, reinterpret_cast<jbyte *>(buffer));

    if (outSize)
    {
        *outSize = arraySize;
    }

    return buffer;
}

csmString JniBridgeC::GetJniLibPath()
{
    JNIEnv *env = GetEnv();

    jstring jstr = reinterpret_cast<jstring>(env->CallStaticObjectMethod(g_JniBridgeJavaClass, g_GetJniLibPathMethodId, NULL));
    const char* chars = env->GetStringUTFChars(jstr, nullptr);
    csmString str = csmString(chars);
    env->ReleaseStringUTFChars(jstr, chars);
    env->DeleteLocalRef(jstr);

    return str;
}

csmInt32 JniBridgeC::CreateAudioInputBuffer(csmInt32 sampleRate, csmInt32 channels, csmInt32 bitDepth)
{
    JNIEnv *env = GetEnv();

    jint id = env->CallStaticIntMethod(g_AudioManagerJavaClass, g_CreateAudioInputMethodId, sampleRate, channels, bitDepth);

    return static_cast<csmInt32>(id);
}

csmByte* JniBridgeC::ReadAudioInputBuffer(csmInt32 id, Csm::csmUint32* outSize)
{
    JNIEnv *env = GetEnv();

    // ファイルロード
    jbyteArray obj = (jbyteArray)env->CallStaticObjectMethod(g_AudioManagerJavaClass, g_ReadAudioInputMethodId, id);
    unsigned int arraySize = static_cast<unsigned int>(env->GetArrayLength(obj));

    csmByte* buffer = new csmByte[arraySize];
    env->GetByteArrayRegion(obj, 0, arraySize, reinterpret_cast<jbyte *>(buffer));

    if (outSize)
    {
        *outSize = arraySize;
    }

    return buffer;
}

csmBool JniBridgeC::StopAudioInputBuffer(csmInt32 id)
{
    JNIEnv *env = GetEnv();

    jboolean result = env->CallStaticBooleanMethod(g_AudioManagerJavaClass, g_StopAudioInputMethodId, id);

    return static_cast<csmBool>(result);
}

csmBool JniBridgeC::DeleteAudioInputBuffer(csmInt32 id)
{
    JNIEnv *env = GetEnv();

    jboolean result = env->CallStaticBooleanMethod(g_AudioManagerJavaClass, g_DeleteAudioInputMethodId, id);

    return static_cast<csmBool>(result);
}

csmInt32 JniBridgeC::CreateAudioOutputBuffer(csmInt32 sampleRate, csmInt32 channels, csmInt32 bitDepth)
{
    JNIEnv *env = GetEnv();

    jint id = env->CallStaticIntMethod(g_AudioManagerJavaClass, g_CreateAudioOutputMethodId, sampleRate, channels, bitDepth);

    return static_cast<csmInt32>(id);
}

csmBool JniBridgeC::WriteAudioOutputBuffer(csmInt32 id, csmInt32 writePos, csmByte* data, csmInt32 dataSize)
{
    JNIEnv *env = GetEnv();

    // char配列をbyte配列に変換
    jbyteArray byteArray = env->NewByteArray(dataSize);
    jbyte* byteData = env->GetByteArrayElements(byteArray, NULL);
    for (csmInt32 i = 0; i < dataSize; i++)
    {
        byteData[i] = static_cast<jbyte>(data[i]);
    }

    jboolean result = env->CallStaticBooleanMethod(g_AudioManagerJavaClass, g_WriteAudioOutputMethodId, id, writePos, byteArray, dataSize);

    // メモリ開放
    env->ReleaseByteArrayElements(byteArray, byteData, 0);

    return static_cast<csmBool>(result);
}

csmInt32 JniBridgeC::GetAudioOutputBufferPosition(csmInt32 id)
{
    JNIEnv *env = GetEnv();

    jint pos = env->CallStaticIntMethod(g_AudioManagerJavaClass, g_GetAudioOutputPositionMethodId, id);

    return static_cast<csmInt32>(pos);
}

csmBool JniBridgeC::StopAudioOutputBuffer(Csm::csmInt32 id)
{
    JNIEnv *env = GetEnv();

    jboolean result = env->CallStaticBooleanMethod(g_AudioManagerJavaClass, g_StopAudioOutputMethodId, id);

    return static_cast<csmBool>(result);
}

csmBool JniBridgeC::DeleteAudioOutputBuffer(csmInt32 id)
{
    JNIEnv *env = GetEnv();

    jboolean result = env->CallStaticBooleanMethod(g_AudioManagerJavaClass, g_DeleteAudioOutputMethodId, id);

    return static_cast<csmBool>(result);
}

void JniBridgeC::MoveTaskToBack()
{
    JNIEnv *env = GetEnv();

    // アプリ終了
    env->CallStaticVoidMethod(g_JniBridgeJavaClass, g_MoveTaskToBackMethodId, NULL);
}

extern "C"
{
    JNIEXPORT void JNICALL
    Java_com_live2d_demo_JniBridgeJava_nativeOnStart(JNIEnv *env, jclass type)
    {
        LAppDelegate::GetInstance()->OnStart();
    }

    JNIEXPORT void JNICALL
    Java_com_live2d_demo_JniBridgeJava_nativeOnPause(JNIEnv *env, jclass type)
    {
        LAppDelegate::GetInstance()->OnPause();
    }

    JNIEXPORT void JNICALL
    Java_com_live2d_demo_JniBridgeJava_nativeOnStop(JNIEnv *env, jclass type)
    {
        LAppDelegate::GetInstance()->OnStop();
    }

    JNIEXPORT void JNICALL
    Java_com_live2d_demo_JniBridgeJava_nativeOnDestroy(JNIEnv *env, jclass type)
    {
        LAppDelegate::GetInstance()->OnDestroy();
    }

    JNIEXPORT void JNICALL
    Java_com_live2d_demo_JniBridgeJava_nativeOnSurfaceCreated(JNIEnv *env, jclass type)
    {
        LAppDelegate::GetInstance()->OnSurfaceCreate();
    }

    JNIEXPORT void JNICALL
    Java_com_live2d_demo_JniBridgeJava_nativeOnSurfaceChanged(JNIEnv *env, jclass type, jint width, jint height)
    {
        LAppDelegate::GetInstance()->OnSurfaceChanged(width, height);
    }

    JNIEXPORT void JNICALL
    Java_com_live2d_demo_JniBridgeJava_nativeOnDrawFrame(JNIEnv *env, jclass type)
    {
        LAppDelegate::GetInstance()->Run();
    }

    JNIEXPORT void JNICALL
    Java_com_live2d_demo_JniBridgeJava_nativeOnTouchesBegan(JNIEnv *env, jclass type, jfloat pointX, jfloat pointY)
    {
        LAppDelegate::GetInstance()->OnTouchBegan(pointX, pointY);
    }

    JNIEXPORT void JNICALL
    Java_com_live2d_demo_JniBridgeJava_nativeOnTouchesEnded(JNIEnv *env, jclass type, jfloat pointX, jfloat pointY)
    {
        LAppDelegate::GetInstance()->OnTouchEnded(pointX, pointY);
    }

    JNIEXPORT void JNICALL
    Java_com_live2d_demo_JniBridgeJava_nativeOnTouchesMoved(JNIEnv *env, jclass type, jfloat pointX, jfloat pointY)
    {
        LAppDelegate::GetInstance()->OnTouchMoved(pointX, pointY);
    }
}
