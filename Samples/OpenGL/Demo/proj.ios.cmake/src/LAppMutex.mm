/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#import "LAppMutex.h"

LAppMutex::LAppMutex()
{
    pthread_mutex_init(&_mutex, NULL);
}

LAppMutex::~LAppMutex()
{
    pthread_mutex_destroy(&_mutex);
}

void LAppMutex::Lock()
{
    pthread_mutex_lock(&_mutex);
}

void LAppMutex::Unlock()
{
    pthread_mutex_unlock(&_mutex);
}
