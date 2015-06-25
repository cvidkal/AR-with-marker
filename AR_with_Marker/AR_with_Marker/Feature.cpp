#include "Feature.h"



//cvx::Feature::Feature(const Mat &img, cvx::FeatureParams featureParams,const Mat &mask)
//{
//	switch (featureParams.type)
//	{
//	case cvx::FAST:
//		detector = FastFeatureDetector::create(featureParams.threshold);
//		break;
//	case cvx::AGAST:
//		detector = AgastFeatureDetector::create(featureParams.threshold);
//		break;
//	case cvx::ORB:
//		detector = ORB::create(featureParams.npoints, featureParams.scaleFactor, featureParams.levelNum);
//		break;
//	default:
//		break;
//	}
//	
//	switch (featureParams.scaleType)	
//	{
//	case cvx::singleScale:
//		detector->detect(img,structure[0].kpts,mask);
//	case cvx::multiScale:
//		for (size_t i = 0; i < featureParams.scaleFactor; i++)
//		{
//			detector->detect()
//		}
//	default:
//		break;
//	}
//
//}
