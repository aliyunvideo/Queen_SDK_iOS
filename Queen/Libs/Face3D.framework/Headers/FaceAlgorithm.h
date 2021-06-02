////
////  FaceAlgorithm.hpp
////  Face AR
////
////  Created by shiwei zhou on 2018/2/23.
////  Copyright © 2018年 shiwei zhou. All rights reserved.
////
//
//#ifndef FaceAlgorithm_hpp
//#define FaceAlgorithm_hpp
//
//#include <stdio.h>
//#ifdef __cplusplus
//#include <opencv2/core/core.hpp>
//#endif
//#include <array>
//#include "pixelai_common.h"
//#include "pixelai_face3D.h"
//#include "pixelai_face3D_private.h"
//using namespace std;
//class FaceAlgorithm
//{
//public:
//    FaceAlgorithm();
//    ~FaceAlgorithm();
//
//public:
//    std::vector<float> ReturnPose(float pose_parameter[6],std::vector<std::array<float, 2>> &inputPts,const int width,const int height,const int iterNum=5);
//    bool InitData(const string path,const pixelai_istream_func& func = nullptr);
//    bool InitDataFaceMask(const string path);
//    std::vector<cv::Point3i> faces_vertex;  //每一维表示三个顶点的组成
//    std::vector<cv::Point3f> positions;
//    std::vector<cv::Point2f> texels; //UV坐标
//    std::vector<cv::Point2f> project_image_pts;
//    std::vector<float> fits_expression;
//private:
//    std::unique_ptr<std::istream> getiStream(const std::string& path,std::ios_base::openmode mode = std::ios_base::in);
//private:
//    std::vector<int> mapping_2d;
//    std::vector<int> mapping_3d;
//    std::vector<int> Combine_Select_Index;
//    cv::Mat FitFaceEigenValue;
//    cv::Mat mean,normalised_pca_basis,normalised_pca_basis_ROI;
//    std::vector<cv::Mat> blendshapes;
//    cv::Mat blenshape_Mat;
//    std::vector<int> left_contour_index,right_contour_index;
//    std::vector<float> fits;
//
//    cv::Mat Add_expression;
//    cv::Mat combine_shape;
//    std::vector<cv::Point3f> Init_positions;
//    float pre_pose_parameter[6];
//    cv::Mat pre_combine_shape;
//    cv::Mat cur_Facerecon;
//
//    pixelai_istream_func istream_func;
//};
//
//#endif /* FaceAlgorithm_hpp */
