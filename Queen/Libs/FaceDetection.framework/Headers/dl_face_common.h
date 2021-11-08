#ifndef _FACE_COMMON_H_
#define _FACE_COMMON_H_

#define MAX_FACE 32
#define MAX_FACE_ATTRIBUTES 16

#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif

/******************************************************************************/
/********************************* 返回值类型 ***********************************/
/******************************************************************************/
#define TB_OK                            0   ///< 正常运行
#define TB_INPUT_IMG_TYPE_ERROR         -1  ///< 输入图像类型不符合
#define TB_MNN_NET_INIT_ERROR           -2  ///< 网络初始化错误
#define TB_HANDLE_ERROR                 -3  ///< 句柄错误
#define TB_DELNOTFOUND_ERROR            -4  ///< 定义缺失
#define TB_IMG_EMPTY_ERROR              -5  ///< 图像内容为空

/******************************************************************************/
/************************** 人脸检测数据结构定义 **********************************/
/******************************************************************************/
/// handle declearation
typedef void *handle_t;
/// result declearation
typedef int   result_t;

/// Ali integer face rectangle definition
typedef struct rect_t {
    int left;    //left of face rectangle
    int top;     //top of face rectangle
    int right;   //right of face rectangle
    int bottom;  //bottom of face rectangle
} rect_t, *p_rect_t;

typedef struct face_rects_t {
    p_rect_t rects;
    float* scores; //confidence of face
    int face_count;
} face_rects_t, *p_face_rects_t;

/// Ali float type point definition
typedef struct pointf_t {
    float x;
    float y;
} pointf_t;

/// Ali integer type point definition
typedef struct pointi_t {
    int x;
    int y;
} pointi_t;

/// Ali float type 3d point definition
typedef struct pointf_3d_t {
    float x;
    float y;
    float z;
} pointf_3d_t;

/// Ali integer type 3d point definition
typedef struct pointi_3d_t {
    int x;
    int y;
    int z;
} pointi_3d_t;


/// pixel format definition
typedef enum {
    TB_PIXEL_FORMAT_Y,                   ///< Y    1        8bpp ( 单通道8bit灰度像素 )
    TB_PIXEL_FORMAT_RGBA8888,            ///< RGBA 8:8:8:8 32bpp ( 4通道32bit RGBA 像素 )
    TB_PIXEL_FORMAT_BGRA8888,            ///< BGRA 8:8:8:8 32bpp ( 4通道32bit RGBA 像素 )
    TB_PIXEL_FORMAT_RGB888,              ///< RGB 8:8:8 24bpp ( 3通道32bit RGB 像素 )
    TB_PIXEL_FORMAT_BGR888,              ///< BGR 8:8:8 24bpp ( 3通道32bit BGR 像素 )
    TB_PIXEL_FORMAT_YUV_420,
    TB_PIXEL_FORMAT_YUV_NV21,
} tb_pixel_format;


/// @brief the information of 106 facial landmarks
typedef struct face_106_t {
    rect_t rect;                         // face rectangle
    float score;                         // confidence
    pointf_t landmarks_array[106];       // 106 facial points
    float landmarks_visible_array[106];  // visibility of 106 facail points
    pointf_t p_extra_face_landmarks[134];    ///< 眼睛、眉毛、嘴唇关键点. 
    int extra_face_landmarks_count;      ///< 眼睛、眉毛、嘴唇关键点个数. 检测到时为134,
    pointf_t p_eyeball_contour[38];      ///< 眼球轮廓点
    pointf_t p_eyeball_center[2];        ///< 眼球瞳孔中心点
    int eyeball_contour_points_count;    ///< 眼球轮廓关键点个数.
    float left_eyeball_score;           ///< 左眼球置信度: [0.0, 1.0]
    float right_eyeball_score;          ///< 右眼球置信度: [0.0, 1.0]
    float yaw;                           // left: + ; right: -
    float pitch;                         // down:-; up:+;
    float roll;                          // left: - ; right: +
//    float eye_dist;                    // pupillary distance
    unsigned int face_action;            // face action
    int faceID;
} face_106_t, *p_face_106_t;

/// @brief face detection result
typedef struct mobile_face_info_t {
    face_106_t *p_faces;                 //face info
    int face_count;                      //face deteciton num
} mobile_face_info_t, *p_mobile_face_info_t;


/******************************************************************************/
/************************** 人脸属性数据结构定义 **********************************/
/******************************************************************************/

#ifndef MOBILE_MAX_ATTR_STRING_LEN
#define MOBILE_MAX_ATTR_STRING_LEN 32
#endif
/// @brief 单个属性
typedef struct face_attribute_t {
    char category[MOBILE_MAX_ATTR_STRING_LEN];   /// 属性描述, 例如 "age", "gender"
    char label[MOBILE_MAX_ATTR_STRING_LEN];      /// 属性标签描述, 例如 "18", "male" or "female"
    float score;                                 /// 该属性标签的置信度
} face_attribute_t;

/// @brief 一个人脸的所有属性
typedef struct face_attributes_t {
    face_attribute_t *p_attributes;    /// 属性数组
    int attribute_count;               /// 属性个数
} face_attributes_t, *p_face_attributes_t;


/******************************************************************************/
/************************** 笑脸分类模块数据结构定义 *******************************/
/******************************************************************************/
typedef enum {
    FACE_SMILE_NEUTRAL = 0,                   ///< 自然表情
    FACE_SMILE_LAUGH = 1,                     ///< 正面大笑
    FACE_SMILE_LEFT_LAUGH = 2,                ///< 朝左大笑
    FACE_SMILE_RIGHT_LAUGH =3,                ///< 朝右大笑
    FACE_SMILE_SMILLING = 4,                  ///< 正面微笑
    FACE_SMILE_UP_SMILLING = 5,               ///< 朝上微笑
    FACE_SMILE_DOWN_SMILLING =6,              ///< 朝下微笑
    FACE_SMILE_LEFT_STICKING_TONGUE = 7,      ///< 左吐舌笑
    FACE_SMILE_RIGHT_STICKING_TONGUE =8,      ///< 右吐舌笑
    FACE_SMILE_LEFT_THROW_KISS = 9,           ///< 左飞吻
    FACE_SMILE_RIGHT_THROW_KISS = 10,         ///< 右飞吻
    FACE_SMILE_LEFT_TWITCH_MOUTH = 11,        ///< 左憋嘴
    FACE_SMILE_RIGHT_TWITCH_MOUTH = 12,       ///< 右憋嘴
} face_smile_type;



typedef struct face_smile_t {
    face_smile_type smile_type;              ///< 笑脸类型
    float score;                             ///< 分数
} face_smile_t, *p_face_smile_t;


#endif // _FACE_COMMON_H_
