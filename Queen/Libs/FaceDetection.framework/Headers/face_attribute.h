//
//  face_attribute.hpp
//  skinsoften_iOS_full
//
//  Created by linfeng on 2018/4/28.
//  Copyright © 2018年 Feng lin. All rights reserved.
//

#ifndef _FACE_ATTRIBUTE_H_
#define _FACE_ATTRIBUTE_H_

#include <string>
#include <vector>
#include <sys/time.h>
#include "dl_face_common.h"
#include <MNN/Interpreter.hpp>
#include <MNN/Tensor.hpp>
#include <MNN/ImageProcess.hpp>
#include <memory>

#define ALIGN_FACE_SIZE_ATT 128

using namespace std;

class FaceAttribute {
public:
    FaceAttribute();
    ~FaceAttribute(void);
    int Init(const char* modelPath);
    
    int face_attribute_detect(const unsigned char *img_data, tb_pixel_format pixel_format, p_mobile_face_info_t p_detection_result, int width, int height, int stride, unsigned int config, p_face_attributes_t *attributes_array);
    
    int DataTransform(const unsigned char *rgba_data, int src_w,int src_h, int dst_w, int dst_h, float *ptr);
    int getAffineTransform(float *srcTri, float *dstTri);
    bool isEnabled;
    const char* GetBizCode();
private:
    std::shared_ptr<MNN::Interpreter> m_attribute_net_;
    MNN::Session* mSession = nullptr;
    MNN::Tensor* mImage = nullptr;
    MNN::Tensor* mFaceAge;
    MNN::Tensor* mFaceGender;
    MNN::Tensor* mFaceBeauty;
    MNN::Tensor* mFaceEmotion;
    MNN::Tensor* mFaceGlasses;
    const MNN::Backend* mBackend = nullptr;
    
    float m_srcTri[6];
    float m_dstTri[6];
    float m_warp_matrix[9];

    MNN::CV::ImageProcess::Config color_config;
    std::shared_ptr<MNN::CV::ImageProcess> m_Pretreat;
    MNN::CV::Matrix m_trans_matrix;
    
    p_face_attributes_t ouput_attributes;
    
};




#endif /* _FACE_ATTRIBUTE_H_ */
