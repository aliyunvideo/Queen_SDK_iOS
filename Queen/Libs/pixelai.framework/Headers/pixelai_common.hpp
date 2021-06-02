#ifndef INCLUDE_PIXELAI_COMMON_H_
#define INCLUDE_PIXELAI_COMMON_H_

#ifdef _MSC_VER
#   ifdef __cplusplus
#       ifdef PIXELAI_STATIC_LIB
#           define PIXELAI_SDK_API  extern "C"
#       else
#           ifdef SDK_EXPORTS
#               define PIXELAI_SDK_API extern "C" __declspec(dllexport)
#           else
#               define PIXELAI_SDK_API extern "C" __declspec(dllimport)
#           endif
#       endif
#   else
#       ifdef PIXELAI_STATIC_LIB
#           define PIXELAI_SDK_API
#       else
#           ifdef SDK_EXPORTS
#               define PIXELAI_SDK_API __declspec(dllexport)
#           else
#               define PIXELAI_SDK_API __declspec(dllimport)
#           endif
#       endif
#   endif
#else /* _MSC_VER */
#   ifdef __cplusplus
#       ifdef SDK_EXPORTS
#           define PIXELAI_SDK_API extern "C" __attribute__((visibility ("default")))
#       else
#           define PIXELAI_SDK_API extern "C"
#       endif
#   else
#       ifdef SDK_EXPORTS
#           define PIXELAI_SDK_API __attribute__((visibility ("default")))
#       else
#           define PIXELAI_SDK_API
#       endif
#   endif
#endif

#ifndef PIXELAI_MIN
#define PIXELAI_MIN(a,b) ( (a)<(b)?(a):(b) )
#endif

#ifndef PIXELAI_MAX
#define PIXELAI_MAX(a,b) ( (a)>(b)?(a):(b) )
#endif


//#ifdef __APPLE__
//#  if TARGET_OS_IPHONE
//#    pragma  message("iphone  macro  activated!")
//#    define TEST_DATA_PATH "../../"
//#    define MODEL_PATH "../../../../../"
//#  elif TARGET_OS_MAC
//#    pragma  message("mac  macro  activated!")
//#    define TEST_DATA_PATH MODEL_PATH ""
//#    define MODEL_PATH "/data/local/tmp/models/"
//#  else
////#    error "Unknown Apple platform"
//#  endif
//#elif __ANDROID__
//// android
//#  pragma  message("android  macro  activated!")
//#  define TEST_DATA_PATH "/data/local/tmp/"
//#  define MODEL_PATH "/data/local/tmp/"
//#  define imshow(...)
//#  define waitKey(...)
//#else
//#  error "Unknown platform"
//#endif


#ifdef __APPLE__
#    define TEST_DATA_PATH "../../"
#    define MODEL_PATH "../../../../../"
#elif __ANDROID__
// android
#  define TEST_DATA_PATH "/data/local/tmp/"
#  define MODEL_PATH "/data/local/tmp/"
#  define imshow(...)
#  define waitKey(...)
#elif defined(_WIN32)
#    define TEST_DATA_PATH "../../"
#    define MODEL_PATH "../../../../../"
#else
#  error "Unknown platform"
#endif




#include <functional>
#include <istream>
#include <string>
#include <memory>
#include <vector>

typedef std::function<std::unique_ptr<std::istream>(const std::string&,std::ios_base::openmode)> pixelai_istream_func;
typedef std::function<std::unique_ptr<std::ostream>(const std::string&,std::ios_base::openmode)> pixelai_ostream_func;
typedef std::function<std::vector<uint8_t>(const std::string)> pixelai_read_buffer_func;
/// ******************** PixelAI 全局句柄 ********************
/// pixelai handle declearation
typedef void *pixelai_handle_t;


/// ******************** PixelAI 函数返回值及类型 ********************
/// pixelai function result declearation
typedef int   pixelai_result_t;

typedef int    pixelai_int_t;
typedef int    *p_pixelai_int_t;
typedef float  pixelai_float_t;
typedef float  *p_pixelai_float_t;

/// pixelai function result type
#define PIXELAI_OK                                  0   ///< 正常运行

#define PIXELAI_ERROR_HANDLE                       -1  ///< 句柄错误
#define PIXELAI_ERROR_INVALID_HANDLE_PARAM         -2  ///< 句柄调用参数不正确
#define PIXELAI_ERROR_OUT_OF_MEMORY                -3  ///< 内存不足

#define PIXELAI_ERROR_MODEL_INIT                   -4  ///< 模型初始化错误
#define PIXELAI_ERROR_MODEL_NOT_FOUND              -5  ///< 模型不存在
#define PIXELAI_ERROR_INVALID_MODEL_FORMAT         -6  ///< 模型格式不正确
#define PIXELAI_ERROR_MODEL_UNINIT                 -7  ///< 模型未初始化

#define PIXELAI_ERROR_PIXEL_NULL_POINTER           -8  ///< 图像为空
#define PIXELAI_ERROR_INVALID_PIXEL_FORMAT         -9  ///< 图像格式不正确
#define PIXELAI_ERROR_INVALID_PIXEL_PARAM          -10  ///< 图像参数不正确
#define PIXELAI_ERROR_DELNOTFOUND                  -11  ///< 定义缺失


/// ******************** PixelAI 通用图像格式 ********************
/// pixelai pixel format definition
typedef enum {
    PIXELAI_PIXEL_FORMAT_Y,            ///< Y    1        8bpp ( 单通道8bit灰度像素 )
    PIXELAI_PIXEL_FORMAT_RGBA8888,     ///< RGBA 8:8:8:8 32bpp ( 4通道32bit RGBA 像素 )
    PIXELAI_PIXEL_FORMAT_BGRA8888,     ///< BGRA 8:8:8:8 32bpp ( 4通道32bit BGRA 像素 )
    PIXELAI_PIXEL_FORMAT_RGB888,       ///< RGB 8:8:8 24bpp ( 3通道32bit RGB 像素 )
    PIXELAI_PIXEL_FORMAT_BGR888,       ///< BGR 8:8:8 24bpp ( 3通道32bit BGR 像素 )
    PIXELAI_PIXEL_FORMAT_YUV_420,      ///< YUV_420
    PIXELAI_PIXEL_FORMAT_YUV_NV21,     ///< YUV_NV21
} pixelai_pixel_format;

/// pixelai image rotate type definition
typedef enum {
    PIXELAI_CLOCKWISE_ROTATE_0 = 0,    ///< 图像不需要旋转
    PIXELAI_CLOCKWISE_ROTATE_90 = 1,   ///< 图像需要顺时针旋转90度
    PIXELAI_CLOCKWISE_ROTATE_180 = 2,  ///< 图像需要顺时针旋转180度
    PIXELAI_CLOCKWISE_ROTATE_270 = 3,   ///< 图像需要顺时针旋转270度
    PIXELAI_FLIP_VERTICAL        = 4,   ///< 图像垂直翻转
    PIXELAI_FILP_HORIZONTAL      = 5,   ///< 图像水平翻转
} pixelai_rotate_type;

/// pixelai 支持的颜色转换格式
typedef enum {
    PIXELAI_RGBA_Y = 0,    ///< PIXELAI_PIXEL_FORMAT_RGBA8888到PIXELAI_PIXEL_FORMAT_Y转换
    PIXELAI_BGRA_Y = 1,    ///< PIXELAI_PIXEL_FORMAT_BGRA8888到PIXELAI_PIXEL_FORMAT_Y转换
    PIXELAI_RGB_Y = 2,     ///< PIXELAI_PIXEL_FORMAT_RGB888到PIXELAI_PIXEL_FORMAT_Y转换
    PIXELAI_BGR_Y = 3,     ///< PIXELAI_PIXEL_FORMAT_BGR888到PIXELAI_PIXEL_FORMAT_Y转换
    PIXELAI_Y_BGR = 4,     ///< PIXELAI_PIXEL_FORMAT_Y到PIXELAI_PIXEL_FORMAT_BGR888转换
    PIXELAI_RGBA_BGR = 5,  ///< PIXELAI_PIXEL_FORMAT_RGBA8888到PIXELAI_PIXEL_FORMAT_BGR888转换
    PIXELAI_BGRA_BGR = 6,  ///< PIXELAI_PIXEL_FORMAT_BGRA8888到PIXELAI_PIXEL_FORMAT_BGR888转换
    PIXELAI_RGB_BGR = 7    ///< PIXELAI_PIXEL_FORMAT_RGB888到PIXELAI_PIXEL_FORMAT_BGR888转换
} pixelai_color_convert_type;


/// ******************** PixelAI 图像处理函数 ********************
/// 进行颜色格式转换
/// @param[in] image_src 用于待转换的图像数据
/// @param[out] image_dst 转换后的图像数据
/// @param[in] image_width 用于转换的图像的宽度(以像素为单位)
/// @param[in] image_height 用于转换的图像的高度(以像素为单位)
/// @param[in] type 需要转换的颜色格式
/// @return 正常返回PIXELAI_OK,否则返回错误类型
PIXELAI_SDK_API pixelai_result_t
pixelai_image_color_convert(
    const unsigned char *image_src,
    unsigned char *image_dst,
    int image_width,
    int image_height,
    pixelai_color_convert_type type
);

/// 旋转图像
/// @param[in] image_src 待旋转的图像数据
/// @param[out] image_dst 旋转后的图像数据
/// @param[in] image_width 待旋转的图像的宽度
/// @param[in] image_height 待旋转的图像的高度
/// @param[in] image_stride 待旋转的图像的跨度
/// @param[in] pixel_format 待旋转的图像的格式
/// @param[in] rotate_type 顺时针旋转角度
/// @return 正常返回PIXELAI_OK,否则返回错误类型
PIXELAI_SDK_API pixelai_result_t
pixelai_image_rotate(
    const unsigned char *image_src,
    unsigned char *image_dst,
    int image_width,
    int image_height,
    int image_stride,
    pixelai_pixel_format pixel_format,
    pixelai_rotate_type rotate_type
);


/// ******************** PixelAI 通用数据格式 ********************
/// pixelai rectangle definition
typedef struct pixelai_rect_t {
    int left;   ///< 矩形最左边的坐标
    int top;    ///< 矩形最上边的坐标
    int right;  ///< 矩形最右边的坐标
    int bottom; ///< 矩形最下边的坐标
} pixelai_rect_t, *p_pixelai_rect_t;

/// pixelai float type point definition
typedef struct pixelai_pointf_t {
    float x;    ///< 点的水平方向坐标,为浮点数
    float y;    ///< 点的竖直方向坐标,为浮点数
} pixelai_pointf_t;

/// pixelai integer type point definition
typedef struct pixelai_pointi_t {
    int x;      ///< 点的水平方向坐标,为整数
    int y;      ///< 点的竖直方向坐标,为整数
} pixelai_pointi_t;

/// pixelai float type 3d point definition
typedef struct pixelai_pointf_3d_t {
    float x;
    float y;
    float z;
} pixelai_pointf_3d_t;

/// pixelai integer type 3d point definition
typedef struct pixelai_pointi_3d_t {
    int x;
    int y;
    int z;
} pixelai_pointi_3d_t;

/// pixelai time definition
typedef struct pixelai_time_t {
    long int tv_sec;    ///< 秒
    long int tv_usec;   ///< 微秒
}pixelai_time_t;

/// pixelai image definition
typedef struct pixelai_image_t {
    unsigned char *data;                 ///< 图像数据指针
    pixelai_pixel_format pixel_format;   ///< 图像格式
    int width;                           ///< 宽度(以像素为单位)
    int height;                          ///< 高度(以像素为单位)
    int stride;                          ///< 跨度, 即每行所占的字节数
    pixelai_time_t time_stamp;           ///< 时间戳
} pixelai_image_t;

#endif
