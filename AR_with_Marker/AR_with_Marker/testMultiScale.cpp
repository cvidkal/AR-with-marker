//#include "common.h"
//#include "C:\\Users\Sandy\Documents\Visual Studio 2013\Projects\anms\anms\anms.h"
//#include "C:\\Users\Sandy\Documents\Visual Studio 2013\Projects\anms\anms\getMatch.h"
//
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
//	vector<KeyPoint> kptsa,kptsb;
//	Mat despa,despb;
//	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
//	GetMatch getMatch = GetMatch(dist_ratioCheck);
//	while (c=cvWaitKey(30))
//	{
//		Mat temp;
//		vc >> temp;
//		imshow("now", temp);
//		if (c == 'a')
//		{
//			imga = temp.clone();
//			orb->detectAndCompute(imga, Mat(), kptsa, despa);
//			isSet = true;
//		}
//		if (isSet)
//		{
//			orb->detectAndCompute(temp, Mat(), kptsb, despb);
//			vector<vector<DMatch>> dmatches;
//			matcher->knnMatch(despb, despa, dmatches, 2);
//			vector<DMatch> dmatch;
//			vector<int> idx;
//			getMatch(dmatches, idx);
//			
//		}
//
//	}
//}
//
