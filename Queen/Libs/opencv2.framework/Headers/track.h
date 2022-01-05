//  Created by zhuangshu on 2018/4/19.
//  Copyright © 2018年 zhuangshu. All rights reserved.
//
#ifndef _MOTION_TRACK_H_
#define _MOTION_TRACK_H_

#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif

/// Ali integer rectangle definition
typedef struct rect_t {
    int left;    //left of face rectangle
    int top;     //top of face rectangle
    int right;   //right of face rectangle
    int bottom;  //bottom of face rectangle
} rect_t, *p_rect_t;

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


/// pixel format definition
typedef enum {
    TB_PIXEL_FORMAT_Y,                   ///< Y    1        8bpp ( 单通道8bit灰度像素 )
    TB_PIXEL_FORMAT_RGBA8888,            ///< RGBA 8:8:8:8 32bpp ( 4通道32bit RGBA 像素 )
    TB_PIXEL_FORMAT_BGRA8888,            ///< BGRA 8:8:8:8 32bpp ( 4通道32bit RGBA 像素 )
    TB_PIXEL_FORMAT_RGB888,              ///< RGB 8:8:8 24bpp ( 3通道32bit RGB 像素 )
    TB_PIXEL_FORMAT_BGR888,              ///< BGR 8:8:8 24bpp ( 3通道32bit BGR 像素 )
} tb_pixel_format;

struct MV
{
    int16_t x;
    int16_t y;
};

#ifdef __cplusplus
extern "C" {
#endif
    
int FaceRectTrack(unsigned char* refY, unsigned char* curY, int picwidth, int picheight, rect_t refbox, rect_t *curbox);
int FaceRectTrack_dia(unsigned char* refY, unsigned char* curY, int picwidth, int picheight, rect_t refbox, rect_t *curbox);
int FaceRectTrack_hex(unsigned char* refY, unsigned char* curY, int picwidth, int picheight, rect_t refbox, rect_t *curbox);

#ifdef __cplusplus
}
#endif

#endif /* _MOTION_TRACK_H_ */
