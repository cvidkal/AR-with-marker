//
//  Marker.h
//  newAR
//
//  Created by ZJU_CVG on 15/1/29.
//  Copyright (c) 2015年 Shangjin Zhai. All rights reserved.
//

#ifndef newAR_Marker_h
#define newAR_Marker_h

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include "fast.h"
#include "Feature.h"

using namespace std;
using namespace cv;
using namespace cvx;


class Marker
{
public:
    Marker();
    ~Marker();
    Marker(const Marker&);
	Marker(const Mat&img, const Mat& R, Ptr<Fast> fast, Ptr<cv::ORB> orb, int levelNum = 8, float scaleFactor = 1.25);
	MultiScaleStruct fastStructure;
	SingleScaleStructrue orbStructure;
	Ptr<Fast> fast;
	Ptr<cv::ORB> orb;


    void reset();
    bool registed;
       
    vector<cv::Point3f> points3D1;
    vector<cv::Point3f> points3D2;
    vector<cv::Point3f> pts2dH;
    vector<KeyPoint> opts;
    vector<vector<KeyPoint>> allKeyPoints;
    vector<Mat> allimgs;
    cv::Mat descriptor;
    cv::Mat img;
    vector<cv::KeyPoint> kpts;
    vector<cv::Point2f> pts2d;
    vector<vector<pair<int, int>>> invertIdx;
    cv::Point3f center;
    
private:

};



#endif
