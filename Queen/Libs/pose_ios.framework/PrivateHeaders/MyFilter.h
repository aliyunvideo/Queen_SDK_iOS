//
// Created by Xuanyi on 2021/6/10.
//

#ifndef HUMANPOSEENG_MYFILTER_H
#define HUMANPOSEENG_MYFILTER_H
#include "StructUtils.h"

class MyFilter {
public:
    MyFilter(int num_points, float alpha, float iou_thr);
    ~MyFilter();
    void update(vector<float> &current_points, vector<int> &results);
//    void update(vector<float> &current_points);
    bool getSkipState();
    void setSkipInterval(int value);

private:
    vector<KeyPointsBox> key_points_box_vector;
    bool is_skip_frame_ = false;
    int skip_interval_ = 2;         // 跳帧间隔
    float skip_thr_ = 0.08f;        // 跳帧阈值
    int skip_cnt_ = 0;
    float alpha_ = 0.5f;            // 滤波当前帧权重
    float beta_up_ = 1.2f;             // 滤波每帧概率增幅
    float beta_down_ = 0.9f;             // 滤波每帧概率减幅
    float iou_threshold_ = 0.6f;    // 人体iou匹配值
    int num_points_ = 18;           // 关键点个数
    const float drop_thr_ = 0.6f;   // point概率丢弃值
    const float dis_thr_ = 0.1f;   // point两帧距离阈值

    int obj_id_=0;
//    vector<int> box_point_idx_ = {2, 5, 8, 11};
    const vector<int> box_point_idx_ = {0, 1, 14, 15, 16, 17,
                                        2, 5, 8, 11};
    const vector<int> face_point_idx_ = {0, 14, 15, 16, 17};

    static float calArea(BBox box);
    static float calIoU(BBox a_box, BBox b_box);

    void filtering(KeyPointsBox &keyPointsBox, vector<fPoint> points_t1);
    vector<KeyPointsBox> getKeypointBox(vector<float> points);
    void matchKeypointBox(vector<KeyPointsBox>);
    void backToUnits(vector<float> &current_points, vector<int> &results);
    void backToFloat(vector<float> &current_points);
    void backToSingleUnit(vector<float> &current_points, vector<int> &results);
    bool judgeSkip(KeyPointsBox kpbox,vector<fPoint> points2);
};


#endif //HUMANPOSEENG_MYFILTER_H
