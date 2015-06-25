#include "common.h"
#include <anms.h>
#include <getMatch.h>
#include <draw.h>
#include "fast.h"
#include "Marker.h"



double scaleFromHomography(const Mat &H)
{
	Mat invH, H2;
	invert(H, invH);
	SVD a(H.colRange(0, 2).rowRange(0, 2), SVD::NO_UV);
	return 1 / sqrt(a.w.at<double>(0)*a.w.at<double>(1));
}


int main()
{
	Ptr<cv::ORB> orb = ORB::create();
	Mat imga, imgb;
	VideoCapture vc(1);
	vc >> imga;
	Ptr<Fast> fast = new Fast(10, 500, imga.size());

	cvWaitKey(50);
	char c;
	bool isSet = false;
	vector<KeyPoint> kptsa, kptsb;
	Mat despa, despb;
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");
	GetMatch getMatch = GetMatch(dist_ratioCheck);
	Marker m;
	SingleScaleStructrue last;
	SingleScaleStructrue now;
	vector<vector<DMatch>> dmatches;
	vector<DMatch> dmatch;
	
	bool isStart = true;
	while (c = cvWaitKey(30))
	{
		Mat temp;
		vc >> temp;
		if (c == 'a')
		{
			m = Marker(temp, Mat(), fast, orb);
			isSet = true;
			continue;
		}
		if (isSet)
		{
			if (!isStart)
			{
				now.img = temp;
				orb->detectAndCompute(now.img, Mat(), now.kpts, now.desp);
				matcher->knnMatch(now.desp, last.desp, dmatches, 2);
				vector<int> idx;
				GetMatch::GetInliers(dmatches, idx,0.6,90);
				GetMatch::GetDMatch(dmatch, idx);
				vector<Point2f> query, train;
				GetMatch::preparePoint2D(now.kpts, last.kpts, dmatch, query, train);
				Mat mask;
				Mat H = findHomography(query, train, mask, CV_RANSAC);
				double t = scaleFromHomography(H);
				int step = log(t) / log(1.25);

				warpPerspective(m.fastStructure.imagePyramid[step], last.img, H, m.img.size());
				fast->detect(last.img, last.kpts);
				fast->compute(last.img, last.kpts, last.desp);
				fast->detect(now.img, now.kpts);
				fast->compute(now.img, now.kpts, now.desp);
				fast->match(now.img, now.kpts, now.desp, last.kpts, last.desp, dmatch);
				GetMatch::GetInliers(dmatch, idx,90);
				GetMatch::preparePoint2D(now.kpts, m.kpts, dmatch, query, train);
				findHomography(query, train, mask, CV_RANSAC);
				
			}
			fast->detect(temp, now.kpts);
			fast->compute(temp, now.kpts, now.desp);


		}
		imshow("now", temp);
	}
}

