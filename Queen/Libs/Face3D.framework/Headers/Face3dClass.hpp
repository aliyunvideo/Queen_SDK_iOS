//
//  Face3dClass.h
//  Face3D
//
//  Created by limengyao on 2020/1/16.
//  Copyright © 2020 shiwei zhou. All rights reserved.
//

#ifndef Face3dClass_h
#define Face3dClass_h

#include <mach-o/dyld.h>
#include <string>

#if defined(__APPLE__) && !defined(ANDROID)
#include <TargetConditionals.h>
#endif

std::string GetFace3dResPath();

#endif /* Face3dClass_h */
