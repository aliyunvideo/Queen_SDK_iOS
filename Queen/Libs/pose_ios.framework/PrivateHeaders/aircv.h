#ifndef AIRCV_H
#define AIRCV_H

#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>
using namespace std;

#ifndef CLIP
#define CLIP(x, y) (x < 0 ? 0 : (x > y ? y : x))
#endif

struct myRect
{
    int x1, y1, w, h;
    myRect(int _x, int _y, int _w, int _h) : x1(_x), y1(_y), w(_w), h(_h) {};
    myRect() : x1(-1), y1(-1), w(-1), h(-1) {};
};

template <typename T>
inline int intRound(const T a)
{
    return int(a + 0.5f);
}

template <typename T>
inline T fastMin(const T a, const T b)
{
    return (a < b ? a : b);
}



void cropImage(unsigned char *src, unsigned char *dst, int img_w, int x1, int y1, int x2, int y2);

//myRect getCropRect(myRect src_rect, int img_w, int img_h);
myRect getCropRect(myRect src_rect, int img_w, int img_h, int num_visable_points);

int air_resize_bilinear_3channel(unsigned char *src, int src_width, int src_height, int src_stride,
                                 unsigned char *dst, int dst_width, int dst_height, int dst_stride);


void connectBodyParts(vector<float> &pose_keypoints,
                      const float *heatmap, const float *peaks,
                      int map_w, int map_h,
                      const int inter_min_above_th,
                      const float inter_th,
                      const int min_subset_cnt,
                      const float min_subset_score,
                      int max_person_);

void findHeatmapPeaks(const float *src, float *dst,
                      const int src_w, const int src_h, const int src_c,
                      const float thr, int max_person_);

int getMaxAreaIdx(vector<float> points, int num_key_points);

vector<float> mergeDecode(vector<float> paf_point, vector<float> argmax_point, int num_key_points, float threshold_);

vector<float> decodeArgmax(const float *heatmap, int num_key_points, int net_out_w, int net_out_h, float threshold_);

vector<float> decodePafHeat(const float *heatmap, int net_out_w, int net_out_h, int max_person_, int num_key_points, float threshold_);

vector<float> mergeDecode(const float *heatmap, int net_out_w, int net_out_h, int max_person, int num_key_points, float threshold_);

void mappingPoints(vector<float> &keypoint, int img_width, int img_height, int num_key_points, myRect &roi_area_);
void uopdateMaxArea(vector<float> &keypoint, int img_width, int img_height, int num_key_points, myRect &roi_area_, int &center_area_cnt_, int &center_interval_);




#endif //AIRCV_H
