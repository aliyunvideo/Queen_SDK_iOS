//
//  MobileHumanAction.h
//
//  Created by jink on 2017/12/29.
//  Copyright © 2017年 Lin Feng. All rights reserved.
//

#ifndef _Mobile_DL_FACE_H_
#define _Mobile_DL_FACE_H_

#include <stdio.h>
#include "dl_face_common.h"

/******************************************************************************/
/********************************* Part I *************************************/
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
//////////////////// Face Detection and Face Alignment /////////////////////////
////////////////////////////////////////////////////////////////////////////////

///*************************** 初始化 config 配置选项*********************************
/// 检测模式
#define FACE_DETECT_MODE_VIDEO             0x10000000  // video
#define FACE_DETECT_MODE_IMAGE             0x20000000  // image

/// 网络模型
#define FACE_DETECT_NETWORK_HBN            0x00000001  // HBN
#define FACE_DETECT_NETWORK_FASTERRCNN     0x00000002  // Faster RCNN


///*************************** 检测结果 detect_config 配置选项*********************************
typedef enum {
    MOBILE_FACE_DETECT       = 1,        ///< 人脸检测+106点关键点检测
    MOBILE_EYE_BLINK         = 1<<1,     ///< 眨眼
    MOBILE_MOUTH_AH          = 1<<2,     ///< 嘴巴大张
    MOBILE_HEAD_YAW          = 1<<3,     ///< 摇头
    MOBILE_HEAD_PITCH        = 1<<4,     ///< 点头
    MOBILE_BROW_JUMP         = 1<<5,     ///< 眉毛挑动
    MOBILE_FACE_DETECT_VC    = 1<<6,     ///< 视频编码专用接口
	MOBILE_EXTRA_FACE_LANDMARK = 1<<7,   ///< 高精度人脸关键点 （耗时较长）
    MOBILE_FACE_EYEBALL_LANDMARK  = 1<<8,   ///< 眼球轮廓点
	
    // 人脸属性模块
    FACE_ATTRIBUTE_AGE       = 1<<9,      ///< 年龄
    FACE_ATTRIBUTE_GENDER    = 1<<10,     ///< 性别
    FACE_ATTRIBUTE_BEAUTY    = 1<<11,     ///< 漂亮指数
    FACE_ATTRIBUTE_EMOTION   = 1<<12,     ///< 表情
    FACE_ATTRIBUTE_GLASSES   = 1<<13      ///< 眼镜
}MNNFaceConfig;
///*************************** 人脸检测阈值调整函数 *********************************
/// @brief face param 人脸参数类型，目前共有12个参数阈值
typedef enum {
    FACE_PARAM_DETECT_INTERVAL = 1,  /// 人脸检测的帧率（默认值30，即每隔30帧检测一次）
    FACE_PARAM_SMOOTH_THRESHOLD = 2, /// 人脸关键点平滑系数（默认值0.25）.
    FACE_PARAM_POSE_SMOOTH_THRESHOLD = 4, /// 姿态平衡系数(0,1], 越大平滑成都越大
    FACE_PARAM_DETECT_THRESHOLD = 5, /// 人脸检测阈值(0,1), 阈值越大，误检越少，但漏检测会增加, default 0.95 for faster rcnn; default 0.3 for SSD
    // 脸部动作
    FACE_ACTION_EYE_BLINK = 6,  ///< 眨眼
    FACE_ACTION_MOUTH_AH = 7, ///< 嘴巴大张
    FACE_ACTION_HEAD_YAW = 8, ///< 摇头
    FACE_ACTION_HEAD_PITCH = 9,  ///< 点头
    FACE_ACTION_BROW_JUMP = 10,  ///< 挑眉
    
    //
    FACE_PARAM_ALIGNMENT_INTERVAL = 11, /// 人脸检测对齐间隔，默认1，一般不要超过5
    FACE_PARAM_MAX_FACE_SUPPORT = 12, /// 最多支持检出的人脸个数，最大设为32, 主要针对faster rcnn
    FACE_PARAM_DETECT_IMG_SIZE = 13,  /// 人脸检测输入的图像大小，default： 320 for faster rcnn, recommend set 320 for tiny face detecton. recomand value(tiny->large): 320, 480, 640, 720, 880, 960, 1280
} face_param_type;

#ifdef __cplusplus

///*************************** 人脸初始化函数*********************************
/// @brief create handle for face processing
/// @param[in] det_paraPath path
/// @param[in] config video: FACE_DETECT_MODE_VIDEO, picture: FACE_DETECT_MODE_IMAGE
/// @parma[out] handle, return NULL, if failed
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_face_create(
                               const char *det_paraPath,
                               const char *pts_paraPath,
                               unsigned int config,
                               handle_t *handle
                               );

/// @brief load  240 facial points model
/// @param[in] handle face handle created by mobile_dl_face_create
/// @param[in] modelPath model path
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_add_submodel(handle_t handle, const char *modelPath);

/// @brief load  eyeball points model
/// @param[in] handle face handle created by mobile_dl_face_create
/// @param[in] modelPath model path
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_add_eyeballmodel(handle_t handle, const char *modelPath);

/// @brief face detetection and facial information, Android interface
/// @param[in] handle with initialed
/// @param[in] image input image
/// @param[in] pixel_format support type BGR、RGBA、RGB、Y(GRAY)，推荐使用RGBA
/// @param[in] image_width width
/// @param[in] image_height height
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位),即每行的字节数, 默认情况下设为 0
/// @param[in] image_angle  rotate image to frontalization for face detection
/// @param[in] detect_config MOBILE_FACE_DETECT, or MOBILE_FACE_DETECT|MOBILE_EYE_BLINK et.al
/// @param[in] output_correct_angle result process rotate specific angle first, angle =  0/90/180/270
/// @param[in] output_flip_axis flip x/y 0(no flip)/1(flip X axis)/2(flip Y axis)
/// @param[out] p_face_info store face detetion result
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_face_detect(
                               handle_t handle,
                               const unsigned char *image,
                               tb_pixel_format pixel_format,
                               int image_width,
                               int image_height,
                               int image_stride,
                               size_t image_angle,
                               unsigned long long detect_config,
                               size_t output_correct_angle,
                               size_t output_flip_axis,
                               mobile_face_info_t *p_face_info
                               );



/// @brief  更改face_param_type定义的参数信息
/// @param[in] handle with initialed
/// @param[in] type face_param_type
/// @param[in] value new threshold
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_face_setparam(
                                 handle_t handle,
                                 face_param_type type,
                                 float value
                                 );


/// @brief  针对video模式下，清空之前跟踪的缓存信息
/// @param[in] handle with initialed
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_face_reset_tracking(handle_t handle);


/// @brief  对人脸检测结果进行旋转，翻转等操作
/// @param[in] handle with initialed
/// @param[in] p_face_info store face detetion result
/// @param[in] width, according to landmarks coordinate
/// @param[in] height, according to landmarks coordinate
/// @param[in] angle result process rotate specific angle first, angle =  0/90/180/270
/// @param[in] flip_axis flip x/y 0(no flip)/1(flip X axis)/2(flip Y axis)
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_face_warp_result(handle_t handle, mobile_face_info_t *&p_face_info, int width, int height, int angle, int flip_axis);


///*************************** 资源销毁释放函数 *********************************
/// @brief destroy face detector handle
/// @param[in] handle to destroy
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_face_destroy(handle_t handle);


/******************************************************************************/
/********************************* Part II ************************************/
/******************************************************************************/
/////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// face attribute ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

///*************************** 人脸属性初始化函数*********************************
/// @brief create handle for face attribute
/// @param[in] modelPath path
/// @parma[out] handle, return NULL, if failed
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_face_attribute_create(
                                         const char *modelPath,
                                         handle_t *handle
                                         );


///*************************** 人脸属性调用函数 *********************************
/// @brief face attribute prediction interface
/// @param[in] handle with initialed
/// @param[in] image input image
/// @param[in] pixel_format support type BGR、RGBA、RGB、Y(GRAY)，推荐使用RGBA
/// @param[in] image_width width
/// @param[in] image_height height
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位),即每行的字节数, 默认情况下设为 0
/// @param[in] config  e.g, FACE_ATTRIBUTE_AGE|FACE_ATTRIBUTE_GENDER|FACE_ATTRIBUTE_BEAUTY|FACE_ATTRIBUTE_EMOTION
/// @param[in] p_face_info  face detecion result
/// @param[out] p_attributes_array  stroe face attribute result
__attribute__ ((visibility("default"))) result_t mobile_dl_face_attribute_detect(
                                         handle_t handle,
                                         const unsigned char *image,
                                         tb_pixel_format pixel_format,
                                         int image_width,
                                         int image_height,
                                         int image_stride,
                                         unsigned long long config,
                                         mobile_face_info_t *p_face_info,
                                         p_face_attributes_t *p_attributes_array
                                         );

///*************************** 资源销毁释放函数 *********************************
/// @brief destroy face detector handle
/// @param[in] handle to destroy
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_face_attribute_destroy(handle_t handle);


/******************************************************************************/
/******************************** Part III ************************************/
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// face smile //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

///*************************** 笑脸分类初始化函数*********************************
/// @brief create handle for face attribute
/// @param[in] modelPath
/// @parma[out] handle, return NULL, if failed
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_face_smile_create(
                                     const char *modelPath,
                                     handle_t *handle
                                     );

///*************************** 笑脸分类调用函数 *********************************
/// @brief face smile classificaiton function
/// @param[in] handle with initialed
/// @param[in] image input image
/// @param[in] pixel_format support type BGR、RGBA、RGB、Y(GRAY)，推荐使用Y(GRAY)
/// @param[in] image_width width
/// @param[in] image_height height
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位),即每行的字节数, 默认情况下设为 0
/// @param[in] p_faces face_106_t face detecion result
/// @param[out] smile face_smile_t stroe predict result
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_face_smile_detect(
                                     handle_t handle,
                                     const unsigned char *image,
                                     tb_pixel_format pixel_format,
                                     int image_width,
                                     int image_height,
                                     int image_stride,
                                     face_106_t *p_faces,
                                     face_smile_t *smile
                                     );
__attribute__ ((visibility("default"))) result_t mobile_dl_face_smile_detect(
                                     handle_t facehandle,
                                     handle_t smilehandle,
                                     const unsigned char *image,
                                     tb_pixel_format pixel_format,
                                     int image_width,
                                     int image_height,
                                     int image_stride,
                                     size_t image_angle,
                                     unsigned long long detect_config,
                                     mobile_face_info_t *p_face_info,
                                     face_smile_t *smile
                                     );

///*************************** 笑脸阈值修改函数 *********************************
/// @brief face smile set threshold function
/// @param[in] handle with initialed
/// @param[in] value newthreshold
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_face_smile_set_threshold(handle_t handle, float value);

///*************************** 资源销毁释放函数 *********************************
/// @brief destroy face smile handle
/// @param[in] handle to destroy
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_face_smile_destroy(handle_t handle);

/******************************************************************************/
/******************************** Part IV ************************************/
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// face 3d //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

///*************************** 3d初始化函数*********************************
/// @brief create handle for face attribute
/// @param[in] modelPath
/// @param[in] 3d modelPath, mean shape, PCA conponets
/// @parma[out] handle, return NULL, if failed
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_face3d_create(
                                     const char *modelPath,
                                     const char *tb3dmmPath,
                                     handle_t *handle
                                     );
///*************************** face 3d调用函数 *********************************
/// @brief face 3d detection function
/// @param[in] handle with initialed
/// @param[in] image input image
/// @param[in] pixel_format support type BGR、RGBA、RGB、Y(GRAY)，推荐使用Y(GRAY)
/// @param[in] image_width width
/// @param[in] image_height height
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位),即每行的字节数, 默认情况下设为 0
/// @param[in] p_faces face_106_t face detecion result
/// @param[out] param_3dmm param, 62 dim, pos: 0~11 shape: 12~51 exp:52:61
/// @param[out] face_vertex stroe face vertex result, format x y z x y z...
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_face3d_detect(
                                  handle_t handle,
                                  const unsigned char *image,
                                  tb_pixel_format pixel_format,
                                  int image_width,
                                  int image_height,
                                  int image_stride,
                                  face_106_t *p_faces,
                                  float *&param_3dmm,
                                  float *&face_vertex
                                  );

///*************************** 获取face 3d模型参数信息 *********************************
/// @brief create handle for face attribute
/// @param[in] handle
/// @parma[out] vertex_num
/// @parma[out] tri_num
/// @parma[out] tri_vec, size(tri_num*3)
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_get_face3d_params(
                                 handle_t handle,
                                 int &vertex_num,
                                 int &tri_num,
                                 int *&tri_vec,
                                 int &kpt_3d_num,
                                 int *&kpt_3d_idxs
                                 );

///*************************** 资源销毁释放函数 *********************************
/// @brief destroy face 3d handle
/// @param[in] handle to destroy
/// @return result_t
__attribute__ ((visibility("default"))) result_t mobile_dl_face3d_destroy(handle_t handle);



/********************* time func ******************************/
double getCurrentMSTime();

#endif

#endif /* MobileHumanAction_h */

