#ifndef INCLUDE_PIXELAI_HAND_H_
#define INCLUDE_PIXELAI_HAND_H_

#include <stdio.h>
#include "pixelai_common.hpp"

#define PIXELAI_MAX_HAND 32

/// ******************** PixelAI 手势识别配置选项 ********************
/// 功能配置选项
#define PIXELAI_HAND_DETECT                     0x00000000001  ///< 手部检测功能
#define PIXELAI_HAND_STATIC_GESTURE             0x00000000002  ///< 静态手势识别功能
#define PIXELAI_HAND_MOTION_GESTURE             0x00000000004  ///< 动态手势识别功能
#define PIXELAI_HAND_2D_KEYPOINT                0x00000000008  ///< 手部2D关键点定位功能
#define PIXELAI_HAND_3D_KEYPOINT                0x00000000010  ///< 手部3D关键点定位功能(TODO)

#define PIXELAI_HAND_PHONE_GESTURE              0x00000000020  ///< 手机交互手势识别功能
#define PIXELAI_HAND_SILENCE_GESTURE            0x00000000040  ///< 安静手势识别功能
#define PIXELAI_HAND_HELLO_GESTURE              0x00000000080  ///< 静态Hello手势识别功能

#define PIXELAI_HAND_DETECT_MODE_IMAGE          0x00000000100  ///< 手势检测image模式
#define PIXELAI_HAND_DETECT_MODE_VIDEO          0x00000000200  ///< 手势检测video模式

///// 静态手势种类
typedef enum {
    /// 单个手势
    PIXELAI_HAND_UNKNOWN =              0,          ///< 未知手势
    PIXELAI_HAND_BLUR =                 1,          ///< 模糊手势
    PIXELAI_HAND_OK =                   2,          ///< OK手势
    PIXELAI_HAND_PALM =                 3,          ///< 数字5/手掌手势
    PIXELAI_HAND_FINGER =               4,          ///< 数字1/食指手势
    PIXELAI_HAND_NUM8 =                 5,          ///< 数字8/手枪手势
    PIXELAI_HAND_HEART =                6,          ///< 单手比心手势
    PIXELAI_HAND_FIST =                 7,          ///< 拳头手势
    PIXELAI_HAND_HOLDUP =               8,          ///< 托举手势
    PIXELAI_HAND_CONGRATULATE =         9,          ///< 抱拳作揖/拜托手势
    PIXELAI_HAND_YEAH =                 10,         ///< 数字2/Yeah/剪刀手手势
    PIXELAI_HAND_LOVE =                 11,         ///< 双手爱心手势
    PIXELAI_HAND_GOOD =                 12,         ///< 点赞/拇指向上手势
    PIXELAI_HAND_ROCK =                 13,         ///< 摇滚手势
    PIXELAI_HAND_NUM3 =                 14,         ///< 数字3手势
    PIXELAI_HAND_NUM4 =                 15,         ///< 数字4手势
    PIXELAI_HAND_NUM6 =                 16,         ///< 数字6手势
    PIXELAI_HAND_NUM7 =                 17,         ///< 数字7手势
    PIXELAI_HAND_NUM9 =                 18,         ///< 数字9手势
    PIXELAI_HAND_GREETING =             19,         ///< 拜年/恭贺手势
    PIXELAI_HAND_PRAY =                 20,         ///< 祈祷手势
    PIXELAI_HAND_THUMBS_DOWN =          21,         ///< 拇指向下手势
    PIXELAI_HAND_THUMBS_LEFT =          22,         ///< 拇指向左手势
    PIXELAI_HAND_THUMBS_RIGHT =         23,         ///< 拇指向右手势
    /// 组合手势
    PIXELAI_HAND_HELLO =                24,         ///< 双手Hello手势
    /// 人脸手势
    PIXELAI_HAND_SILENCE =              25,         ///< 安静手势

} hand_static_action_type;

/// 动态手势种类
typedef enum {
    PIXELAI_HAND_MOTION_UNKNOWN =               0,          ///< 未知动态手势
    PIXELAI_HAND_MOTION_SWIPE_UP =              1,          ///< 向上划手势
    PIXELAI_HAND_MOTION_SWIPE_DOWN =            2,          ///< 向下划手势
    PIXELAI_HAND_MOTION_SWIPE_LEFT =            3,          ///< 向左划手势
    PIXELAI_HAND_MOTION_SWIPE_RIGHT =           4,          ///< 向右划手势
    PIXELAI_HAND_MOTION_FINGER_SWIPE_UP =       5,          ///< 单指上划手势(TODO)
    PIXELAI_HAND_MOTION_FINGER_SWIPE_DOWN =     6,          ///< 单指下划手势(TODO)
    PIXELAI_HAND_MOTION_FINGER_SWIPE_LEFT =     7,          ///< 单指左划手势(TODO)
    PIXELAI_HAND_MOTION_FINGER_SWIPE_RIGHT =    8,          ///< 单指右划手势(TODO)
    PIXELAI_HAND_MOTION_PALMS_OPEN =            9,          ///< 双手打开手势(TODO)
    PIXELAI_HAND_MOTION_PALMS_CLOSE =           10,         ///< 双手闭合手势(TODO)
    PIXELAI_HAND_MOTION_WAVING =                11,         ///< 双手挥手手势(TODO)
    PIXELAI_HAND_MOTION_PALM2FIST =             12,         ///< 单手由掌变拳手势(TODO)
} hand_motion_action_type;

/// 手机交互手势
typedef enum {
    PIXELAI_HAND_PHONE_UNKNOWN =        0,          ///< 手持手机未知手势
    PIXELAI_HAND_PHONE_SCROLL =         1,          ///< 手持手机翻页手势
    PIXELAI_HAND_PHONE_SIGN =           2,          ///< 手持手机签字手势
} hand_phone_action_type;

/// 手部关键点配置信息
#define PIXELAI_HAND_KEYPOINT_NUM 21
#define PIXELAI_HAND_LIMB_NUM 20

const int PIXELAI_HAND_LIMB_IDX[PIXELAI_HAND_LIMB_NUM][2] =
{
    {0,  1},
    {1,  2},
    {2,  3},
    {3,  4},
    {0,  5},
    {5,  6},
    {6,  7},
    {7,  8},
    {0,  9},
    {9,  10},
    {10, 11},
    {11, 12},
    {0,  13},
    {13, 14},
    {14, 15},
    {15, 16},
    {0,  17},
    {17, 18},
    {18, 19},
    {19, 20}
};


/// ******************** PixelAI 手势识别数据结构定义 ********************
typedef struct pixelai_common_rects_t {
    p_pixelai_rect_t rects;
    float* scores;                                  /// 通用物体检测置信度
    int count;
} pixelai_common_rects_t, *p_pixelai_common_rects_t;

/// @brief 手势检测结果
typedef struct pixelai_hand_t {
    int id;                                         ///< 手的id
    pixelai_rect_t rect;                            ///< 手部矩形框
    pixelai_pointf_t p_key_points[21];              ///< 手部关键点
    float p_key_points_score[21];                   ///< 手部关键点置信度[0,1]
    hand_static_action_type hand_static_action;     ///< 手部静态动作
    hand_motion_action_type hand_motion_action;     ///< 手部动态动作
    float det_score;                                ///< 手部检测置信度[0,1]
    float static_score;                             ///< 手部静态动作置信度[0,1]
    float motion_score;                             ///< 手部动态动作置信度[0,1]
    
    bool phone_touched;                             ///< 是否手持手机
    float phone_touched_score;                      ///< 手持手机置信度[0,1]
    pixelai_pointf_t p_phone_touched_point;         ///< 手部和手机接触点
    hand_phone_action_type hand_phone_action;       ///< 手持手机动作
} pixelai_hand_t, *p_pixelai_hand_t;

/// @brief hand detection result
typedef struct pixelai_hand_info_t {
    pixelai_hand_t *p_hands = NULL;                 ///< 检测到的手的信息
    int hand_count;                                 ///< 检测到的手的数目
} pixelai_hand_info_t, *p_pixelai_hand_info_t;


/// ******************** PixelAI 手势识别阈值调整函数 ********************
/// @brief hand param 手势参数类型
typedef enum {
    PIXELAI_HAND_PARAM_DETECT_INTERVAL = 0,  /// 手部检测的帧率（默认值2, 即每隔2帧检测一次）
    PIXELAI_HAND_PARAM_DETECT_THRESHOLD = 1,  /// 手部检测阈值[0,1], 阈值越大, 误检越少, 但漏检测会增加, default 0.95
    PIXELAI_HAND_PARAM_DETECT_IMG_SIZE = 2,  /// 手部检测输入的图像大小, default: 480, recommend set 480 for tiny hand detecton. recomand value(tiny->large): 320, 480, 640, 720, 880, 960, 1280
    PIXELAI_HAND_PARAM_CLASSIFY_THRESHOLD = 3, /// 手势识别阈值[0,1], 阈值越大, 误识别越少, 但漏识别会增加, default 0.90
    PIXELAI_HAND_PARAM_CLASSIFY_SMOOTH = 4, /// 是否开启静态手势识别结果的平滑功能, 0代表不开启, 1代表开启, default 0 不开启
    PIXELAI_HAND_PARAM_HELLO_GESTURE = 5, /// 双手Hello手势参数, 范围[0,1], 参数值越大, 误识别越少, 但漏识别会增加, default 0.5
    PIXELAI_HAND_PARAM_SILENCE_GESTURE = 6, /// 安静手势参数, 范围[0,1], 参数值越大, 误识别越少, 但漏识别会增加, default 0.25
    PIXELAI_HAND_PARAM_MOTION_GESTURE = 7, /// 动态手势参数, 范围[0,1], 参数值越大, 动态手势需要划动的范围越大, default 0.25
} hand_param_type;


/// ******************** PixelAI 手势识别函数接口 ********************
/// @brief create handle for hand processing
/// @param[in] det_netPath, hand detection model path
/// @param[in] config, video: HAND_DETECT_MODE_VIDEO, picture: HAND_DETECT_MODE_IMAGE
/// @parma[out] handle, return NULL, if failed
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_hand_create(
                       const char *det_netPath,
                       unsigned int config,
                       pixelai_handle_t *handle
                       );

/// @brief load hand gesture model
/// @param[in] handle, hand handle created by pixelai_dl_hand_create
/// @param[in] cls_netPath, hand gesture model path
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_hand_add_gesture_model(
                                  pixelai_handle_t handle,
                                  const char *cls_netPath
                                  );

/// @brief load hand keypoint model
/// @param[in] handle, hand handle created by pixelai_dl_hand_create
/// @param[in] est_netPath, hand keypoint model path
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_hand_add_keypoint_model(
                                   pixelai_handle_t handle,
                                   const char *est_netPath
                                   );

/// @brief hand detetection, gesture recognition and pose estimation
/// @param[in] handle, handle with initialed
/// @param[in] image, input image
/// @param[in] image_width
/// @param[in] image_height
/// @param[in] image_stride
/// @param[in] image_angle
/// @param[out] p_hand_info, hand detetion result
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_hand_detect(
                       pixelai_handle_t handle,
                       const unsigned char *image,
                       pixelai_pixel_format pixel_format,
                       int image_width,
                       int image_height,
                       int image_stride,
                       int image_angle,
                       unsigned int detect_config,
                       pixelai_hand_info_t *p_hand_info
                       );

/// @brief 更改hand_param_type定义的参数信息
/// @param[in] handle, handle with initialed
/// @param[in] type, hand_param_type
/// @param[in] value, new value
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_hand_setparam(
                         pixelai_handle_t handle,
                         hand_param_type type,
                         float value
                         );

/// @brief destroy hand handle
/// @param[in] handle, handle to destroy
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
PIXELAI_SDK_API pixelai_result_t
pixelai_dl_hand_destroy(
                        pixelai_handle_t handle
                        );

#endif
