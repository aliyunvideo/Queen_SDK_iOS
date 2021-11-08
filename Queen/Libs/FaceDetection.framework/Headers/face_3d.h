//
//  face_attribute.hpp
//  skinsoften_iOS_full
//
//  Created by linfeng on 2018/4/28.
//  Copyright © 2018年 Feng lin. All rights reserved.
//

#ifndef _FACE_3D_H_
#define _FACE_3D_H_

#include <string>
#include <vector>
#include <sys/time.h>
#include "dl_face_common.h"
#include <MNN/Interpreter.hpp>
#include <MNN/Tensor.hpp>
#include <MNN/ImageProcess.hpp>
#include <memory>

#define ALIGN_FACE_SIZE_3D 120

using namespace std;

class Face3D {
public:
    Face3D();
    ~Face3D(void);
    int Init(const char* modelPath, const char *tb3dmmPath);
    
    int face_3d_detect(const unsigned char *img_data, tb_pixel_format pixel_format, face_106_t *p_faces, int width, int height, int stride, float *&param_3dmm, float *&output_vertex);
    int get_face_3d_params(int &vertex_num, int &tri_num, int *&tri, int &kpt_num, int *&kpt_idxs);
    int reconstruct_vertex(float *params, float scale);
    rect_t parse_roi_box(rect_t bbox);
    
    int DataTransform(const unsigned char *rgba_data, int src_w,int src_h, int dst_w, int dst_h, float *ptr);
    int getAffineTransform(float *srcTri, float *dstTri);
    bool isEnabled;
    const char* GetBizCode();
private:
    std::shared_ptr<MNN::Interpreter> m_face3d_net_;
    MNN::Session* mSession = nullptr;
    MNN::Tensor* mImage = nullptr;
    MNN::Tensor* mOutput;
    const MNN::Backend* mBackend = nullptr;
    
    float m_srcTri[6];
    float m_dstTri[6];
    float m_warp_matrix[9];

    MNN::CV::ImageProcess::Config color_config;
    std::shared_ptr<MNN::CV::ImageProcess> m_Pretreat;
    MNN::CV::Matrix m_trans_matrix;
    
    // 3dmm params
    float *m_mu;
    int *m_tri;
    float *m_w_exp;
    float *m_w_shape;
    int *m_3d_kpt;
    
    int m_vertex_num;
    int m_tri_num;
    int m_exp_num;
    int m_shape_num;
    int m_3d_kpt_num;
    
    //output
    float m_3dmm_param[62];
    float *m_output_vertex;
    
};




#endif /* _FACE_ATTRIBUTE_H_ */
