//
//  air_bokeh_infer.h
//  bokeh_ios
//
//  Created by Jiayong Deng on 2021/3/31.
//

#ifndef air_bokeh_infer_h
#define air_bokeh_infer_h
#include <time.h>
#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <string.h>

#include "mnn_infer.h"
#include "MyFilter.h"
#include "aircv.h"

using namespace std;
using namespace air;

class PoseInfer
{
public:
    PoseInfer();
    ~PoseInfer(){};
    void release();
    
    // 构建计算图，加载模型
    int Create(const char *write_path, const char *model_version);
    int resizeToSmallModelGPU();

    //处理
    void Process(const air8u *src_rgb, int src_width_, int src_height_, int rotate_degree, int *dst_key_points);

// private:
public:

    double avg_time = 0;
    int resize_mnn_net_in_min = 192;
    int resize_mnn_net_in_max = 320;
    int resize_mnn_net_s_in_min = 160;
    int resize_mnn_net_s_in_max = 256;

    //net input w and h
    static int m_net_in_min;
    static int m_net_in_max;
    static int m_net_in_c;

    //net output w and h (mask)
    static int m_net_out_min;
    static int m_net_out_max;
    static int m_net_out_c;

    //print log file for inference time
    bool printLogTxtFile = 0;
    fstream m_time_log;
    uint32_t nframes;
    uint32_t nGPUframes;
    string m_write_path;
    string m_model_file_v;
    string m_model_file_h;
    string m_model_version;

    //load model
    static pthread_mutex_t *mutex;
    static pthread_mutex_t *mutex_load;
    static pthread_t *switch_model_thread;
    static bool m_load_model_success_cpu;
    static bool m_load_model_success_gpu;
    static MnnInfer *seg_net_gpu_h;
    static MnnInfer *seg_net_gpu_v;
    static MnnInfer *seg_net_cpu_h;
    static MnnInfer *seg_net_cpu_v;
    static bool m_set_shape;

    static MnnInfer *seg_net_s_gpu_h;
    static MnnInfer *seg_net_s_gpu_v;
    static bool m_load_model_s_success_gpu;

    bool m_skip = false;
    int max_person_ = 8;
    int num_key_points = 18;
    int stride_ = 4;
    myRect roi_rect = myRect(0, 0, 10000, 10000);
    float threshold_ = 0.2f;
    int m_center_area_cnt = 0;
    int m_center_interval = 10;

    int heat_resize_stride = 4;
    float *heatmap = NULL;
    float *heatmap_peaks = NULL;
    vector<float> keypoints;
    vector<int> person_box_ids;
    vector<int> shape;

    //MultiObjectFilter *lowPassFilter = NULL;
    MyFilter *lowPassFilter = NULL;
    
    //input image
    int m_src_w;
    int m_src_h;
    
    //填充黑边
    int m_src_size;
    uint8_t *m_src_img_tmp;
    uint8_t *m_src_img_resize;

    int new_bg_w;
    int new_bg_h;
    
    int last_src_width;
    int last_src_height;
    int last_src_degree;
    bool m_src_img_change;
    
};

void *my_load_pose_model(void *args);
void *my_loadSmallModel_pose_model(void *args);


inline double air_get_current_system_time2()
{
    struct timeval tv_date;
    gettimeofday(&tv_date, NULL);
    return (double)tv_date.tv_sec * 1000 + (double)tv_date.tv_usec * 0.001;
}



#endif /* air_bokeh_infer_h */
