//
// Created by Xuanyi on 2021/6/10.
//

#ifndef HUMANPOSEENG_STRUCTUTILS_H
#define HUMANPOSEENG_STRUCTUTILS_H

#include <iostream>
#include <utility>
#include <vector>
using namespace std;

struct fPoint{
    float x;
    float y;
    float score;
    fPoint(){
        x = 0;
        y = 0;
        score = 0;
    }
    fPoint(float _x, float _y, float _score){
        x = _x;
        y = _y;
        score = _score;
    }
};

struct iPoint{
    int x;
    int y;
    float score;
};

struct BBox {
    float x1, y1, x2, y2;
    BBox(){}
    BBox(float _x1, float _y1, float _x2, float _y2){
        x1 = _x1;
        y1 = _y1;
        x2 = _x2;
        y2 = _y2;
    }

};

struct KeyPointsBox {
    BBox box;
    vector<fPoint> points;
    int id;
    KeyPointsBox(BBox _box, vector<fPoint> _points){
        box = _box;
        points = _points;
    }
    KeyPointsBox(BBox _box, vector<fPoint> _points, int _id){
        box = _box;
        points = _points;
        id = _id;
    }
//    KeyPointsBox(BBox _box, vector<fPoint> _points, vector<fPoint> _outliers){
//        box = _box;
//        points = std::move(_points);
//        outliers = std::move(_outliers);
//    }
    KeyPointsBox(BBox _box){
        box = _box;
    }
    KeyPointsBox(){
    }
};

//template <typename T>
//inline T Min(const T a, const T b)
//{
//    return (a < b ? a : b);
//}
//
//template <typename T>
//inline T Max(const T a, const T b)
//{
//    return (a > b ? a : b);
//}
#endif //HUMANPOSEENG_STRUCTUTILS_H
