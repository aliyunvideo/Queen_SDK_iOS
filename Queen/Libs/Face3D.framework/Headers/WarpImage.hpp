//
//  WarpImage.hpp
//  CartoonGenerate
//
//  Created by zhoushiwei on 16/1/18.
//  Copyright © 2016年 zhoushiwei. All rights reserved.
//

#ifndef WarpImage_hpp
#define WarpImage_hpp

#include <stdio.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

void getExternPoints(std::vector<cv::Point2f>& extPoints, double rollAngle, int width, int height);

void morphTriangle(const Mat &img1, const vector<Point2f> &src, const vector<Point2f> &dst, const vector<Vec3i> &indices, Mat &img2);

void ImageMorphing(std::vector<cv::Point2f>& src_points1,
                   std::vector<cv::Point2f>& src_points2,
                   std::vector<cv::Vec3i>& triangle_indices,
                   int width, int height);

void CreateMap(const cv::Mat& TriangleMap,
               const std::vector<cv::Mat>& HomMatrices,
               cv::Mat& map_x,
               cv::Mat& map_y);

// Morph homography matrix
void MorphHomography(const std::vector<cv::Mat>& Homs,
                     std::vector<cv::Mat>& MorphHoms1,
                     std::vector<cv::Mat>& MorphHoms2,
                     float blend_ratio);

#endif /* WarpImage_hpp */
