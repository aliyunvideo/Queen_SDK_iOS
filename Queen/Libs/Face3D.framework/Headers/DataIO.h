//
//  DataIO.h
//
//  Created by zhoushiwei on 14-6-5.
//  Copyright (c) 2014年 zhoushiwei. All rights reserved.
//

#ifndef __Avatar__DataIO__
#define __Avatar__DataIO__

//#include <opencv2/core/core_c.h>
#include <fstream>
#include <vector>
#include <string>
#include <opencv2/core.hpp>
#include "pixelai_face3D.h"
#include "ErrorType.h"

using namespace std;

std::string GetFace3dResPath();
void ReadMatrix(std::istream *s, cv::Mat &M);
void ReadNumber(std::istream *s, int &number);
bool WriteData(const std::string &Mainpath, std::vector<cv::Mat> params);
void WriteMatrix(std::ofstream *s, cv::Mat &M);
void WriteNumber(std::ofstream *s, int &number);
bool LoadMatBinary(const std::string& filename, cv::Mat& output);
bool SaveMatBinary(const std::string& filename, const cv::Mat& output);
std::unique_ptr<std::ofstream> getoStream(const std::string& path,ios_base::openmode mode);
std::unique_ptr<std::istream> getiStream(const std::string& path,ios_base::openmode mode);
cv::Mat ReadMat(std::istream *s);
#endif /* defined(__Avatar__DataIO__) */
