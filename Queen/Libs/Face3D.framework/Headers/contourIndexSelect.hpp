//
//  contourIndexSelect.cpp
//  Face AR
//
//  Created by shiwei zhou on 2018/1/30.
//  Copyright © 2018年 shiwei zhou. All rights reserved.
//

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include "pixelai_face3D.h"
#include "pixelai_face3D_private.h"
using namespace std;

inline float distance(cv::Point2f pt1,cv::Point2f pt2){
    return sqrt((pt1.x-pt2.x)*(pt1.x-pt2.x)+(pt1.y-pt2.y)*(pt1.y-pt2.y));
}
inline std::vector<int> SelectBestleftIndexFrom3D(std::vector<int> &contour_index_3D,cv::Mat &commbine_shape,std::vector<cv::Point2f>&image_pts,glm::mat4x4 modelview, glm::mat4x4 projection, glm::vec4 viewport)
{
    std::vector<int> bestIndex;
    std::vector<cv::Point3f> contour_pts;
    for (int i=0; i<contour_index_3D.size(); i++) {
        contour_pts.push_back(cv::Point3f(commbine_shape.at<float>((contour_index_3D[i]*3+0),0),commbine_shape.at<float>((contour_index_3D[i]*3+1),0),commbine_shape.at<float>((contour_index_3D[i]*3+2),0)));
    }

    std::vector<cv::Point2f> project_contour_image_pts;
    Project3DVertexTo2Dimage(project_contour_image_pts, contour_pts, modelview, projection, viewport);
    
//    for (int i=0; i<project_contour_image_pts.size(); i++) {
//       cv::circle(img,project_contour_image_pts[i],2,Scalar(0,0,255),1,0);
//    }
    std::vector<int> order_index_3d;
    for(int i=0;i<8;i++){
        float min_distance=100000.0;
        int min_index=-1;
        for (int j=0; j<project_contour_image_pts.size(); j++)
        {
            cv::Point2f cur_org_pt=image_pts[i];
            cv::Point2f project_cur_pt=project_contour_image_pts[j];
            float dis=distance(cur_org_pt,project_cur_pt);
            if(min_distance>dis){min_distance=dis;min_index=contour_index_3D[j];}
        }
        
        order_index_3d.push_back(min_index);
     //   cv::circle(img,image_pts[i],1,Scalar(255,0,0),1,0);
    }
//    for(int i=0;i<order_index_3d.size();i++){
//        int index=order_index_3d[i];
//        cv::circle(img,project_contour_image_pts[index],4,Scalar(0,255,0),1,0);
//    }
//    imshow("left",img);
//    cv::waitKey(0);
    return order_index_3d;
}

inline std::vector<int> SelectBestRightIndexFrom3D(std::vector<int> &contour_index_3D,cv::Mat &commbine_shape,std::vector<cv::Point2f>&image_pts,glm::mat4x4 modelview, glm::mat4x4 projection, glm::vec4 viewport)
{
    std::vector<int> bestIndex;
    std::vector<cv::Point3f> contour_pts;
    for (int i=0; i<contour_index_3D.size(); i++) {
        contour_pts.push_back(cv::Point3f(commbine_shape.at<float>((contour_index_3D[i]*3+0),0),commbine_shape.at<float>((contour_index_3D[i]*3+1),0),commbine_shape.at<float>((contour_index_3D[i]*3+2),0)));
    }
    
    std::vector<cv::Point2f> project_contour_image_pts;
    Project3DVertexTo2Dimage(project_contour_image_pts, contour_pts, modelview, projection, viewport);
    
    //    for (int i=0; i<project_contour_image_pts.size(); i++) {
    //        cv::circle(img,project_contour_image_pts[i],2,Scalar(0,0,255),1,0);
    //    }
    
    std::vector<int> order_index_3d;
    for(int i=9;i<17;i++){
        float min_distance=100000.0;
        int min_index=-1;
        for (int j=0; j<project_contour_image_pts.size(); j++)
        {
            cv::Point2f cur_org_pt=image_pts[i];
            cv::Point2f project_cur_pt=project_contour_image_pts[j];
            float dis=distance(cur_org_pt,project_cur_pt);
            if(min_distance>dis){
                min_distance=dis;
                min_index=contour_index_3D[j];
            }
        }
        
        order_index_3d.push_back(min_index);
        // cv::circle(img,image_pts[i],8,Scalar(255,0,0),1,0);
    }
    //    for(int i=0;i<order_index_3d.size();i++){
    //        int index=order_index_3d[i];
    //        cv::circle(img,project_contour_image_pts[index],4,Scalar(0,255,0),1,0);
    //    }
    //    imshow("right",img);
    //    cv::waitKey(0);
    return order_index_3d;
}
