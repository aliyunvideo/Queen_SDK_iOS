#ifndef INCLUDE_PIXELAI_SEGMENT_H_
#define INCLUDE_PIXELAI_SEGMENT_H_

#include <stdio.h>
#include "pixelai_common.hpp"

/// ******************** PixelAI 人体分割配置选项 ********************
#define PIXELAI_SEGMENT_PORTRAIT                0x00000001  ///< 人像分割功能
#define PIXELAI_SEGMENT_HAIR                    0x00000002  ///< 头发分割功能
#define PIXELAI_SEGMENT_NAIL                    0x00000004  ///< 指甲分割功能

#define PIXELAI_SEGMENT_MODE_VIDEO_FAST         0x00010000  ///< video fast模式
#define PIXELAI_SEGMENT_MODE_VIDEO_REFINE       0x00020000  ///< video refine模式
#define PIXELAI_SEGMENT_MODE_IMAGE              0x00040000  ///< image模式


/// ******************** PixelAI 分割模型输出尺寸 ********************
// Portrait
#define VIDEO_FAST_OUTPUT_WIDTH_PORTRAIT 225
#define VIDEO_FAST_OUTPUT_HEIGHT_PORTRAIT 225
#define VIDEO_REFINE_OUTPUT_WIDTH_PORTRAIT 720
#define VIDEO_REFINE_OUTPUT_HEIGHT_PORTRAIT 1280
#define IMAGE_OUTPUT_WIDTH_PORTRAIT 720
#define IMAGE_OUTPUT_HEIGHT_PORTRAIT 1280
// Hair
#define VIDEO_FAST_OUTPUT_WIDTH_HAIR 225
#define VIDEO_FAST_OUTPUT_HEIGHT_HAIR 225
#define VIDEO_REFINE_OUTPUT_WIDTH_HAIR 720
#define VIDEO_REFINE_OUTPUT_HEIGHT_HAIR 1280
#define IMAGE_OUTPUT_WIDTH_HAIR 720
#define IMAGE_OUTPUT_HEIGHT_HAIR 1280
// Nail
#define VIDEO_FAST_OUTPUT_WIDTH_NAIL 320
#define VIDEO_FAST_OUTPUT_HEIGHT_NAIL 640
#define VIDEO_REFINE_OUTPUT_WIDTH_NAIL 720
#define VIDEO_REFINE_OUTPUT_HEIGHT_NAIL 1280
#define IMAGE_OUTPUT_WIDTH_NAIL 720
#define IMAGE_OUTPUT_HEIGHT_NAIL 1280

/// ******************** PixelAI 人体分割数据结构定义 ********************
typedef struct pixelai_segment_info_t {
    pixelai_image_t *p_output_frame;  ///< 与分割结果对应的原始图像
    pixelai_image_t *p_mask;  ///< mask信息
    float avg_hair_color[3];  ///< 头发分割mask区域内的平均颜色
} pixelai_segment_info_t, *p_pixelai_segment_info_t;


/// ******************** PixelAI 分割接口 ********************
/// @brief 初始化SDK
/// @param[in] target - 分割目标，0 - portrait, 1 - hair, 2 - nail
/// @param[in] model_path - 模型路径
/// @param[out] handle - SDK对象指针
/// @return 成功返回PIXELAI_OK， 失败返回PIXELAI_ERROR_HANDLE
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_segment_create(
                          int target,
                          const char *model_path,
                          pixelai_handle_t *handle
                          );

/// @brief 调用SDK，得到分割结果
/// @param[in] handle - SDK对象指针
/// @param[in] srcimg_data - 原始图像数据，必须是BGRA格式
/// @param[in] srcimg_pixel_format - 原始图像的数据类型
/// @param[in] srcimg_cols - 原始图像宽
/// @param[in] srcimg_rows - 原始图像高
/// @param[in] srcimg_angle - 原始图像角度（逆时针）
/// @param[in] detect_config - 指定分割目标和模式
/// @param[out] detect_result_info - 分割结果
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_segment_detect(
                          pixelai_handle_t handle,
                          unsigned char *srcimg_data,
                          pixelai_pixel_format srcimg_pixel_format,
                          int srcimg_cols,
                          int srcimg_rows,
                          size_t srcimg_angle,
                          int mask_cols,
                          int mask_rows,
                          unsigned int detect_config,
                          int mnn_stride,
                          pixelai_segment_info_t &detect_result_info
                          );

/// @brief SDK对象销毁
/// @param[in] handle - SDK对象指针
/// @param[in] target - 分割目标，0 - portrait, 1 - hair, 2 - nail
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_segment_destroy(
                           pixelai_handle_t handle,
                           int target
                           );

#endif
