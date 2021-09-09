//
//  face_attribute.hpp
//  skinsoften_iOS_full
//
//  Created by linfeng on 2018/4/28.
//  Copyright © 2018年 Feng lin. All rights reserved.
//

#ifndef _FACE_SMILE_H_
#define _FACE_SMILE_H_

#include <string>
#include <vector>
#include <sys/time.h>
#include <MNN/Interpreter.hpp>
#include <MNN/Tensor.hpp>
#include <MNN/ImageProcess.hpp>
#include "dl_face_common.h"
#include "MobileDLFace.h"
#include <memory>

#define ALIGN_FACE_SIZE_SMILE 112

using namespace std;

class FaceSmile {
public:
    FaceSmile();
    ~FaceSmile(void);
    int Init(const char* modelPath);
    
    int face_smile_detect(const unsigned char *img_data, tb_pixel_format pixel_format, face_106_t *p_faces, int width, int height, int stride, face_smile_t *smile);
    
    int DataTransform(const unsigned char *y_data, float* dst, int src_w,int src_h, int dst_w, int dst_h);
    int getAffineTransform(float *srcTri, float *dstTri);
    int setThreshold(float value);
    bool isEnabled;
    const char* GetBizCode();
private:
    std::shared_ptr<MNN::Interpreter> mNet;
    MNN::Session* mSession = nullptr;
    MNN::Tensor* mImage = nullptr;
    MNN::Tensor* mOutput = nullptr;
    const MNN::Backend* mBackend = nullptr;
    
//    std::shared_ptr<MNN::Tensor> m_gray_img;
    std::shared_ptr<MNN::CV::ImageProcess> m_Pretreat;
    MNN::CV::Matrix m_trans_matrix;
    
    float m_srcTri[6];
    float m_dstTri[6];
    float m_warp_matrix[9];
    float m_threadhold;
};




#endif /* _FACE_SMILE_H_ */
