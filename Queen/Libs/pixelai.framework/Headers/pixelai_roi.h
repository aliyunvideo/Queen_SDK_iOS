//
//  pixelai_roi.h
//  pixelai_roi_mac_demo
//
//  Created by Bo Gao on 2020/2/20.
//  Copyright © 2020年 Bo Gao. All rights reserved.
//

#ifndef pixelai_roi_h
#define pixelai_roi_h

#include "pixelai_common.hpp"

#define PIXELAI_MAX_ROI 32


/// ******************** PixelAI ROI检测配置选项 ********************
#define PIXELAI_ROI_DETECT_MODE_VIDEO          0x00000000100  ///< ROI检测video模式
#define PIXELAI_ROI_DETECT_MODE_IMAGE          0x00000000200  ///< ROI检测image模式


/// ******************** PixelAI ROI检测数据结构定义 ********************
typedef struct pixelai_roi_rects_t {
    p_pixelai_rect_t rects;
    float* scores; //confidence of roi presence
    int roi_count;
} pixelai_roi_rects_t, *p_pixelai_roi_rects_t;

/// @brief ROI检测结果
typedef struct pixelai_roi_t {
    int id;                                 ///< id
    pixelai_rect_t rect;                    ///< 矩形框
    float det_score;                        ///< 检测置信度
} pixelai_roi_t, *p_pixelai_roi_t;

/// @brief ROI检测结果
typedef struct pixelai_roi_info_t {
    pixelai_roi_t *p_rois;                 ///< 检测到的ROI的信息
    int roi_count;                         ///< 检测到的ROI的数目
} pixelai_roi_info_t, *p_pixelai_roi_info_t;


///*************************** ROI检测阈值调整函数 *********************************
/// @brief roi param roi参数类型
typedef enum {
    PIXELAI_ROI_PARAM_DETECT_INTERVAL = 1,  /// ROI检测的帧率（默认值10，即每隔10帧检测一次）
    PIXELAI_ROI_PARAM_DETECT_THRESHOLD = 2, /// ROI检测阈值(0,1), 阈值越大，误检越少，但漏检测会增加, default 0.90
    PIXELAI_ROI_PARAM_ROI_AREA_RATIO = 3    /// ROI面积占比
} roi_param_type;


/// ******************** PixelAI ROI检测函数接口 ********************
/// @brief create handle for roi processing
/// @param[in] det_netPath for detection
/// @parma[out] handle, return NULL, if failed
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_roi_create(
                      const char *det_netPath,
                      unsigned int config,
                      pixelai_handle_t *handle
                      );

/// @brief roi detetection
/// @param[in] handle with initialed
/// @param[in] image :
/// @param[in] image_width :
/// @param[in] image_height :
/// @param[in] image_stride :
/// @param[in] image_angle :
/// @param[out] p_roi_info roi detetion result
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_roi_detect(
                      pixelai_handle_t handle,
                      const unsigned char *image,
                      pixelai_pixel_format pixel_format,
                      int image_width,
                      int image_height,
                      int image_stride,
                      size_t image_angle,
                      pixelai_roi_info_t *p_roi_info
                      );

/// @brief  更改roi_param_type定义的参数信息
/// @param[in] handle with initialed
/// @param[in] type roi_param_type
/// @param[in] value new threshold
/// @return result_t
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_roi_setparam(
                        pixelai_handle_t handle,
                        roi_param_type type,
                        float value
                        );

/// @brief destroy roi handle
/// @param[in] handle to destroy
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_roi_destroy(
                       pixelai_handle_t handle
                       );

#endif /* pixelai_roi_h */
