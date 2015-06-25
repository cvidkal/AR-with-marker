//#include "common.h"
//#include <anms.h>
//#include <getMatch.h>
//#include <draw.h>
//
//int main()
//{
//	Ptr<ORB> orb = ORB::create();
//	Mat imga, imgb;
//	VideoCapture vc(1);
//	vc >> imga;
//	cvWaitKey(50);
//	char c;
//	bool isSet = false;
//	vector<KeyPoint> kptsa, kptsb;
//	Mat despa, despb;
//	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
//	GetMatch getMatch = GetMatch(dist_ratioCheck);
//	while (c = cvWaitKey(30))
//	{
//		Mat temp;
//		vc >> temp;
//		if (c == 'a')
//		{
//			
//			imga = temp.clone();
//			orb->detectAndCompute(imga, Mat(), kptsa, despa);
//			isSet = true;
//			continue;
//		}
//		if (isSet)
//		{
//			orb->detectAndCompute(temp, Mat(), kptsb, despb);
//			vector<vector<DMatch>> dmatches;
//			matcher->knnMatch(despb, despa, dmatches, 2);
//			vector<DMatch> dmatch;
//			vector<int> idx;
//			getMatch(dmatches, idx);
//			getMatch.GetDMatch(dmatches, idx, dmatch);
//			vector<Point2f> query, train;
//			getMatch.preparePoint2D(kptsb, kptsa, dmatch, query, train);
//			Mat mask;
//			if (query.size() > 8)
//			{
//				findFundamentalMat(query, train, mask);
//				getMatch.GetDMatch(dmatch, mask);
//				temp = drawTrackLine(temp, kptsa, kptsb, dmatch);
//			}
//				std::ostringstream str;
//				str << "match ratio" << (double)dmatch.size() / dmatches.size();
//				temp = drawInfo(temp, str.str(), Point2i(100, 100));
//			
//		}
//		imshow("now", temp);
//	}
//}
//
