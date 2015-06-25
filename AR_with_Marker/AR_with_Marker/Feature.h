#ifndef __FEATURE_H_
#define __FEATURE_H_

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

/*
	This is a class describing 2D feature used for tracking in AR.
	At the begining, I will first implement FAST/AGAST corner and SSD descriptor.
	The most important module I want to implement is the Gaussian-pyramid multi-scale feature.
*/
namespace cvx
{
	enum ScaleType
	{
		singleScale, multiScale,
	};

	enum FeatureType
	{
		FAST, ORB, AGAST,
	};

	struct SingleScaleStructrue
	{
		Mat img;
		vector<KeyPoint> kpts;
		Mat desp;
	};

	struct MultiScaleStruct
	{
		Mat img;
		vector<vector<KeyPoint>> allKeypoints;
		vector<Mat> imagePyramid;
		vector<Mat> allDesps;
	};

	struct FeatureParams
	{
		FeatureType type;
		int threshold;
		int npoints;
		ScaleType scaleType;
		int levelNum;
		float scaleFactor;
		FeatureParams(FeatureType type_, int threshold_, int npoints_,ScaleType scaleType=singleScale,int levelNum_=1,float scaleFactor_=1.0) :type(type_), threshold(threshold_), npoints(npoints_),levelNum(levelNum_),scaleFactor(scaleFactor_){};
		FeatureParams();
	};


	class Feature
	{
	private:
		vector<SingleScaleStructrue> structure;
		Ptr<FeatureDetector> detector;
		Ptr<DescriptorExtractor> extractor;
		FeatureParams featureParams;
		vector<Mat> imagePyramid;
		vector<Mat> maskPyramid;
	public:
		Feature(){};
//		Feature(const Mat &img, FeatureParams featureParams_, const Mat &mask = Mat());
	};

}

#endif