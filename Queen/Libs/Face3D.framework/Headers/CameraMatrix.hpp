////
////  CameraMatrix.cpp
////
////  Created by shiwei zhou on 2018/1/24.
////  Copyright © 2018年 shiwei zhou. All rights reserved.
////
//#include "pixelai_face3D.h"
//#include "glm/mat3x3.hpp"
//#include "opencv2/core/core.hpp"
//#include <vector>
//#include <cassert>
//inline RotationProjectionParameters solveposeprojectmatrix(std::vector<cv::Vec2f> image_points, std::vector<cv::Vec4f> model_points, bool is_viewport_upsidedown, int Height)
//{
//    using cv::Mat;
//    assert(image_points.size() == model_points.size());
//    assert(image_points.size() >= 4);
//
//    const int num_correspondences = static_cast<int>(image_points.size());
//
//    if (is_viewport_upsidedown)
//    {
//        for (auto&& ip : image_points)
//        {
//            ip[1] = Height - ip[1];
//        }
//    }
//
//    Mat A = Mat::zeros(2 * num_correspondences, 8, CV_32FC1);
//    int row_index = 0;
//    for (int i = 0; i < model_points.size(); ++i)
//    {
//        Mat p = Mat(model_points[i]).t();
//        p.copyTo(A.row(row_index++).colRange(0, 4));
//        p.copyTo(A.row(row_index++).colRange(4, 8));
//    }
//
//    Mat b(2 * num_correspondences, 1, CV_32FC1);
//    row_index = 0;
//    for (int i = 0; i < image_points.size(); ++i)
//    {
//        b.at<float>(row_index++) = image_points[i][0];
//        b.at<float>(row_index++) = image_points[i][1];
//    }
//
//    Mat k; // resulting affine matrix (8x1)
//    cv::solve(A, b, k, cv::DECOMP_SVD);
//
//    const Mat R1 = k.rowRange(0, 3);
//    const Mat R2 = k.rowRange(4, 7);
//    const float sTx = k.at<float>(3);
//    const float sTy = k.at<float>(7);
//    const auto s = (cv::norm(R1) + cv::norm(R2)) / 2.0;
//    Mat r1 = R1 / cv::norm(R1);
//    Mat r2 = R2 / cv::norm(R2);
//    Mat r3 = r1.cross(r2);
//    Mat R;
//    r1 = r1.t();
//    r2 = r2.t();
//    r3 = r3.t();
//    R.push_back(r1);
//    R.push_back(r2);
//    R.push_back(r3);
//
//    Mat S, U, Vt;
//    cv::SVDecomp(R, S, U, Vt);
//    Mat R_ortho = U * Vt;
//
//    if (cv::determinant(R_ortho) < 0)
//    {
//        U.row(2) = -U.row(2);
//        R_ortho = U * Vt;
//    }
//
//    const auto t1 = sTx / s;
//    const auto t2 = sTy / s;
//
// //   std::cout<<"Parameter :"<<std::endl;
//    glm::mat3x3 R_glm; // identity
//    for (int r = 0; r < 3; ++r) {
//        for (int c = 0; c < 3; ++c) {
//            R_glm[c][r] = R_ortho.at<float>(r, c);
//     //       cout<<R_ortho.at<float>(r, c)<<" ";
//        }
//   //     cout<<std::endl;
//    }
////    cout<<"t1 :"<<t1<<std::endl;
////    cout<<"t2 :"<<t2<<std::endl;
////    cout<<"s :" <<s<<std::endl;
//
//    return RotationProjectionParameters{ R_glm, t1, t2, s };
//};
