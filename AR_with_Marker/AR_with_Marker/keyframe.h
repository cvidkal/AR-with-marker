#ifndef __KEYFRAME_H__
#define __KEYFRAME_H__
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include "Feature.h"
#include "fast.h"
using namespace std; 
using namespace cv;
class Keyframe
{
public:
    Keyframe();
    ~Keyframe();
	cvx::SingleScaleStructrue orb;

    cv::Mat img;
    cv::Mat R;
    cv::Mat t;
    Mat CC;//Camera Coorindates
    cv::Mat descriptor;
    vector<cv::Mat> descriptors;
    vector<vector<cv::KeyPoint>> keyPts;
    vector<vector<int>> idxs;
    //	vector<Point3d> pointSet3D;
    vector<pair<int, int>> indices;//(-1,-1) means a invalid index
    vector<int> markerIdx;
    vector<cv::KeyPoint> keyPoints;
    vector<cv::Point2f> pts2D;
    vector<cv::Point3f> pts3D;

	
private:

};



#endif
