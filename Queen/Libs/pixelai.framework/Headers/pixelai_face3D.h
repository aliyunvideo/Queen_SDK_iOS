#ifndef INCLUDE_PIXELAI_FACE3D_H_
#define INCLUDE_PIXELAI_FACE3D_H_

#include <stdio.h>
#include "pixelai_common.hpp"
#include "ALMFaceShapeType.h"
#include <vector>

#ifndef PI
#define PI 3.141592654
#endif

#define PIXELAI_DENSIFY_LANDMARK 0 // 是否做人脸关键点稠密

#define PIXELAI_FACE3D_LANDMARK_NUM 72
#define PIXELAI_FACE_LANDMARK106_NUM 106
#define PIXELAI_FACE_EXT_LANDMARK_NUM 134
#define PIXELAI_FACE_LANDMARK240_NUM 240

/// ******************** PixelAI 3D人脸美型初始化接口 ********************
/// @brief create handle for face 3D FaceBeauty
/// @param[in] model_Path model path for face 3D FaceBeauty
/// @parma[out] handle, return NULL, if failed
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_face3DFaceBeauty_create(pixelai_handle_t *handle,
                                   const std::string& model_Path = "",
                                   const pixelai_istream_func& func = nullptr);
/// ******************** PixelAI 3D人脸美型主程序接口 ********************
/// @brief 人脸美型主接口
/// @param[in] input_pts 输入的人脸关键点，106个点集合  归一化到（0到1 的坐标值）
/// @param[out] positionArray 美型前的3D坐标插值后返回的2D坐标点
/// @param[out] coordArray 美型后的3D坐标插值后返回的2D坐标点
/// @param[out] indexArray 三角网格顶点索引值
/// @param[in]  rotate 图像旋转类型, 支持PIXELAI_CLOCKWISE_ROTATE_0和PIXELAI_FLIP_VERTICAL两种类型
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_face3DFaceBeauty_Run(pixelai_handle_t handle,
                         const std::vector<std::array<float, 2> > &input_pts,
                         std::vector<float>& positionArray,
                         std::vector<float> &coordArray,
                         std::vector<unsigned short> &indexArray,
                         int width=720, int height=1280,
                         int faceCount= 1, int rotate=PIXELAI_CLOCKWISE_ROTATE_0
                         );

/// ******************** PixelAI 3D人脸美型参数设置接口 ********************
/// @brief 3D人脸美型参数设置接口
/// @param[in] type 人脸参数模板
/// @param[in] strength 人脸美型模板对应的参数值
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t pixelai_updateFaceShapeWithType( pixelai_handle_t handle,ALMFaceShapeType type ,float &strength);

PIXELAI_SDK_API pixelai_result_t pixelai_enableFaceShapeWithType( pixelai_handle_t handle,ALMFaceShapeType type ,bool enable);

PIXELAI_SDK_API pixelai_result_t pixelai_getFaceType( pixelai_handle_t handle, std::vector<float> feature, ALMFaceType &type);

PIXELAI_SDK_API pixelai_result_t pixelai_setFaceShapeTypeWithFaceType( pixelai_handle_t handle, ALMFaceType type);

/// ******************** PixelAI 3D人脸函数接口 ********************
/// @brief create handle for face 3D
/// @param[in] model_Path model path for face 3D
/// @parma[out] handle, return NULL, if failed
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_face3D_create(
    const char *model_Path,
    pixelai_handle_t *handle
);

/// @brief 人脸驱动
/// @param[in] handle with initialed
/// @param[in] inputPts 输入的人脸关键点信息，可以是68或者106个点集合
/// @param[in] image_width 输入图像的宽
/// @param[in] image_height 输入图像的高
/// @param[in] iterNum 设置迭代次数，迭代次数越高精度越高，反正则越低，低端机型可以适当降低迭代次数
/// @param[out] pose_parameter 返回的姿态矩阵参数 ：
///    pose_parameter[0]=pitch;
///    pose_parameter[1]=yaw;
///    pose_parameter[2]=roll;
///    pose_parameter[3]=scale;
///    pose_parameter[4]=tx;
///    pose_parameter[5]=ty;
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_face3D_detect(
    pixelai_handle_t handle,
    std::vector<std::array<float, 2> > inputPts,
    int image_width,
    int image_height,
    int iterNum,
    float *pose_parameter,
    std::vector<std::array<float, 3> > &Position
);

pixelai_result_t pixelai_dl_face3D_avatar(
                                          pixelai_handle_t handle,
                                          std::vector<std::array<float, 2> > inputPts,
                                          int image_width,
                                          int image_height,
                                          int iterNum,
                                          float *pose_parameter,
                                          std::vector<float> &fits_expression
                                          );

/// @brief face estimation for face 3D
/// @param[in] p_face 输入的人脸关键点信息，可以是68或者106个点集合
/// @parma[out] returnPose
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
//PIXELAI_SDK_API pixelai_result_t
//pixelai_dl_faceEstimation_detect(
//    std::vector<std::array<float, 2> > inputPts,
//    std::vector<float> &returnPose,
//    float *ration_parameter
//);

/// ******************** PixelAI 美妆初始化接口 ********************
/// @param[in] model_Path model path for face 3D FaceBeauty
/// @parma[out] handle, return NULL, if failed
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_face3DFaceMakeup_create(pixelai_handle_t *handle);

/// ******************** PixelAI 美妆素材读取接口 ********************
/// @param[in] model_Path model path for face 3D FaceBeauty
/// @parma[out] handle, return NULL, if failed
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_face3DFaceMakeup_setup(pixelai_handle_t handle,
                                  const ALMMakeupType type,
                                  const std::string& model_Path = "",
                                  const pixelai_istream_func& func = nullptr
                                  );

/// ******************** PixelAI 美妆接口 ********************
/// @brief 人脸美妆主接口
/// @param[in]  input_pts     输入的人脸关键点，2D为280个点集合, 3D为三维人脸坐标，屏幕坐标
/// @param[in]  type          美妆类型
/// @param[out] positionArray 美妆的位置坐标点, 屏幕坐标[x0, y0, z0, ...,xn, yn, zn]
/// @param[out] coordArray    美妆的纹理坐标点, 归一化[0,1]坐标[x_norm_0, y_norm_0, z_norm_0, ..., x_norm_n, y_norm_n, z_norm_n]
/// @param[out] indexArray    美妆的三角网格顶点索引值[t0_0, t0_1, t0_2, ..., tn_0, tn_1, tn_2]
/// @param[out] bboxArray     美妆部位的外接矩形, 屏幕坐标 [x0,y0,x1,y1,x2,y2,x3,y3]
/// @param[in]  rotate 图像旋转类型, 支持PIXELAI_CLOCKWISE_ROTATE_0和PIXELAI_FLIP_VERTICAL两种类型
/// @param[in]  faceCnt       美妆人脸数量
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码

PIXELAI_SDK_API pixelai_result_t
pixelai_dl_face3DFaceMakeup_Run( pixelai_handle_t handle,
                                 const std::vector<std::array<float, 3> > &input_pts,
                                 std::vector<std::vector<float> > pose,
                                 std::vector<float>& positionArray,
                                 std::vector<float> &coordArray,
                                 std::vector<unsigned short> &indexArray,
                                 std::vector<float> &bboxArray,
                                 int faceCnt,
                                 int rotate=PIXELAI_CLOCKWISE_ROTATE_0
                                 );

/// ******************** PixelAI MLS形变接口 ********************
/// @brief 人脸美型主接口
/// @param[in]  input_pts     原始图像中的控制点
/// @param[in]  dst_pts       形变后图像的控制点
/// @param[out] positionArray 位置坐标
/// @param[out] coordArray    纹理坐标
/// @param[out] indexArray    三角网格顶点索引值
/// @param[in]  obj           MLS变换矩阵，0:放射变换；1:相似变换；2:刚性变换
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码

PIXELAI_SDK_API pixelai_result_t
pixelai_dl_deform_MLS(
                      const std::vector<std::array<float, 2> > &input_pts,
                      const std::vector<std::array<float, 2> > &dst_pts,
                      std::vector<float>& positionArray,
                      std::vector<float> &coordArray,
                      std::vector<unsigned short> &indexArray,
                      int obj=0
                      );

/// @brief face estimation for face 3D
/// @param[in] inputPts 输入坐标点
/// @param[in] angle 旋转角度（0-360）
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_rotatePoints(std::vector<std::array<float, 2> > &inputPts,float &angle,int &cols,int &rows);

/// @brief destroy face3D handle
/// @param[in] handle to destroy
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_face3D_destroy(
    pixelai_handle_t *handle
);

/// @brief destroy faceBeauty handle
/// @param[in] handle to destroy
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_faceBeauty_destroy(
     pixelai_handle_t *handle
                              );


/// @brief destroy faceMakeup handle
/// @param[in] handle to destroy
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
pixelai_result_t pixelai_dl_faceMakeup_destroy(pixelai_handle_t *handle);


/// ******************** PixelAI 视频封面图提取始化接口 @远至********************
/// @param[in] model_Path model path for video thumbnail extract
/// @parma[out] handle, return NULL, if failed
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_videoThumbnailExtract_create(pixelai_handle_t *handle);

/// ******************** PixelAI 视频封面图提取接口 @远至 ***********************
/// @brief 人脸美妆主接口
/// @param[in]  frame_buffer  视频帧内存
/// @param[in]  width         视频宽
/// @param[in]  height        视频高
/// @param[in]  channel       视频channel
/// @param[out] score         封面图分数
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_videoThumbnailExtract_run( pixelai_handle_t handle,
								unsigned char *frame_buffer,
								int width,
								int height,
								int channel,
								float &score
								);

/// ******************** PixelAI 视频封面图提取销毁接口 @远至 **********************
/// @brief destroy video thumbnail extract handle
/// @param[in] handle to destroy
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_videoThumbnailExtract_destroy(
							  pixelai_handle_t *handle
							  );
#endif
