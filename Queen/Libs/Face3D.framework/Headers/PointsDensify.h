//
//  PointsDensify
//  AliMedia
//
//  Created by limengyao on 2018/9/28.
//

#ifndef PointsDensify_h
#define PointsDensify_h

#include <opencv2/core.hpp>
#include "pixelai_face3D.h"
#include "pixelai_face3D_private.h"
using namespace cv;
using namespace std;

void DensifyKeyPoints(const vector<Point2f> &initialKP, vector<Point2f> &denseKP, double scale, vector<int>& dirs);
void ExtendKeyPoints(const vector<Point2f> &initialKP, vector<Point2f> &extendKP, double scale, vector<int>& dirs);
void DensifySeg(const vector<Point2f> &points, vector<Point2f> &denseKP, int degree, int dir, int times);
Mat PolynomialFitting(const vector<Point2f> &points, int degree, int dir);

void SmoothKeyPoints(vector<Point2f> &sourceKP, vector<Point2f> &resultKP);

vector<Point3d> Densify3DKeyPoint(const Mat &initialMat, bool interDense=false);
Mat point2Mat(const vector<Point3d> &denseKP);
vector<Point3d> mat2Point(const Mat &initiaMat);
vector<Point3d> Outer3DKeyPoint(const vector<Point3d> &initialKP, const vector<Point3d> &denseKP);
void DensifyHelper(const vector<Point3d> &initialKP, vector<Point3d> &denseKP);

#endif /* PointsDensify_h */
