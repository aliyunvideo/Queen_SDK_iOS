//
//  air_bokeh.h
//  pose_ios
//
//  Created by Jiayong Deng on 2021/8/6.
//
#pragma once
#ifndef air_pose_h
#define air_pose_h

#include <stdio.h>
#include <stdint.h>

#if defined(_WIN32)
#define AIR_PUBLIC extern "C" __declspec(dllexport)
#elif defined(__APPLE__)
#define AIR_PUBLIC __attribute__((visibility("default"))) extern "C"
#elif defined(__ANDROID__) || defined(__linux__)
#define AIR_PUBLIC extern "C" __attribute__((visibility("default")))
#else
#define AIR_PUBLIC extern "C"
#endif

/**
 * create a context
 * parm: no parms
 **/
//AIR_PUBLIC void *air_create_context_pose();
AIR_PUBLIC void *air_create_context_pose(const char *write_path = NULL, bool skip_frame = false);

/**
 * feed frame to context one by one
 * parm src_rgb: input rgbrgbrgb buffer
 * parm src_width: input image width
 * parm src_height: input image height
 * parm keypoints: keypoints = new int[10*(1+18*3)+1];
 * {n, [id, x,y,score, x,y,score, x,y,score, x,y,score ...], [id, x,y,score, x,y,score, x,y,score, x,y,score ...], ...}
 * n: is real num of person, maximun num of person is 10
 * id: indicates person id
 **/
AIR_PUBLIC int air_feed_frame_pose(void *context, const uint8_t *src_rgb, int src_width, int src_height, int rotate_degree, int *keypoints);
/**
 * destroy context
 **/
AIR_PUBLIC int air_destroy_context_pose(void *context);


#endif /* air_pose_h */
