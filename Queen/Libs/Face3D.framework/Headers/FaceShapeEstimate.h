//
//  FaceShapeEstimate.h
//  Face3D
//
//  Created by limengyao on 2019/8/19.
//  Copyright © 2019 shiwei zhou. All rights reserved.
//

#ifndef FaceShapeEstimate_h
#define FaceShapeEstimate_h
#include "ALMFaceShapeType.h"

#define FACE_SHAPE_NUM 6
#define FEATURE_TOTAL_NUM 40
#define FEATURE_NUM 40

int getFaceType(std::vector<std::vector<float>> face_center, std::vector<float> feature);
std::vector<std::vector<float>> readCenter(std::istream* f, size_t row, size_t col);

#endif /* FaceShapeEstimate_h */
