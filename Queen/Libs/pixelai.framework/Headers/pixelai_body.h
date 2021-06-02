#ifndef INCLUDE_PIXELAI_BODY_H_
#define INCLUDE_PIXELAI_BODY_H_

#include <stdio.h>
#include "pixelai_common.hpp"

#define PIXELAI_MAX_BODY 4
#define PIXELAI_BODY_KEYPOINT_NUM 14
#define PIXELAI_BODY_LIMB_NUM 13

const int PIXELAI_BODY_LIMB_IDX[PIXELAI_BODY_LIMB_NUM][2] =
{
    {1,  2},
    {1,  5},
    {2,  3},
    {3,  4},
    {5,  6},
    {6,  7},
    {1,  8},
    {8,  9},
    {9,  10},
    {1,  11},
    {11, 12},
    {12, 13},
    {1,  0}
};

/// ******************** PixelAI 人体姿态识别配置选项 ********************
#define PIXELAI_BODY_KEYPOINT_MODE_IMAGE        0x00000001  ///< 人体姿态识别image模式
//#define PIXELAI_BODY_KEYPOINT_MODE_VIDEO      0x00000002  ///< 人体姿态识别video模式(待实现)

#define PIXELAI_BODY_KEYPOINT_SIN_PERSON        0x00000100  ///< 单人姿态关键点识别功能
#define PIXELAI_BODY_KEYPOINT_MUL_PERSON        0x00000200  ///< 多人姿态关键点识别功能

/// ******************** PixelAI 人体姿态识别数据结构定义 ********************
/// @brief 肢体检测结果
typedef struct pixelai_body_t {
    int id;                                 ///< 肢体 id
    pixelai_pointf_t *p_key_points;         ///< 肢体关键点
    float *p_key_points_score;              ///< 肢体关键点的置信度[0,1]
    int key_points_count;                   ///< 肢体关键点个数 目前为14
} pixelai_body_t, *p_pixelai_body_t;

/// @brief body pose estimation result
typedef struct pixelai_body_info_t {
    pixelai_body_t *p_bodys;         ///< 检测到的人体的信息 按面积从大到小排序
    int body_count;                  ///< 检测到的人体的数目
} pixelai_body_info_t, *p_pixelai_body_info_t;


/// ******************** PixelAI 人体姿态识别函数接口 ********************
/// @brief create handle for body pose processing
/// @param[in] model_path for body pose estimation
/// @param[in] config, video: PIXELAI_BODY_KEYPOINT_MODE_VIDEO, picture: PIXELAI_BODY_KEYPOINT_MODE_IMAGE
/// @parma[out] handle, return NULL, if failed
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t 
pixelai_dl_body_create(
    const char *est_netPath,
    unsigned int config,
    pixelai_handle_t *handle
);

/// @brief body pose estimation
/// @param[in] handle with initialed
/// @param[in] input image
/// @param[in] image_width
/// @param[in] image_height
/// @param[in] image_angle
/// @param[out] body pose estimation result
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t 
pixelai_dl_body_detect(
    pixelai_handle_t handle,
    const unsigned char *image,
    pixelai_pixel_format pixel_format,
    int image_width,
    int image_height,
    int image_angle,
    unsigned int detect_config,
    pixelai_body_info_t *p_body_info
);

/// @brief destroy body pose handle
/// @param[in] handle to destroy
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t 
pixelai_dl_body_destroy(
  pixelai_handle_t handle
);

#endif
