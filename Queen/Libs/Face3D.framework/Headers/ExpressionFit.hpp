////
////  ExpressionFit.cpp
////  Face AR
////
////  Created by shiwei zhou on 2018/1/31.
////  Copyright © 2018年 shiwei zhou. All rights reserved.
////
//
//#include <stdio.h>
//#include <Eigen/Eigen>
//#ifndef __ANDROID__
//#include "nnls.h"
//#endif
//#include <opencv2/core/core.hpp>
//
//
//using namespace std;
//using namespace cv;
//auto FitExpressionBlendshape(cv::Mat &currentShape,cv::Mat &blenshape,std::vector<cv::Point2f> &landmarks,std::vector<int> &mapping_3d,cv::Mat &affine_matrix)
//{
//
//    int num=(int)landmarks.size();
//    int num_coeffs_to_fit=(int)blenshape.cols;
//    cv::Mat A(3*num,num_coeffs_to_fit,CV_32FC1,Scalar(0));
//    //    cv::Mat current_hat_h = cv::Mat::zeros(4, num_coeffs_to_fit, CV_32FC1);
//    cv::Mat affine_matrix_ROI=affine_matrix(cv::Rect(0,0,3,3));
//    for (int i=0; i<mapping_3d.size(); i++) {
//
//        int index=(int)mapping_3d.at(i)*3;
//        //        cv::Mat vertex_basis=blenshape.rowRange(index, index+3);
//        cv::Mat vertex_basis=blenshape(cv::Rect(0,index,num_coeffs_to_fit,3));
//        //        vertex_basis.copyTo(current_hat_h.rowRange(0,3));
//        Mat B=affine_matrix_ROI*vertex_basis;
//        Mat roi=A(cv::Rect(0,i*3,num_coeffs_to_fit,3));
//        B.copyTo(roi);
//    }
//
//    //    cv::Mat V_hat_h = cv::Mat::zeros(4 *num, num_coeffs_to_fit, CV_32FC1);
//    //    int rowIndex=0;
//    //    for (int i=0; i<mapping_3d.size(); i++) {
//    //        int index=(int)mapping_3d.at(i)*3;
//    //        cv::Mat vertex_basis=blenshape.rowRange(index, index+3);
//    //        vertex_basis.colRange(0,num_coeffs_to_fit).copyTo(V_hat_h.rowRange(rowIndex, rowIndex+3));
//    //        rowIndex +=4;
//    //    }
//
//    //    cv::Mat camera_matrix=affine_matrix;
//    ////  affine_matrix.convertTo(camera_matrix,CV_32F);
//    //
//    //    cv::Mat Project_matrix(3*num,4*num,CV_32FC1,Scalar(0));
//    //    for (int i = 0; i < num; ++i) {
//    //        Mat submatrix_to_replace = Project_matrix.colRange(4 * i, (4 * i) + 4).rowRange(3 * i, (3 * i) + 3);
//    //        camera_matrix.copyTo(submatrix_to_replace);
//    //    }
//
//    cv::Mat y = Mat::ones(3 * num, 1, CV_32FC1);
//    for (int i = 0; i < num; ++i) {
//        y.at<float>(3 * i, 0) = landmarks.at(i).x;
//        y.at<float>((3 * i) + 1, 0) = landmarks.at(i).y;
//    }
//    cv::Mat AffineMat= Mat::ones(3*num,1,CV_32FC1);
//    cv::Mat cur_v_bar = Mat::ones(4, 1, CV_32FC1);
//    for (int i = 0; i < num; ++i) {
//
//        int index=(int)mapping_3d.at(i)*3;
//        cv::Mat cur_num_roi=currentShape(cv::Rect(0,index,1,3));
//        cur_num_roi.copyTo(cur_v_bar(cv::Rect(0,0,1,3)));
//        Mat tranform=affine_matrix*cur_v_bar;
//        tranform.copyTo(AffineMat(cv::Rect(0,i*3,1,3)));
//
//
//    }
//
//    //    cv::Mat v_bar = Mat::ones(4 * num, 1, CV_32FC1);
//    //    for (int i = 0; i < num; ++i) {
//    //        int index=(int)mapping_3d.at(i)*3;
//    //        cv::Vec4f model_mean(currentShape.at<float>(index+0,0), currentShape.at<float>(index + 1,0), currentShape.at<float>(index + 2,0), 1.0f);
//    //        v_bar.at<float>(4 * i, 0) = model_mean[0];
//    //        v_bar.at<float>((4 * i) + 1, 0) = model_mean[1];
//    //        v_bar.at<float>((4 * i) + 2, 0) = model_mean[2];
//    //        //  v_bar.at<float>((4 * i) + 3, 0) = 1;
//    //        //    cout<<mean.at<float>(index+0,0)<<"  "<< mean.at<float>(index + 1,0)<<"   "<<mean.at<float>(index + 2,0)<<endl;
//    //    }
//
//    //  cv::Mat A = Project_matrix * V_hat_h;
//    //    cv::Mat b = Project_matrix * v_bar - y;
//    cv::Mat b = AffineMat - y;
//
//
//
//    //    // Solve using NNLS:
//    using RowMajorMatrixXf = Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
//    Eigen::Map<RowMajorMatrixXf> A_Eigen(A.ptr<float>(), A.rows, A.cols);
//    Eigen::Map<RowMajorMatrixXf> b_Eigen(b.ptr<float>(), b.rows, b.cols);
//
//    // Meathod one
//    //#ifndef __ANDROID__
//    //    Eigen::VectorXf x;
//    //    Eigen::NNLS<Eigen::MatrixXf>::solve(A_Eigen, -b_Eigen, x);
//    //    cv::Mat c_s((int)x.rows(), (int)x.cols(), CV_32FC1, x.data());
//    //    return std::vector<float>(c_s);
//    //#else
//
//    // Meathod two
//    int num_blendshapes=num_coeffs_to_fit;
//    float lambda =200.0f;
//    const Eigen::MatrixXf AtAReg =
//    A_Eigen.transpose() * A_Eigen + lambda * Eigen::MatrixXf::Identity(num_blendshapes, num_blendshapes);
//    const Eigen::MatrixXf rhs = -A_Eigen.transpose() * b_Eigen;
//    Eigen::VectorXf coefficients = AtAReg.colPivHouseholderQr().solve(rhs);
//    return std::vector<float>(coefficients.data(), coefficients.data() + coefficients.size());
//
//    // Meathod three
//    //    const int num_shape_pc = num_coeffs_to_fit;
//    //    float lambda=15.0f;//50
//    //    Mat AtOmegaA = A.t() * Omega * A;
//    //    Mat AtOmegaAReg = AtOmegaA + lambda * Mat::eye(num_shape_pc, num_shape_pc, CV_32FC1);
//    //    Mat c_s;
//    //    bool non_singular = cv::solve(AtOmegaAReg, -A.t() * Omega.t() * b, c_s, cv::DECOMP_SVD);
//    //    return std::vector<float>(c_s);
//    // #endif
//
//}
//
