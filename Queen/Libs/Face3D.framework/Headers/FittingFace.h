////
////  FittingFace.cpp
////  Face AR
////
////  Created by shiwei zhou on 2018/1/23.
////  Copyright © 2018年 shiwei zhou. All rights reserved.
////
//
//#include <stdio.h>
//#include <opencv2/core/core.hpp>
//using namespace std;
//
//std::vector<float> FittingFace(cv::Mat &mean,cv::Mat &basis_face,std::vector<cv::Point2f> &landmarks,std::vector<int> &mapping_3d,cv::Mat &affine_matrix)
//{
//
//    int num=(int)landmarks.size();
//    int num_coeffs_to_fit=5;
//
//    //    cv::Mat A(3*num,num_coeffs_to_fit,CV_32FC1,cv::Scalar(0));
//    //    cv::Mat current_hat_h = cv::Mat::zeros(4, num_coeffs_to_fit, CV_32FC1);
//    //    for (int i=0; i<mapping_3d.size(); i++) {
//    //
//    //        int index=(int)mapping_3d.at(i)*3;
//    //        //        cv::Mat vertex_basis=blenshape.rowRange(index, index+3);
//    //        cv::Mat vertex_basis=basis_face(cv::Rect(0,index,num_coeffs_to_fit,3));
//    //        vertex_basis.copyTo(current_hat_h.rowRange(0,3));
//    //        cv::Mat B=affine_matrix*current_hat_h;
//    //        cv::Mat roi=A(cv::Rect(0,i*3,num_coeffs_to_fit,3));
//    //        B.copyTo(roi);
//    //
//    //    }
//
//    cv::Mat V_hat_h = cv::Mat::zeros(4 *num, num_coeffs_to_fit, CV_32FC1);
//    int rowIndex=0;
//    for (int i=0; i<mapping_3d.size(); i++) {
//        int index=(int)mapping_3d.at(i)*3;
//        //  cout<<"index :"<<index<<endl;
//        cv::Mat vertex_basis=basis_face.rowRange(index, index+3);
//        vertex_basis.colRange(0,num_coeffs_to_fit).copyTo(V_hat_h.rowRange(rowIndex, rowIndex+3));
//        rowIndex +=4;
//    }
//
//    cv::Mat camera_matrix=affine_matrix;
//
//
//    cv::Mat Project_matrix(3*num,4*num,CV_32FC1,cv::Scalar(0));
//    for (int i = 0; i < num; ++i) {
//        cv::Mat submatrix_to_replace = Project_matrix.colRange(4 * i, (4 * i) + 4).rowRange(3 * i, (3 * i) + 3);
//        camera_matrix.copyTo(submatrix_to_replace);
//    }
//
//    float sigma_squared_2D = std::pow((std::sqrt(3.0f)), 2) + std::pow((0.0f), 2);
//    cv::Mat Omega = cv::Mat::zeros(3 * num, 3 * num, CV_32FC1);
//    for (int i = 0; i < 3 * num; ++i) {
//        Omega.at<float>(i, i) = 1.0f / sigma_squared_2D;
//    }
//
//    cv::Mat y = cv::Mat::ones(3 * num, 1, CV_32FC1);
//    for (int i = 0; i < num; ++i) {
//        y.at<float>(3 * i, 0) = landmarks.at(i).x;
//        y.at<float>((3 * i) + 1, 0) = landmarks.at(i).y;
//    }
//
//
//    cv::Mat v_bar = cv::Mat::ones(4 * num, 1, CV_32FC1);
//    for (int i = 0; i < num; ++i) {
//        int index=(int)mapping_3d.at(i)*3;
//        cv::Vec4f model_mean(mean.at<float>(index+0,0), mean.at<float>(index + 1,0), mean.at<float>(index + 2,0), 1.0f);
//        v_bar.at<float>(4 * i, 0) = model_mean[0];
//        v_bar.at<float>((4 * i) + 1, 0) = model_mean[1];
//        v_bar.at<float>((4 * i) + 2, 0) = model_mean[2];
//
//    }
//
//    cv::Mat A = Project_matrix * V_hat_h;
//    cv::Mat b = Project_matrix * v_bar - y;
//
//    const int num_shape_pc = num_coeffs_to_fit;
//    float lambda=50.0f;
//    cv::Mat AtOmegaA = A.t() * Omega * A;
//    cv::Mat AtOmegaAReg = AtOmegaA + lambda * cv::Mat::eye(num_shape_pc, num_shape_pc, CV_32FC1);
//    cv::Mat c_s;
//    cv::solve(AtOmegaAReg, -A.t() * Omega.t() * b, c_s, cv::DECOMP_SVD);
//    return std::vector<float>(c_s);
//}
//
