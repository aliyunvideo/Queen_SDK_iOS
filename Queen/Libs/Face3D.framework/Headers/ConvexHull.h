//
//  ConvexHull.hpp
//  GPUImage-x-Sample
//
//  Created by limengyao on 2019/8/28.
//  Copyright © 2019 jin. All rights reserved.
//

#ifndef ConvexHull_h
#define ConvexHull_h

#include <stdio.h>
#include "pixelai_common.h"
void getMinAreaRect(pixelai_pointf_t *arr, int len, std::vector<pixelai_pointf_t> &rect);
void convexHull(pixelai_pointf_t *points, int n, std::vector<pixelai_pointf_t>& convexPoints);
void scale_polygon(const std::vector<pixelai_pointf_t> &points,
                    std::vector<pixelai_pointf_t> &new_points,
                    pixelai_pointf_t scale);
float area_of_polygon(const std::vector<pixelai_pointf_t> &points);
pixelai_pointf_t centroid_of_polygon(const std::vector<pixelai_pointf_t> &points);
bool isInside(const std::vector<pixelai_pointf_t> &polygon, int n, pixelai_pointf_t p);
float minimum_distance(const pixelai_pointf_t &v, const pixelai_pointf_t &w, pixelai_pointf_t &p);
float point_foot_on_polygon(const std::vector<pixelai_pointf_t> &polygon, pixelai_pointf_t &p);
bool is_polygon_convex(const std::vector<pixelai_pointf_t> &points);
#endif /* ConvexHull_h */
