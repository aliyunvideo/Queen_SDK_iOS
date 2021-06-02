#ifndef INCLUDE_PIXELAI_TRACK_H_
#define INCLUDE_PIXELAI_TRACK_H_

#include <stdio.h>
#include "pixelai_common.hpp"

///*************************** 跟踪器模式 *********************************
#define PIXELAI_MODE_TRACKING               0x10000000  ///< Tracking Mode
#define PIXELAI_MODE_INIT                   0x20000000  ///< Init Mode

///*************************** 初始化 tracker 配置选项*********************************
/// 跟踪器种类
#define PIXELAI_TRACKER_KCF                 0x000000010  ///< kernerlized correlation filter Tracker
#define PIXELAI_TRACKER_DSST                0x000000011  ///< Discriminative Scale Space Tracker
#define PIXELAI_TRACKER_SAD                 0x000000012  ///< SAD Tracker


///*************************** 跟踪结果 track_config 配置选项*********************************
typedef enum {
    PIXELAI_MOBILE_TRACK_RECT             = 1,        ///< 跟踪非旋转框
    PIXELAI_MOBILE_TRACK_ROTATED_RECT     = 1<<1,     ///< 跟踪旋转框
    PIXELAI_MOBILE_TRACK_PIXEL            = 1<<2,     ///< 跟踪像素
    PIXELAI_MOBILE_TRACK_HEATMAP          = 1<<3,     ///< 跟踪热图
    PIXELAI_MOBILE_TRACK_MULTISCALE       = 1<<4,     ///< 多尺寸跟踪
}track_config;

/// @brief the information of object tracking results
typedef struct pixelai_track_info_t {
    pixelai_rect_t rect;                 // face rectangle
    float score;                         // confidence
    int pixel_width;
    int pixel_height;
    pixelai_image_t *p_output_pixel;    ///< 跟踪像素模式输出的结果，单通道
    int heatmap_width;
    int heatmap_height;
    pixelai_image_t *p_output_heatmap;  ///< 跟踪热图模式输出的结果，单通道
    float yaw;                           // left: + ; right: -
    float pitch;                         // down:-; up:+;
    float roll;                          // left: - ; right: +
    int trackID;

} pixelai_track_info_t, *p_pixelai_track_info_t;

///*************************** 跟踪初始化函数*********************************
/// @brief create handle for track processing
/// @parma[out] handle, return NULL, if failed
/// @param[in] config video: FACE_DETECT_MODE_VIDEO, picture: FACE_DETECT_MODE_IMAGE
/// @return result_t
PIXELAI_SDK_API pixelai_result_t pixelai_dl_track_create(
                               pixelai_handle_t *handle,
                               unsigned int config
                               );

/// @brief face detetection and facial information, Android interface
/// @param[in] handle with initialed
/// @param[in] image input image
/// @param[in] pixel_format support type BGR、RGBA、RGB、Y(GRAY)，推荐使用RGBA
/// @param[in] image_width width
/// @param[in] image_height height
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位),即每行的字节数, 默认情况下设为 0
/// @param[in] image_angle  rotate image to frontalization for face detection
/// @param[in] track_config
/// @param[out] p_track_info_in
/// @param[out] p_track_info_out
/// @return result_t
PIXELAI_SDK_API pixelai_result_t pixelai_dl_track_run(
                               pixelai_handle_t handle,
                               const unsigned char *image,
                               pixelai_pixel_format pixel_format,
                               int image_width,
                               int image_height,
                               int image_stride,
                               size_t image_angle,
                               unsigned long long track_config,
                               pixelai_track_info_t *p_track_info_in,
                               pixelai_track_info_t *p_track_info_out
                               );

///*************************** 资源销毁释放函数 *********************************
/// @brief destroy track handle
/// @param[in] handle to destroy
/// @return result_t
PIXELAI_SDK_API pixelai_result_t pixelai_dl_track_destroy(pixelai_handle_t handle);
#endif
