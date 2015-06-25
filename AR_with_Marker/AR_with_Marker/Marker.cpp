//
//  Marker.cpp
//  newAR
//
//  Created by ZJU_CVG on 15/1/29.
//  Copyright (c) 2015年 Shangjin Zhai. All rights reserved.
//

#include "Marker.h"


Marker::Marker()
{
    registed = false;
}

Marker::~Marker()
{
}

Marker::Marker(const Marker& m)
{
    points3D1 = m.points3D1;
    points3D2 = m.points3D2;
    descriptor = m.descriptor.clone();
    keypoints = m.keypoints;
    registed = m.registed;
    pts2d = m.pts2d;
    invertIdx = m.invertIdx;
    center = m.center;
    img = m.img.clone();
    opts   = m.opts;
    allimgs = m.allimgs;
}


Marker::Marker(const Mat&img, const Mat& R,Ptr<Fast> fast, Ptr<cv::ORB> orb,int levelNum,float scaleFactor)
{
	this->img = img.clone();
	fast->detect(img, Mat(), levelNum, scaleFactor, fastStructure.allKeypoints,fastStructure.imagePyramid);
	orb->detectAndCompute(img, Mat(), orbStructure.kpts, orbStructure.desp);
}

void Marker::reset()
{
//    invertIdx.clear();
//    invertIdx.resize(keypoints.size());
    
}
