//
//  pixelai_face3D_private.h
//  Face3D
//
//  Created by limengyao on 2019/8/16.
//  Copyright © 2019 shiwei zhou. All rights reserved.
//

#ifndef pixelai_face3D_private_h
#define pixelai_face3D_private_h

#include "PointsDensify.h"
#include "VertexProject.hpp"
#include "WarpImage.hpp"
#include "TriangleIndex.h"

// Convert 106 Points to 72 Points
const int PIXELAI_SELECT_FACE_ID[]={0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,
    32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,
    52,53,54,55,56,57,58,59,60,61,62,63,84,85,86,87,88,89,90,91,
    92,93,94,95,97,98,99,101,102,103,78,79,80,81,82,83};

//Convert 240 Points to 72 Points
const int PIXELAI_SELECT_FACE_ID_EXTRA[] = {
    0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32,
    150,152,153,155,156,169,167,166,164,163,
    43,44,45,46,47,48,49,50,51,
    117,119,125,127,113,108,149,147,141,128,131,136,
    176,179,182,184,186,189,192,237,234,232,230,228,198,201,203,218,216,213,
    78,79,80,81,82,83,
};
#define PIXELAI_FACE3D_OUTKP_WIDTH 50

#endif /* pixelai_face3D_private_h */
