//
//  PortraitBeauty.h
//
//  Created by limengyao on 19-11-12.
//  Copyright (c) 2019年 limengyao. All rights reserved.
//

#ifndef PortraitMakeup_h
#define PortraitMakeup_h

#include <iostream>
#include <string.h>
#include <vector>
#include "pixelai_face3D.h"

#define MAKEUP_POSE_DIM 2
#define OFFSET 3
#define RECT_PTS_NUM 8
using namespace std;

class MakeupParameter{
    
public:
    ALMMakeupType makeupType;
    std::string resName;
    int res_num;
    int rect_num;
    int pts_part_num;
    std::vector<std::array<float, 2> > resSize, offset;
    std::vector<std::vector<int> > pts_idx;
    std::vector<std::vector<int> > rect_idx;
    std::vector<std::vector<float> > rect_arr;
    std::vector<float> std_pts_arr;
    std::vector<unsigned short> idxArr;
};


class PortraitMakeup{
    
private:

public:
    PortraitMakeup();
    ~PortraitMakeup();
    
public:
    bool InitData(const ALMMakeupType type, const std::string& path = "",
                  const pixelai_istream_func& istream_func = nullptr);
    
    bool setMakeupType(const ALMMakeupType type);
    
    /// ******************** PixelAI 美妆接口 ********************
    /// @brief 人脸美妆主接口
    /// @param[in]  input_pts     输入的人脸关键点，280个点集合  归一化到（0到1 的坐标值）
    /// @param[out] positionArray 美妆的位置坐标点, [x0,y0,z0,...xn,yn,zn]
    /// @param[out] coordArray    美妆的纹理坐标点, [[x0,y0,z0,...xn,yn,zn]
    /// @param[out] indexArray    美妆的三角网格顶点索引值
    /// @param[in]  rotate        图像翻转类型
    /// @param[in]  faceCnt       美妆人脸数量
    /// @return 成功返回PIXELAI_OK， 失败返回其他错误码
    void FaceMakeupInterface(   const std::vector<std::array<float, 3> > &input_pts,
                                std::vector<std::vector<float> > pose,
                                std::vector<float>& positionArray,
                                std::vector<float> &coordArray,
                                std::vector<unsigned short> &indexArray,
                                std::vector<float> &bboxArray,
                                int faceCount,
                                int rotate=PIXELAI_CLOCKWISE_ROTATE_0
                            );
private:
    void FaceMakeup2D(   const std::vector<std::array<float, 3> > &input_pts,
                         std::vector<std::vector<float> > pose,
                         std::vector<float>& positionArray,
                         std::vector<float> &coordArray,
                         std::vector<unsigned short> &indexArray,
                         std::vector<float> &bboxArray,
                         int faceCount,
                         int rotate=PIXELAI_CLOCKWISE_ROTATE_0
                     );
    void FaceMakeup3D(   const std::vector<std::array<float, 3> > &input_pts,
                         std::vector<float>& positionArray,
                         std::vector<float> &coordArray,
                         std::vector<unsigned short> &indexArray,
                         int faceCount
                     );
    ALMBlendType getBlendType(const ALMMakeupType type);
    float getVisibility(const std::vector<pixelai_pointf_t> &input_pts,
                        pixelai_pointf_t &position);
    void getExtPointsVisibility(const std::vector<pixelai_point3df_t> &input_pts,
                        std::vector<float> &vis);
    bool readParams(const std::string &stdPtsPath, const std::string &indexPath, const std::string &rectPath);
    pixelai_istream_func istream_func;
    pixelai_ostream_func ostream_func;
    MakeupParameter _makeupParam;
    std::vector<MakeupParameter> _makeupParamArr;
    ALMMakeupType makeupType;
};




#endif
