#ifndef INCLUDE_PIXELAI_FACE_H_
#define INCLUDE_PIXELAI_FACE_H_

#include <stdio.h>
#include "pixelai_common.h"

#define PIXELAI_MAX_FACE 32

/// ******************** PixelAI 人脸检测配置选项 ********************
#define PIXELAI_FACE_DETECT                     0x00000001  ///< 人脸检测
#define PIXELAI_EYE_BLINK                       0x00000002  ///< 眨眼
#define PIXELAI_MOUTH_AH                        0x00000004  ///< 嘴巴大张
#define PIXELAI_HEAD_YAW                        0x00000008  ///< 摇头
#define PIXELAI_HEAD_PITCH                      0x00000010  ///< 点头
#define PIXELAI_BROW_JUMP                       0x00000020  ///< 眉毛挑动
#define PIXELAI_EXTRA_FACE_LANDMARK             0x00000040  ///< 高精度人脸关键点（耗时较长）
#define PIXELAI_FACE_ATTRIBUTE_AGE              0x00000080  ///< 年龄
#define PIXELAI_FACE_ATTRIBUTE_GENDER           0x00000100  ///< 性别
#define PIXELAI_FACE_ATTRIBUTE_BEAUTY           0x00000200  ///< 漂亮指数
#define PIXELAI_FACE_ATTRIBUTE_EMOTION          0x00000400  ///< 表情

#define PIXELAI_FACE_DETECT_MODE_VIDEO          0x00010000  ///< 人脸检测video模式
#define PIXELAI_FACE_DETECT_MODE_IMAGE          0x00020000  ///< 人脸检测image模式

#define PIXELAI_FACE_DETECT_NETWORK_HBN         0x10000000  ///< 人脸检测网络模型HBN
#define PIXELAI_FACE_DETECT_NETWORK_FASTERRCNN  0x20000000  ///< 人脸检测网络模型Faster RCNN


/// ******************** PixelAI 人脸检测数据结构定义 ********************
typedef struct pixelai_face_rects_t {
    p_pixelai_rect_t rects;
    float* scores; //confidence of face
    int face_count;
} pixelai_face_rects_t, *p_pixelai_face_rects_t;

/// @brief the information of 106 facial landmarks
typedef struct pixelai_face_106_t {
    pixelai_rect_t rect;                         // face rectangle
    float score;                                 // confidence
    pixelai_pointf_t landmarks_array[106];       // 106 facial points
    float landmarks_visible_array[106];          // visibility of 106 facail points
    pixelai_pointf_t *p_extra_face_landmarks;    ///< 眼睛、眉毛、嘴唇关键点. 没有检测到时为NULL
    int extra_face_landmarks_count;              ///< 眼睛、眉毛、嘴唇关键点个数. 检测到时为134,
    float yaw;                                   // left: + ; right: -
    float pitch;                                 // down:-; up:+;
    float roll;                                  // left: - ; right: +
    //    float eye_dist;                            // pupillary distance
    unsigned int face_action;                    // face action
    int faceID;
} pixelai_face_106_t, *p_pixelai_face_106_t;

/// @brief face detection result
typedef struct pixelai_face_info_t {
    pixelai_face_106_t *p_faces;         //face info
    int face_count;                      //face deteciton num
} pixelai_face_info_t, *p_pixelai_face_info_t;

/// @brief face param 人脸参数类型，目前共有12个参数阈值
typedef enum {
    PIXELAI_FACE_PARAM_DETECT_INTERVAL = 1,  /// 人脸检测的帧率（默认值10，即每隔10帧检测一次）
    PIXELAI_FACE_PARAM_SMOOTH_THRESHOLD = 2, /// 人脸关键点平滑系数（默认值0.25）.
    PIXELAI_FACE_PARAM_POSE_SMOOTH_THRESHOLD = 4, /// 姿态平衡系数(0,1], 越小平滑成都越大
    PIXELAI_FACE_PARAM_DETECT_THRESHOLD = 5, /// 人脸检测阈值(0,1), 阈值越大，误检越少，但漏检测会增加, default 0.95 for faster rcnn; default 0.3 for SSD
    PIXELAI_FACE_PARAM_ALIGNMENT_INTERVAL = 6, /// 人脸检测对齐间隔，默认1，一般不要超过5
    PIXELAI_FACE_PARAM_MAX_FACE_SUPPORT = 7, /// 最多支持检出的人脸个数，最大设为32,默认为10
    
    // 设置脸部动作阈值
    PIXELAI_FACE_ACTION_EYE_BLINK = 8,  ///< 眨眼
    PIXELAI_FACE_ACTION_MOUTH_AH = 9, ///< 嘴巴大张
    PIXELAI_FACE_ACTION_HEAD_YAW = 10, ///< 摇头
    PIXELAI_FACE_ACTION_HEAD_PITCH = 11,  ///< 点头
    PIXELAI_FACE_ACTION_BROW_JUMP = 12,  ///< 挑眉
} pixelai_face_param_type;


/// ******************** PixelAI 人脸属性数据结构定义 ********************

#ifndef PIXELAI_MAX_ATTR_STRING_LEN
#define PIXELAI_MAX_ATTR_STRING_LEN 32
#endif
/// @brief 单个属性
typedef struct pixelai_face_attribute_t {
    char category[PIXELAI_MAX_ATTR_STRING_LEN];   /// 属性描述, 例如 "age", "gender"
    char label[PIXELAI_MAX_ATTR_STRING_LEN];      /// 属性标签描述, 例如 "18", "male" or "female"
    float score;                                 /// 该属性标签的置信度
} pixelai_face_attribute_t;

/// @brief 一个人脸的所有属性
typedef struct pixelai_face_attributes_t {
    pixelai_face_attribute_t *p_attributes;    /// 属性数组
    int attribute_count;               /// 属性个数
} pixelai_face_attributes_t, *p_pixelai_face_attributes_t;


/// ******************** PixelAI 笑脸分类模块数据结构定义 ********************
typedef enum {
    PIXELAI_FACE_SMILE_NEUTRAL = 0,                   ///< 自然表情
    PIXELAI_FACE_SMILE_LAUGH = 1,                     ///< 正面大笑
    PIXELAI_FACE_SMILE_LEFT_LAUGH = 2,                ///< 朝左大笑
    PIXELAI_FACE_SMILE_RIGHT_LAUGH =3,                ///< 朝右大笑
    PIXELAI_FACE_SMILE_SMILLING = 4,                  ///< 正面微笑
    PIXELAI_FACE_SMILE_UP_SMILLING = 5,               ///< 朝上微笑
    PIXELAI_FACE_SMILE_DOWN_SMILLING =6,              ///< 朝下微笑
    PIXELAI_FACE_SMILE_LEFT_STICKING_TONGUE = 7,      ///< 左吐舌笑
    PIXELAI_FACE_SMILE_RIGHT_STICKING_TONGUE =8,      ///< 右吐舌笑
    PIXELAI_FACE_SMILE_LEFT_THROW_KISS = 9,           ///< 左飞吻
    PIXELAI_FACE_SMILE_RIGHT_THROW_KISS = 10,         ///< 右飞吻
    PIXELAI_FACE_SMILE_LEFT_TWITCH_MOUTH = 11,        ///< 左憋嘴
    PIXELAI_FACE_SMILE_RIGHT_TWITCH_MOUTH = 12,       ///< 右憋嘴
} pixelai_face_smile_type;

typedef struct pixelai_ace_smile_t {
    pixelai_face_smile_type smile_type;              ///< 笑脸类型
    float score;                                     ///< 分数
} pixelai_face_smile_t, *p_pixelai_face_smile_t;


#ifdef __cplusplus
/// ******************** PixelAI 人脸初始化函数 ********************
/// @brief create handle for face processing
/// @param[in] det_paraPath path
/// @param[in] config video: PIXELAI_FACE_DETECT_MODE_VIDEO, picture: PIXELAI_FACE_DETECT_MODE_IMAGE
/// @parma[out] handle, return NULL, if failed
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
pixelai_result_t
pixelai_dl_face_create(
                       const char *det_paraPath,
                       const char *pts_paraPath,
                       unsigned int config,
                       pixelai_handle_t *handle
                       );

/// @brief load  240 facial points model
/// @param[in] handle, face handle created by mobile_dl_face_create
/// @param[in] modelPath, model path
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
pixelai_result_t
pixelai_dl_add_submodel(
                        pixelai_handle_t handle,
                        const char *modelPath
                        );


/// ******************** PixelAI 人脸检测函数 (iOS) ********************
/// @brief face detetection and facial information, iOS interface
/// @param[in] handle with initialed
/// @param[in] image input image
/// @param[in] pixel_format support type BGR、RGBA、RGB、Y(GRAY)，推荐使用RGBA
/// @param[in] image_width width
/// @param[in] image_height height
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位),即每行的字节数, 默认情况下设为 0
/// @param[in] image_angle  rotate image to frontalization for face detection
/// @param[in] detect_config MOBILE_FACE_DETECT, or MOBILE_FACE_DETECT|MOBILE_EYE_BLINK et.al
/// @param[out] p_face_info store face detetion result
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
pixelai_result_t
pixelai_dl_face_detect(
                       pixelai_handle_t handle,
                       const unsigned char *image,
                       pixelai_pixel_format pixel_format,
                       int image_width,
                       int image_height,
                       int image_stride,
                       size_t image_angle,
                       unsigned long long detect_config,
                       pixelai_face_info_t *p_face_info
                       );

/// @brief face detetection and facial information, iOS interface
/// @param[in] handle with initialed
/// @param[in] image input image
/// @param[in] pixel_format support type BGR、RGBA、RGB、Y(GRAY)，推荐使用RGBA
/// @param[in] image_width width
/// @param[in] image_height height
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位),即每行的字节数, 默认情况下设为 0
/// @param[in] image_angle  rotate image to frontalization for face detection
/// @param[in] detect_config MOBILE_FACE_DETECT, or MOBILE_FACE_DETECT|MOBILE_EYE_BLINK et.al
/// @param[in] faceMonitor face detection monitor pointer
/// @param[in] alignMonitor face alignment monitor pointer
/// @param[out] p_face_info store face detetion result
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
pixelai_result_t
pixelai_dl_face_detect(
                       pixelai_handle_t handle,
                       const unsigned char *image,
                       pixelai_pixel_format pixel_format,
                       int image_width,
                       int image_height,
                       int image_stride,
                       size_t image_angle,
                       unsigned long long detect_config,
                       void *faceMonitor,
                       void *alignMonitor,
                       pixelai_face_info_t *p_face_info
                       );


/// ******************** PixelAI 人脸检测函数 (Android) ********************
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
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
pixelai_result_t
pixelai_dl_face_detect(
                       pixelai_handle_t handle,
                       const unsigned char *image,
                       pixelai_pixel_format pixel_format,
                       int image_width,
                       int image_height,
                       int image_stride,
                       size_t image_angle,
                       unsigned long long detect_config,
                       size_t output_correct_angle,
                       size_t output_flip_axis,
                       pixelai_face_info_t *p_face_info
                       );

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
/// @param[in] faceMonitor face detection monitor pointer
/// @param[in] alignMonitor face alignment monitor pointer
/// @param[out] p_face_info store face detetion result
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
pixelai_result_t
pixelai_dl_face_detect(
                       pixelai_handle_t handle,
                       const unsigned char *image,
                       pixelai_pixel_format pixel_format,
                       int image_width,
                       int image_height,
                       int image_stride,
                       size_t image_angle,
                       unsigned long long detect_config,
                       size_t output_correct_angle,
                       size_t output_flip_axis,
                       void *faceMonitor,
                       void *alignMonitor,
                       pixelai_face_info_t *p_face_info
                       );


/// ******************** PixelAI 人脸检测阈值调整函数 ********************
/// @brief  更改face_param_type定义的参数信息
/// @param[in] handle with initialed
/// @param[in] type face_param_type
/// @param[in] value new threshold
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
pixelai_result_t
pixelai_dl_face_setparam(
                         pixelai_handle_t handle,
                         pixelai_face_param_type type,
                         float value
                         );


/// ******************** PixelAI 人脸检测资源销毁释放函数 ********************
/// @brief destroy face detector handle
/// @param[in] handle to destroy
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
pixelai_result_t
pixelai_dl_face_destroy(
                        pixelai_handle_t handle
                        );


/// ******************** PixelAI 人脸属性初始化函数 ********************
/// @brief create handle for face attribute
/// @param[in] modelPath path
/// @parma[out] handle, return NULL, if failed
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
pixelai_result_t
pixelai_dl_face_attribute_create(
                                 const char *modelPath,
                                 pixelai_handle_t *handle
                                 );


/// ******************** PixelAI 人脸属性调用函数 ********************
/// @brief face attribute prediction interface
/// @param[in] handle with initialed
/// @param[in] image input image
/// @param[in] pixel_format support type BGR、RGBA、RGB、Y(GRAY)，推荐使用RGBA
/// @param[in] image_width width
/// @param[in] image_height height
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位),即每行的字节数, 默认情况下设为 0
/// @param[in] config, e.g, PIXELAI_FACE_ATTRIBUTE_AGE|PIXELAI_FACE_ATTRIBUTE_GENDER|PIXELAI_FACE_ATTRIBUTE_BEAUTY|PIXELAI_FACE_ATTRIBUTE_EMOTION
/// @param[in] p_face_info, face detecion result
/// @param[out] p_attributes_array, stroe face attribute result
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
pixelai_result_t
pixelai_dl_face_attribute_detect(
                                 pixelai_handle_t handle,
                                 const unsigned char *image,
                                 pixelai_pixel_format pixel_format,
                                 int image_width,
                                 int image_height,
                                 int image_stride,
                                 unsigned long long config,
                                 pixelai_face_info_t *p_face_info,
                                 p_pixelai_face_attributes_t *p_attributes_array
                                 );

pixelai_result_t
pixelai_dl_face_attribute_detect(
                                 pixelai_handle_t handle,
                                 const unsigned char *image,
                                 pixelai_pixel_format pixel_format,
                                 int image_width,
                                 int image_height,
                                 int image_stride,
                                 unsigned long long config,
                                 pixelai_face_info_t *p_face_info,
                                 p_pixelai_face_attributes_t *p_attributes_array,
                                 void *attrMonitor
                                 );


/// ******************** PixelAI 人脸属性资源销毁释放函数 ********************
/// @brief destroy face detector handle
/// @param[in] handle to destroy
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
pixelai_result_t
pixelai_dl_face_attribute_destroy(
                                  pixelai_handle_t handle
                                  );


/// ******************** PixelAI 笑脸分类初始化函数 ********************
/// @brief create handle for face attribute
/// @param[in] modelPath
/// @parma[out] handle, return NULL, if failed
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
pixelai_result_t
pixelai_dl_face_smile_create(
                             const char *modelPath,
                             pixelai_handle_t *handle
                             );


/// ******************** PixelAI 笑脸分类调用函数 ********************
/// @brief face smile classificaiton function
/// @param[in] handle with initialed
/// @param[in] image input image
/// @param[in] pixel_format support type BGR、RGBA、RGB、Y(GRAY)，推荐使用Y(GRAY)
/// @param[in] image_width width
/// @param[in] image_height height
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位),即每行的字节数, 默认情况下设为 0
/// @param[in] p_faces face_106_t face detecion result
/// @param[out] smile face_smile_t stroe predict result
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
pixelai_result_t
pixelai_dl_face_smile_detect(
                             pixelai_handle_t handle,
                             const unsigned char *image,
                             pixelai_pixel_format pixel_format,
                             int image_width,
                             int image_height,
                             int image_stride,
                             pixelai_face_106_t *p_faces,
                             pixelai_face_smile_t *smile
                             );

pixelai_result_t
pixelai_dl_face_smile_detect(
                             pixelai_handle_t handle,
                             const unsigned char *image,
                             pixelai_pixel_format pixel_format,
                             int image_width,
                             int image_height,
                             int image_stride,
                             pixelai_face_106_t *p_faces,
                             pixelai_face_smile_t *smile,
                             void *smileMonitor
                             );

pixelai_result_t
pixelai_dl_face_smile_detect(
                             pixelai_handle_t facehandle,
                             pixelai_handle_t smilehandle,
                             const unsigned char *image,
                             pixelai_pixel_format pixel_format,
                             int image_width,
                             int image_height,
                             int image_stride,
                             size_t image_angle,
                             unsigned long long detect_config,
                             pixelai_face_info_t *p_face_info,
                             pixelai_face_smile_t *smile,
                             void *faceMonitor,
                             void *alignMonitor,
                             void *smileMonitor
                             );


/// ******************** PixelAI 笑脸阈值修改函数 ********************
/// @brief face smile set threshold function
/// @param[in] handle with initialed
/// @param[in] value newthreshold
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
pixelai_result_t
pixelai_dl_face_smile_set_threshold(
                                    pixelai_handle_t handle,
                                    float value
                                    );


/// ******************** PixelAI 笑脸资源销毁释放函数 ********************
/// @brief destroy face detector handle
/// @param[in] handle to destroy
/// @return 成功返回PIXELAI_OK， 失败返回其他错误码
pixelai_result_t
pixelai_dl_face_smile_destroy(
                              pixelai_handle_t handle
                              );


/// ******************** PixelAI 其他工具函数 ********************
double getCurrentMSTime();

#endif

#endif
