//
//  ErrorType.h
//  Face3D
//
//  Created by shiwei zhou on 2018/11/19.
//  Copyright © 2018 shiwei zhou. All rights reserved.
//

#ifndef ErrorType_h
#define ErrorType_h

enum class ErrorType
{
    LOAD_MEAN_MODEL_ERROR,
    LOAD_normalised_pca_basis_MODEL_ERROR,
    LOAD_blendshapes_MODEL_ERROR,
    LOAD_2DTo3D_MODEL_ERROR,
    LOAD_2DTo3D_Edge_MODEL_ERROR,
    LOAD_FaceBeauty_MODEL_ERROR,
    LOAD_FacePara_MODEL_ERROR,
    Input_FaceLandMarks_NUM_ERROR,
};

enum class ProcessErrorType
{
    PACKAGE_NOT_EXISTS,
    CONFIG_FILE_NOT_FOUND
};
#endif /* ErrorType_h */
