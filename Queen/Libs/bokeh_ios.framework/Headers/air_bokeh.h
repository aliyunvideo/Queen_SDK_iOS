//
//  air_bokeh.h
//  bokeh_ios
//
//  Created by Jiayong Deng on 2021/4/1.
//
#pragma once
#ifndef air_bokeh_h
#define air_bokeh_h

#include <stdio.h>
#include <stdint.h>


//#if defined(_WIN32)
//#define AIR_PUBLIC extern "C" __declspec(dllexport)
//#elif defined(__APPLE__)
//#define AIR_PUBLIC __attribute__((visibility("default"))) (extern "C")
//#elif defined(__ANDROID__) || defined(__linux__)
//#define AIR_PUBLIC extern "C" __attribute__((visibility("default")))
//#else
//#define AIR_PUBLIC extern "C"
//#endif


/**
 * create a context
 * parm: flag must be 0 or 1, 0-high computation phones, 1 for low computation phones
 **/
void *air_create_context(int flag);

/**
 * feed frame to context one by one
 * parm src_rgb: input rgbrgbrgb buffer
 * parm src_width: input image width
 * parm src_height: input image height
 * parm mask: output mask [0, 255]
 **/
int air_feed_frame(void *context, uint8_t *src_rgb, int src_width, int src_height, int rotate_degree, uint8_t *mask);
/**
 * destroy context
 **/
int air_destroy_context(void *context);


#endif /* air_bokeh_h */
