/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include <GL/glew.h>
#include <CubismFramework.hpp>
#include "LAppMicrophoneDelegate.hpp"

int main()
{
    // create the application instance
    if (LAppMicrophoneDelegate::GetInstance()->Initialize() == GL_FALSE)
    {
        return 1;
    }

    LAppMicrophoneDelegate::GetInstance()->Run();

    return 0;
}
