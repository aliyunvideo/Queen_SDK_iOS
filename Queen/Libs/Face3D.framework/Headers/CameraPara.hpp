////
////  CameraPara.cpp
////
////  Created by shiwei zhou on 2018/1/25.
////  Copyright © 2018年 shiwei zhou. All rights reserved.
////
//
//#include <stdio.h>
//#include "opencv2/core/core.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtc/quaternion.hpp"
//#include <string>
//#include <fstream>
//#include <stdexcept>
//using namespace std;
//struct RotationProjectionParameters {
//    glm::mat3x3 R;
//    double tx, ty;
//    double s;
//};
//
//struct Frustum
//{
//    //    float l, r, b, t;
//    Frustum() {};
//    Frustum(float l, float r, float b, float t) : l(l), r(r), b(b), t(t) {};
//    //Frustum(float l, float r, float b, float t, float n, float f) : l(l), r(r), b(b), t(t), n(n), f(f) {};
//    float l = -1.0f;
//    float r = 1.0f;
//    float b = -1.0f;
//    float t = 1.0f;
//};
//
//enum class CameraType
//{
//    Orthographic,
//    Perspective
//};
//
//
//class FacePoseParameters
//{
//public:
//
//    FacePoseParameters() {};
//    FacePoseParameters(RotationProjectionParameters ortho_params, int screen_width, int screen_height) : camera_type(CameraType::Orthographic), t_x(ortho_params.tx), t_y(ortho_params.ty), screen_width(screen_width), screen_height(screen_height) {
//        rotation = glm::quat(ortho_params.R);
//        const auto l = 0.0;
//        const auto r = screen_width / ortho_params.s;
//        const auto b = 0.0;
//        const auto t = screen_height / ortho_params.s;
//        frustum = Frustum(l, r, b, t);
//    };
//
//    auto get_camera_type() const {
//        return camera_type;
//    };
//
//    glm::quat get_rotation() const {
//        return rotation;
//    };
//
//    glm::mat4x4 get_modelview() const {
//        glm::mat4x4 modelview = glm::mat4_cast(rotation);
//
//        modelview[3][0] = t_x;
//        modelview[3][1] = t_y;
//        return modelview;
//    };
//
//    glm::mat4x4 get_projection() const {
//        if (camera_type == CameraType::Orthographic)
//        {
//            return glm::ortho<float>(frustum.l, frustum.r, frustum.b, frustum.t);
//        }
////        else {
////            throw std::runtime_error("get_projection() for CameraType::Perspective is not implemented yet.");
////        }
//    };
//
//    Frustum get_frustum() const {
//        return frustum;
//    };
//
//    int get_screen_width() const {
//        return screen_width;
//    };
//
//    int get_screen_height() const {
//        return screen_height;
//    };
//
//private:
//    CameraType camera_type = CameraType::Orthographic;
//    Frustum frustum;
//
//    glm::quat rotation;
//
//    float t_x;
//    float t_y;
//
//
//    int screen_width;
//    int screen_height;
//
//};
//
//
//inline glm::vec4 get_opencv_viewport(int width, int height)
//{
//    return glm::vec4(0, height, width, -height);
//};
//
//
//inline cv::Mat to_mat(const glm::mat4x4& glm_matrix)
//{
//
//    auto glm_matrix_t = glm::transpose(glm_matrix);
//    cv::Mat opencv_mat(4, 4, CV_32FC1, &glm_matrix_t[0]);
//
//    return opencv_mat.clone();
//};
//
//cv::Mat get_3x4_affine_camera_matrix(FacePoseParameters params, int width, int height)
//{
//    auto view_model = to_mat(params.get_modelview());
//    auto ortho_projection = to_mat(params.get_projection());
//    cv::Mat mvp = ortho_projection * view_model;
//    //            cout<<"view_model "<<view_model<<endl;
//    //            cout<<"ortho_projection "<<ortho_projection<<endl;
//
//    glm::vec4 viewport(0, height, width, -height);
//
//    cv::Mat viewport_mat = (cv::Mat_<float>(4, 4) << viewport[2] / 2.0f, 0.0f,       0.0f, viewport[2] / 2.0f + viewport[0],
//                            0.0f,               viewport[3] / 2.0f, 0.0f, viewport[3] / 2.0f + viewport[1],
//                            0.0f,               0.0f,               1.0f, 0.0f,
//                            0.0f,               0.0f,               0.0f, 1.0f);
//
//    cv::Mat full_projection_4x4 = viewport_mat * mvp;
//    cv::Mat full_projection_3x4 = full_projection_4x4.rowRange(0, 3); // we take the first 3 rows, but then set the last one to [0 0 0 1]
//    full_projection_3x4.at<float>(2, 0) = 0.0f;
//    full_projection_3x4.at<float>(2, 1) = 0.0f;
//    full_projection_3x4.at<float>(2, 2) = 0.0f;
//    full_projection_3x4.at<float>(2, 3) = 1.0f;
//
//    return full_projection_3x4;
//};
//
//
