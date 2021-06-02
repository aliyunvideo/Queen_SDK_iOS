#ifndef INCLUDE_PIXELAI_FACE_H_
#define INCLUDE_PIXELAI_FACE_H_

#include <stdio.h>
#include "pixelai_common.hpp"

#define MAX_FACE 32
#define MAX_FACE_ATTRIBUTES 16

#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif


/// ******************** PixelAI 人脸检测数据结构定义 ********************
typedef struct pixelai_face_rects_t {
    p_pixelai_rect_t rects;
    float* scores; //confidence of face
    int face_count;
} pixelai_face_rects_t, *p_pixelai_face_rects_t;

/// @brief the information of 106 facial landmarks
typedef struct pixelai_face_106_t {
    pixelai_rect_t rect;                         // face rectangle
    float score;                         // confidence
    pixelai_pointf_t landmarks_array[106];       // 106 facial points
    float landmarks_visible_array[106];  // visibility of 106 facail points
    pixelai_pointf_t p_extra_face_landmarks[134];    ///< 眼睛、眉毛、嘴唇关键点. 
    int extra_face_landmarks_count;      ///< 眼睛、眉毛、嘴唇关键点个数. 检测到时为134,
    pixelai_pointf_t p_eyeball_contour[38];      ///< 眼球轮廓点
    pixelai_pointf_t p_eyeball_center[2];        ///< 眼球瞳孔中心点
    int eyeball_contour_points_count;    ///< 眼球轮廓关键点个数.
    float left_eyeball_score;           ///< 左眼球置信度: [0.0, 1.0]
    float right_eyeball_score;          ///< 右眼球置信度: [0.0, 1.0]
    float yaw;                           // left: + ; right: -
    float pitch;                         // down:-; up:+;
    float roll;                          // left: - ; right: +
//    float eye_dist;                    // pupillary distance
    unsigned int face_action;            // face action
    int faceID;

} pixelai_face_106_t, *p_pixelai_face_106_t;

/// @brief face detection result
typedef struct pixelai_face_info_t {
    pixelai_face_106_t *p_faces;         //face info
    int face_count;                      //face deteciton num
} pixelai_face_info_t, *p_pixelai_face_info_t;

///*************************** 初始化 config 配置选项*********************************
/// 检测模式
#define PIXELAI_FACE_DETECT_MODE_VIDEO             0x10000000  // video
#define PIXELAI_FACE_DETECT_MODE_IMAGE             0x20000000  // image

/// 网络模型
#define PIXELAI_FACE_DETECT_NETWORK_HBN            0x00000001  // HBN
#define PIXELAI_FACE_DETECT_NETWORK_FASTERRCNN     0x00000002  // Faster RCNN


///*************************** 检测结果 detect_config 配置选项*********************************
typedef enum {
    PIXELAI_MOBILE_FACE_DETECT       = 1,        ///< 人脸检测+106点关键点检测
    PIXELAI_MOBILE_EYE_BLINK         = 1<<1,     ///< 眨眼
    PIXELAI_MOBILE_MOUTH_AH          = 1<<2,     ///< 嘴巴大张
    PIXELAI_MOBILE_HEAD_YAW          = 1<<3,     ///< 摇头
    PIXELAI_MOBILE_HEAD_PITCH        = 1<<4,     ///< 点头
    PIXELAI_MOBILE_BROW_JUMP         = 1<<5,     ///< 眉毛挑动
    PIXELAI_MOBILE_FACE_DETECT_VC    = 1<<6,     ///< 视频编码专用接口
    PIXELAI_MOBILE_EXTRA_FACE_LANDMARK = 1<<7,   ///< 高精度人脸关键点 134 点
    PIXELAI_MOBILE_FACE_EYEBALL_LANDMARK  = 1<<8,   ///< 眼球轮廓点
    
    // 人脸属性模块
    PIXELAI_FACE_ATTRIBUTE_AGE       = 1<<9,      ///< 年龄
    PIXELAI_FACE_ATTRIBUTE_GENDER    = 1<<10,     ///< 性别
    PIXELAI_FACE_ATTRIBUTE_BEAUTY    = 1<<11,     ///< 漂亮指数
    PIXELAI_FACE_ATTRIBUTE_EMOTION   = 1<<12,     ///< 表情
    PIXELAI_FACE_ATTRIBUTE_GLASSES   = 1<<13      ///< 眼镜
}face_det_config;

///*************************** 人脸检测阈值调整函数 *********************************
/// @brief face param 人脸参数类型，目前共有12个参数阈值
typedef enum {
    PIXELAI_FACE_PARAM_DETECT_INTERVAL = 1,  /// 人脸检测的帧率（默认值30，即每隔30帧检测一次）
    PIXELAI_FACE_PARAM_SMOOTH_THRESHOLD = 2, /// 人脸关键点平滑系数（默认值0.25）.
    PIXELAI_FACE_PARAM_POSE_SMOOTH_THRESHOLD = 4, /// 姿态平衡系数(0,1], 越大平滑成都越大
    PIXELAI_FACE_PARAM_DETECT_THRESHOLD = 5, /// 人脸检测阈值(0,1), 阈值越大，误检越少，但漏检测会增加, default 0.95 for faster rcnn; default 0.3 for SSD
    // 脸部动作
    PIXELAI_FACE_ACTION_EYE_BLINK = 6,  ///< 眨眼
    PIXELAI_FACE_ACTION_MOUTH_AH = 7, ///< 嘴巴大张
    PIXELAI_FACE_ACTION_HEAD_YAW = 8, ///< 摇头
    PIXELAI_FACE_ACTION_HEAD_PITCH = 9,  ///< 点头
    PIXELAI_FACE_ACTION_BROW_JUMP = 10,  ///< 挑眉
    
    //
    PIXELAI_FACE_PARAM_ALIGNMENT_INTERVAL = 11, /// 人脸检测对齐间隔，默认1，一般不要超过5
    PIXELAI_FACE_PARAM_MAX_FACE_SUPPORT = 12, /// 最多支持检出的人脸个数，最大设为32, 主要针对faster rcnn
    PIXELAI_FACE_PARAM_DETECT_IMG_SIZE = 13,  /// 人脸检测输入的图像大小，default： 320 for faster rcnn, recommend set 320 for tiny face detecton. recomand value(tiny->large): 320, 480, 640, 720, 880, 960, 1280
} face_param_type;

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
    PIXELAI_FACE_SMILE_LEFTSTICKING_TONGUE = 7,      ///< 左吐舌笑
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

/******************************************************************************/
/********************************* Part I *************************************/
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
//////////////////// Face Detection and Face Alignment /////////////////////////
////////////////////////////////////////////////////////////////////////////////

///*************************** 人脸初始化函数*********************************
/// @brief create handle for face processing
/// @param[in] det_paraPath path
/// @param[in] config video: FACE_DETECT_MODE_VIDEO, picture: FACE_DETECT_MODE_IMAGE
/// @parma[out] handle, return NULL, if failed
/// @return result_t
PIXELAI_SDK_API pixelai_result_t pixelai_dl_face_create(
                               const char *det_paraPath,
                               const char *pts_paraPath,
                               unsigned int config,
                               pixelai_handle_t *handle
                               );

/// @brief load  240 facial points model
/// @param[in] handle face handle created by pixelai_dl_face_create
/// @param[in] modelPath model path
/// @return result_t
PIXELAI_SDK_API pixelai_result_t pixelai_dl_add_extra_pointmodel(pixelai_handle_t handle, const char *modelPath);

/// @brief load  eyeball points model
/// @param[in] handle face handle created by pixelai_dl_face_create
/// @param[in] modelPath model path
/// @return result_t
PIXELAI_SDK_API pixelai_result_t pixelai_dl_add_eyeballmodel(pixelai_handle_t handle, const char *modelPath);

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
PIXELAI_SDK_API pixelai_result_t pixelai_dl_face_detect(
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

/// @brief  对人脸检测结果进行旋转，翻转等操作
/// @param[in] handle with initialed
/// @param[in] p_face_info store face detetion result
/// @param[in] width, according to landmarks coordinate
/// @param[in] height, according to landmarks coordinate
/// @param[in] angle result process rotate specific angle first, angle =  0/90/180/270
/// @param[in] flip_axis flip x/y 0(no flip)/1(flip X axis)/2(flip Y axis)
/// @return result_t
PIXELAI_SDK_API pixelai_result_t pixelai_dl_face_trans_detect_result(pixelai_handle_t handle, pixelai_face_info_t *p_face_info, int img_width, int img_height, int output_correct_angle, int output_flip_axis);


/// @brief  更改face_param_type定义的参数信息
/// @param[in] handle with initialed
/// @param[in] type face_param_type
/// @param[in] value new threshold
/// @return result_t
PIXELAI_SDK_API pixelai_result_t pixelai_dl_face_setparam(
                                 pixelai_handle_t handle,
                                 face_param_type type,
                                 float value
                                 );


/// @brief  针对video模式下，清空之前跟踪的缓存信息
/// @param[in] handle with initialed
/// @return result_t
PIXELAI_SDK_API pixelai_result_t pixelai_dl_face_reset_tracking(pixelai_handle_t handle);



///*************************** 资源销毁释放函数 *********************************
/// @brief destroy face detector handle
/// @param[in] handle to destroy
/// @return result_t
PIXELAI_SDK_API pixelai_result_t pixelai_dl_face_destroy(pixelai_handle_t handle);


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
PIXELAI_SDK_API pixelai_result_t pixelai_dl_face_attribute_create(
                                         const char *modelPath,
                                         pixelai_handle_t *handle
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
PIXELAI_SDK_API pixelai_result_t pixelai_dl_face_attribute_detect(
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

///*************************** 资源销毁释放函数 *********************************
/// @brief destroy face detector handle
/// @param[in] handle to destroy
/// @return result_t
PIXELAI_SDK_API pixelai_result_t pixelai_dl_face_attribute_destroy(pixelai_handle_t handle);


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
PIXELAI_SDK_API pixelai_result_t pixelai_dl_face_smile_create(
                                     const char *modelPath,
                                     pixelai_handle_t *handle
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
PIXELAI_SDK_API pixelai_result_t pixelai_dl_face_smile_detect(
                                     pixelai_handle_t handle,
                                     const unsigned char *image,
                                     pixelai_pixel_format pixel_format,
                                     int image_width,
                                     int image_height,
                                     int image_stride,
                                     pixelai_face_106_t *p_faces,
                                     pixelai_face_smile_t *smile
                                     );
//PIXELAI_SDK_API pixelai_result_t pixelai_dl_face_smile_detect(
//                                     pixelai_handle_t facehandle,
//                                     pixelai_handle_t smilehandle,
//                                     const unsigned char *image,
//                                     pixelai_pixel_format pixel_format,
//                                     int image_width,
//                                     int image_height,
//                                     int image_stride,
//                                     size_t image_angle,
//                                     unsigned long long detect_config,
//                                     pixelai_face_info_t *p_face_info,
//                                     pixelai_face_smile_t *smile
//                                     );

///*************************** 笑脸阈值修改函数 *********************************
/// @brief face smile set threshold function
/// @param[in] handle with initialed
/// @param[in] value newthreshold
/// @return result_t
PIXELAI_SDK_API pixelai_result_t pixelai_dl_face_smile_set_threshold(pixelai_handle_t handle, float value);

///*************************** 资源销毁释放函数 *********************************
/// @brief destroy face smile handle
/// @param[in] handle to destroy
/// @return result_t
PIXELAI_SDK_API pixelai_result_t pixelai_dl_face_smile_destroy(pixelai_handle_t handle);

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
PIXELAI_SDK_API pixelai_result_t pixelai_dl_face3d_create(
                                     const char *modelPath,
                                     const char *tb3dmmPath,
                                     pixelai_handle_t *handle
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
/// @param[out] 3dmm param, 62 dim, pos: 0~11 shape: 12~51 exp:52:61
/// @param[out] face_vertex stroe face vertex result, format x y z x y z...
/// @return result_t
PIXELAI_SDK_API pixelai_result_t pixelai_dl_face3d_detect(
                                  pixelai_handle_t handle,
                                  const unsigned char *image,
                                  pixelai_pixel_format pixel_format,
                                  int image_width,
                                  int image_height,
                                  int image_stride,
                                  pixelai_face_106_t *p_faces,
                                  p_pixelai_float_t *param_3dmm,
                                  p_pixelai_float_t *face_vertex
                                  );

///*************************** 获取face 3d模型参数信息 *********************************
/// @brief create handle for face attribute
/// @param[in] handle
/// @parma[out] vertex_num
/// @parma[out] tri_num
/// @parma[out] tri_vec, size(tri_num*3)
/// @return result_t
PIXELAI_SDK_API pixelai_result_t pixelai_dl_get_face3d_params(
                                 pixelai_handle_t handle,
                                 p_pixelai_int_t vertex_num,
                                 p_pixelai_int_t tri_num,
                                 p_pixelai_int_t *tri_vec,
                                 p_pixelai_int_t kpt_3d_num,
                                 p_pixelai_int_t *kpt_3d_idxs
                                 );

///*************************** 资源销毁释放函数 *********************************
/// @brief destroy face 3d handle
/// @param[in] handle to destroy
/// @return result_t
PIXELAI_SDK_API pixelai_result_t pixelai_dl_face3d_destroy(pixelai_handle_t handle);

/******************************************************************************/
/******************************** Part V ************************************/
/******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// face recognition //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

///*************************** 人脸识别初始化函数*********************************
/// @brief create handle for face recognition
/// @param[in] modelPath
/// @parma[out] handle, return NULL, if failed
/// @return result_t
PIXELAI_SDK_API pixelai_result_t mobile_dl_face_recognition_create(
                                           const char *modelPath,
                                           pixelai_handle_t *handle
                                           );

///*************************** face recognition feature extraction *********************************
/// @brief face feature extract function
/// @param[in] handle with initialed
/// @param[in] image input image
/// @param[in] pixel_format support type BGR、RGBA、RGB、Y(GRAY)
/// @param[in] image_width width
/// @param[in] image_height height
/// @param[in] image_stride 用于检测的图像的跨度(以像素为单位),即每行的字节数, 默认情况下设为 0
/// @param[in] p_faces face_106_t face detecion result
/// @param[out] 3dmm param, 62 dim, pos: 0~11 shape: 12~51 exp:52:61
/// @param[out] face_vertex stroe face vertex result, format x y z x y z...
/// @return result_t
PIXELAI_SDK_API pixelai_result_t mobile_dl_face_recognition_feature(
                                            pixelai_handle_t handle,
                                            const unsigned char *image,
                                            pixelai_pixel_format pixel_format,
                                            int image_width,
                                            int image_height,
                                            int image_stride,
                                            pixelai_face_106_t *p_faces,
                                            p_pixelai_float_t *feature,
                                            p_pixelai_int_t fea_dim
                                            );

///*************************** get face similarity *********************************
/// @brief face feature extract function
/// @param[in] handle with initialed
/// @param[in] feature of Face A
/// @param[in] feature of Face B
/// @param[out] face similarity score
/// @return result_t
PIXELAI_SDK_API pixelai_result_t mobile_dl_face_similarity(
                                   pixelai_handle_t handle,
                                   p_pixelai_float_t *fea_1,
                                   p_pixelai_float_t *fea_2,
                                   p_pixelai_float_t score
                                   );
///*************************** 资源销毁释放函数 *********************************
/// @brief destroy face recognition handle
/// @param[in] handle to destroy
/// @return result_t
PIXELAI_SDK_API pixelai_result_t mobile_dl_face_recognition_destroy(pixelai_handle_t handle);

/********************* time func ******************************/
PIXELAI_SDK_API double getCurrentMSTime();

#endif
