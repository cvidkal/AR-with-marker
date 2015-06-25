#ifndef __FAST_H_
#define __FAST_H_

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class FastDescriptorExtractor
{
public:
    FastDescriptorExtractor(int size);
    ~FastDescriptorExtractor();
    void compute(const Mat &image, vector<KeyPoint> &keypts, Mat &desp);
private:
    char * patch;
    int size;
};

class FastDescriptorMatcher
{
public:
    FastDescriptorMatcher(cv::Size a){
        bucket1.resize(a.width / 10 * a.height / 10);
        for (int i = 0; i < bucket1.size(); i++)
        {
            bucket1[i].reserve(50);
        }
    };
    ~FastDescriptorMatcher(){};
    void match(const Mat &img,  vector<KeyPoint> &queryPts,  Mat &queryDesp,  vector<KeyPoint> &trainPts,  Mat &trainDesp, vector<DMatch> &dmatches);
private:
    __inline int computeSSD(const Mat &a,const  Mat&b);
    vector<vector<int>> bucket1;
    
};




class Fast
{
public:
    Fast(int threshold,int npoints,cv::Size a,int blocksize = 9);
    ~Fast();
    void detect(const Mat &img, vector<KeyPoint> &kpts, const Mat &mask = Mat());
	void Fast::detect(const cv::Mat &img, const Mat &mask, int levelNum, float scale, vector<vector<cv::KeyPoint>> &allKeyPoints, vector<Mat>&imagePyramid);
	void compute(const Mat &img, vector<KeyPoint> &kpts, Mat &desp);
    void match(const Mat &img,  vector<KeyPoint> &queryPts,  Mat &queryDesp,  vector<KeyPoint> &trainPts,  Mat &trainDesp, vector<DMatch> &dmatches,int blockSize = 3);
private:
    __inline int computeSSD(const Mat &a, const  Mat&b);
	float getScale(int level, int firstLevel, double scaleFactor);
	void setPyramid(const Mat &img, const Mat &mask, const int levelsNum, const float scaleFactor, vector<Mat> &imagePyramid, vector<Mat>&maskPyramid);
	Ptr<FeatureDetector> detector;
    vector<vector<int>> bucket1;
    int size;
	int npoints;
};


#endif