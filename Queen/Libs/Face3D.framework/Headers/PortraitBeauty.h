//
//  PortraitBeauty.h
//
//  Created by zhoushiwei on 14-3-10.
//  Copyright (c) 2014年 zhoushiwei. All rights reserved.
//

#ifndef PortraitBeauty_h
#define PortraitBeauty_h

#include <iostream>
#include <string.h>
#include <vector>
#include <opencv2/core.hpp>
#include "pixelai_face3D.h"

using namespace cv;
using namespace std;


class FaceShapeParameter{
    
public:
    bool enable;
    float scale;
};


class PortraitBeauty{
    
private:

public:
    PortraitBeauty();
    ~PortraitBeauty();
    
public:
    bool InitData(const std::string& path = "",const pixelai_istream_func& istream_func = nullptr);
    bool loadFaceShapeModel(const std::string& path);
    Mat FaceBeauty(cv::Mat&shape2D);
    std::vector<cv::Point2f> ProjectShapeTo2D(Mat&Shape_3D);
    void doBeauty(Mat &SS_);
    void doBeautyForTri(Mat& SS_);
    bool ReadData(const std::string &Mainpath, std::vector<cv::Mat> &params);
    void RemoveJitter(const std::vector<std::array<float, 2>>& prev, std::vector<std::array<float, 2>>& landmarks, float& model_scale);
    
    ALMFaceType GetFaceType(vector<float> feature);
    
    void SetFaceParam(float scale, ALMFaceShapeType type)
    {
        if (type >= ALMFaceShapeTypMAX || type < 0) {
            return;
        }
        faceshape_param[type].enable = true;
        faceshape_param[type].scale = scale;
    }
    
    float GetFaceParam(ALMFaceShapeType type)
    {
        return faceshape_param[type].scale;
    }
    void EnableFaceParam(ALMFaceShapeType type)
    {
        faceshape_param[type].enable = 1;
    }
    void DisableFaceParam(ALMFaceShapeType type)
    {
        faceshape_param[type].enable = 0;
    }
    
    void SetDefaultFaceShapeParams();
    
    /// @brief 人脸美型主接口
    /// @param[in] input_pts 输入的人脸关键点，280点集合，[x,y,z,...]，z为关键点可见度
    /// @param[out] positionArray 美型前的3D坐标插值后返回的2D坐标点
    /// @param[out] coordArray 美型后的3D坐标插值后返回的2D坐标点
    /// @param[out] indexArray 三角网格顶点索引值
    /// @param[in]  rotate 图像翻转类型 pixelai_rotate_type
    void FaceBeautyInterface(const std::vector<std::array<float, 2> >& input_pts,std::vector<float>& positionArray,std::vector<float> &coordArray,std::vector<unsigned short> &indexArray, int width = 720, int height = 1280, int faceCount = 1, int rotate = PIXELAI_CLOCKWISE_ROTATE_0);

    /// ******************** 人脸美型Anchor主程序接口 ********************
    /// @brief 人脸美型Anchor主接口
    /// @param[in] input_pts 输入的人脸关键点，106个点集合  归一化到（0到1 的坐标值）
    /// @param[out] orgArr  美型前的3D坐标值
    /// @param[out] dstArr  美型后的3D坐标值
    /// @return 成功返回PIXELAI_OK， 失败返回其他错误码
    void FaceBeautyAnchorInterface(const std::vector<std::array<float, 2> >& input_pts,std::vector<float>& orgArr,std::vector<float> &dstArr, int width = 720, int height = 1280, int faceCount = 1);
    
    // 旧版渲染需要暴露接口
    Mat posepram;
    std::vector<cv::Mat> faceshape_coefs;
    std::vector<int> faceshape_coefs_valid;
private:
    Mat poseplocal;
    Mat s;
    Mat SS_;  //人脸3D点
    Mat MeanVector2 ;
    Mat Variation;
    Mat Rotation_matrix;
    vector<vector<float>> face_center;
    
private:
    FaceShapeParameter faceshape_param[ALMFaceShapeTypMAX];
    pixelai_istream_func istream_func;
    pixelai_ostream_func ostream_func;
};




#endif
