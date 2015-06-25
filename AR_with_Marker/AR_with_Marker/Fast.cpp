#include "fast.h"

using namespace std;



class parallelTestBody : public ParallelLoopBody//参考官方给出的answer，构造一个并行的循环体类
{
public:
    parallelTestBody(vector<KeyPoint> &queryPts,vector<vector<int>> &bucket,Mat &queryDesp,Mat &trainDesp,vector<DMatch> &dmatch,int width,int height,int blockSize)//class constructor
    {
        this->queryPts = &queryPts;
        this->bucket = &bucket;
        this->dmatch = &dmatch;
        this->queryDesp = &queryDesp;
        this->trainDesp = &trainDesp;
        this->height = height;
        this->width = width;
        this->blockSize = blockSize;
        mask.resize(bucket.size());
        for (int i = 0; i < mask.size(); i++)
        {
            if (bucket[i].empty())
            {
                mask[i] = 0;
            }
            else
            {
                mask[i] = 1;
            }
        }
    }
    void operator()(const Range& range) const//重载操作符（）
    {
        for (int i = range.start; i < range.end; i++)
        {
            int x = (*queryPts)[i].pt.x / 10;
            int y = (*queryPts)[i].pt.y / 10;
            int lx = x - blockSize < 0 ? 0 : x - 1;
            int ly = y - blockSize < 0 ? 0 : y - 1;
            int rx = x + blockSize >= width ? width - 1 : x + 1;
            int ry = y + blockSize >= height ? height - 1 : y + 1;
            int distance = 0xffffff;
            int idx;
            for (int j = lx; j <= rx; j++)
            {
                for (int k = ly; k <= ry; k++)
                {
                    if (mask[k*width + j])
                    {
                        for (auto l : (*bucket)[y*width+x])
                        {
                            int t = computeSSD(queryDesp->row(i), trainDesp->row(l));
                            if (t < distance)
                            {
                                distance = t;
                                idx = l;
                            }
                        }
                    }
                }
            }
            DMatch t;
			if (distance == 0xffffff)
            {
                t.distance = 0xffffff;
                t.trainIdx = -1;
                t.queryIdx = -1;
                (*dmatch)[i] = t;
            }
            else
            {
                t.distance = distance;
                t.queryIdx = i;
                t.trainIdx = idx;
                (*dmatch)[i] = t;
            }
        }
    }
    
private:
    
    __inline int computeSSD(const Mat &a, const Mat&b) const
    {
        char * aa = (char*)a.data;
        char * bb = (char*)b.data;
        int width = a.cols;
        int ret = 0;
        for (int i = 0; i < width; i++)
        {
            ret += (aa[i] - bb[i])*(aa[i] - bb[i]);
        }
        return ret/width;
    }
    vector<KeyPoint> * queryPts;
    vector<vector<int>> *bucket;
    Mat * queryDesp, *trainDesp;
    vector<DMatch> * dmatch;
    int width, height;
    int blockSize;
    vector<int> mask;
};

class parallelDesp : public ParallelLoopBody//参考官方给出的answer，构造一个并行的循环体类
{
public:
    parallelDesp(const uchar *a, vector<KeyPoint> &keypts, Mat &desp,int width,int step,int size)//class constructor
    {
        this->a = a;
        this->keypts = &keypts;
        this->desp = &desp;
        this->width = width;
        this->step = step;
        this->size = size;
    }
    void operator()(const Range& range) const//重载操作符（）
    {
        for (int j = range.start; j < range.end; j++)
        {
            auto &i = (*keypts)[j];
            int x = i.pt.x;
            int y = i.pt.y;
            int lx = x - step;
            int rx = x + step + 1;
            int ly = y - step;
            int ry = y + step + 1;
            //if (lx< 0 || ly <0 || rx >image.cols || ry>image.rows)
            //{
            //	if (hasMask)
            //		mask.at<char>(k) = 0;
            //	keypts.erase(keypts.begin() + j);
            //	j--;
            //	continue;
            //}
            uchar * b = (uchar*)(*desp).row(j).data;
            int ss = 0;
            int sum = 0;
            for (size_t i = lx; i < rx; i++)
            {
                for (size_t j = ly; j < ry; j++)
                {
                    b[ss] = a[j*width + i];
                    sum += b[ss];
                    ss++;
                }
            }
            int mean = sum / size / size;
            char *c = (char*)(*desp).row(j).data;
            for (int i = 0; i < size*size; i++)
            {
                c[i] = b[i] - mean;
            }
        }
    }
    
private:
    
    const uchar * a;
    vector<KeyPoint> *keypts;
    Mat * desp;
    int width, step,size;
};


Fast::Fast(int threshold,int npoints,Size a,int size)
{
    bucket1.resize(a.width / 10 * a.height / 10);
    for (int i = 0; i < bucket1.size(); i++)
    {
        bucket1[i].reserve(50);
    }
    this->size = size;
	this->npoints = npoints;
    detector =  FastFeatureDetector::create(threshold);
}

Fast::~Fast()
{
}

float Fast::getScale(int level, int firstLevel, double scaleFactor)
{
	return (float)std::pow(scaleFactor, (double)(level - firstLevel));
}

void Fast::setPyramid(const Mat &img,const Mat &mask, const int levelsNum, const float scaleFactor, vector<Mat> &imagePyramid,vector<Mat>&maskPyramid)
{
	imagePyramid.resize(levelsNum);
	if (!mask.empty())
		maskPyramid.resize(levelsNum);
	const int border = 5;
	const int firstLevel = 0;
	for (int level = 0; level < levelsNum; ++level)
	{
		float scale = 1 / getScale(level, 0, scaleFactor);
		Size sz(cvRound(img.cols*scale), cvRound(img.rows*scale));
		Size wholeSize(sz.width + border * 2, sz.height + border * 2);
		Mat temp(wholeSize, img.type()), masktemp;
		imagePyramid[level] = temp(Rect(border, border, sz.width, sz.height));
		if (!mask.empty())
		{
			masktemp = Mat(wholeSize, mask.type());
			maskPyramid[level] = masktemp(Rect(border, border, sz.width, sz.height));
		}

		// Compute the resized img
		if (level != firstLevel)
		{
			if (level < firstLevel)
			{
				resize(img, imagePyramid[level], sz, 0, 0, INTER_LINEAR);
			}
			else
			{
				resize(imagePyramid[level - 1], imagePyramid[level], sz, 0, 0, INTER_LINEAR);
				if (!mask.empty())
				{
					resize(maskPyramid[level - 1], maskPyramid[level], sz, 0, 0, INTER_LINEAR);
					//threshold(maskPyramid[level], maskPyramid[level], 254, 0, THRESH_TOZERO);
				}
			}

			copyMakeBorder(imagePyramid[level], temp, border, border, border, border,
				BORDER_REFLECT_101 + BORDER_ISOLATED);
		}
		else
		{
			copyMakeBorder(img, temp, border, border, border, border,
				BORDER_REFLECT_101);
		}
	}
}

void Fast::detect(const Mat &img, vector<KeyPoint> &kpts, const Mat &mask)
{
    detector->detect(img, kpts, mask);
	vector<int> idx;
	KeyPointsFilter::runByANMS(kpts, npoints, idx);

}

void Fast::detect(const cv::Mat &img,const Mat &mask, int levelNum,float scale,vector<vector<cv::KeyPoint>> &allKeyPoints,vector<Mat>&imagePyramid)
{
    allKeyPoints.resize(levelNum);
    int allPoints = 0;
	vector<Mat> maskPyramid;
	setPyramid(img, mask, levelNum, scale, imagePyramid, maskPyramid);
	if (!mask.empty())
	{
		for (size_t i = 0; i < levelNum; i++)
		{
			maskPyramid.push_back(Mat());
		}
	}

    for (int i=0; i<levelNum; i++)
    {
		detector->detect(imagePyramid[i], allKeyPoints[i],maskPyramid[i]);
    }

	for (int i = 0; i<levelNum; i++)
    {
//        float scale_ = getScale(i, 0, scale);
        for(int j=0;j<allKeyPoints[i].size();j++)
        {
            auto &t = allKeyPoints[i][j];
//            t.pt  *= scale_;
            t.octave = i;
        }
    }
    
    
}

void Fast::compute(const Mat &image, vector<KeyPoint> &keypts, Mat &desp)
{
    bool hasMask = false;
    Mat _image;
    if (image.channels() != 1)
    {
        cvtColor(image, _image, CV_BGR2GRAY);
    }
    else
    {
        _image = image;
    }
    Mat_<char> temp(1, size*size);
    int step = (size - 1) / 2;
    KeyPointsFilter::runByImageBorder(keypts, image.size(), 5);
    desp = Mat(keypts.size(), size*size, CV_8U);
    uchar *a = (uchar*)_image.data;
    const int width = _image.cols;
    parallel_for_(Range(0, keypts.size()), parallelDesp(a, keypts, desp, width, step, size));
    //tm.stop();
    //cout << tm.getTimeSec() << endl;
    //tm.reset();
    //tm.start();
    //for (int j = 0, k = 0; j < keypts.size(); j++, k++)
    //{
    //	auto &i = keypts[j];
    //	int x = i.pt.x;
    //	int y = i.pt.y;
    //	int lx = x - step;
    //	int rx = x + step + 1;
    //	int ly = y - step;
    //	int ry = y + step + 1;
    //	//if (lx< 0 || ly <0 || rx >image.cols || ry>image.rows)
    //	//{
    //	//	if (hasMask)
    //	//		mask.at<char>(k) = 0;
    //	//	keypts.erase(keypts.begin() + j);
    //	//	j--;
    //	//	continue;
    //	//}
    //	uchar * a = (uchar*)_image.data;
    //	uchar * b = (uchar*)desp.row(j).data;
    //	int ss = 0;
    //	int sum = 0;
    //	const int width = _image.cols;
    //	for (size_t i = lx; i < rx; i++)
    //	{
    //		for (size_t j = ly; j < ry; j++)
    //		{
    //			b[ss] = a[j*width + i];
    //			sum += b[ss];
    //			ss++;
    //		}
    //	}
    //	int mean = sum / size / size;
    //	char *c = (char*)desp.row(j).data;
    //	for (int i = 0; i < size*size; i++)
    //	{
    //		c[i] = b[i] - mean;
    //	}
    //}
    //tm.stop();
    //cout << tm.getTimeSec() << endl;
    
}

__inline int Fast::computeSSD(const Mat &a, const Mat&b)
{
    char * aa = (char*)a.data;
    char * bb = (char*)b.data;
    int width = a.cols;
    int ret = 0;
    for (int i = 0; i < width; i++)
    {
        ret += (aa[i] - bb[i])*(aa[i] - bb[i]);
    }
    return ret;
}

void Fast::match(const Mat &img,  vector<KeyPoint> &queryPts,  Mat &queryDesp,  vector<KeyPoint> &trainPts,  Mat &trainDesp, vector<DMatch> &dmatches,int blockSize)
{
    dmatches.clear();
    int width = img.cols / 10;
    int height = img.rows / 10;
    dmatches.resize(queryPts.size());
    //	dmatches.reserve(queryPts.size());
    int sss = 0;
    for (int i = 0; i < bucket1.size(); i++)
    {
        bucket1[i].clear();
    }
    int x, y;
    for (int i = 0; i < trainPts.size(); i++)
    {
        x = trainPts[i].pt.x / 10;
        y = trainPts[i].pt.y / 10;
        bucket1[y*width + x].push_back(i);
    }
    int step = (blockSize-1)/2;
    parallel_for_(Range(0, queryPts.size()), parallelTestBody(queryPts, bucket1, queryDesp, trainDesp, dmatches, width, height,step));
    ////	dmatches.clear();
    //	for (int i = 0; i < queryPts.size(); i++)
    //	{
    //		int x = queryPts[i].pt.x / 10;
    //		int y = queryPts[i].pt.y / 10;
    //		int lx = x - 1;
    //		int ly = y - 1;
    //		int rx = x + 1;
    //		int ry = y + 1;
    //		int distance = -1;
    //		int idx;
    //		if (lx>0 && ly > 0 && rx < width&&ry < height)
    //		{
    //			int start = ly*width + lx;
    //			for (int j = 0; j < 9; j++)
    //			{
    //				for (int l = 0; l < bucket1[start+idxx[j]].size(); l++)
    //				{
    //					int s = bucket1[start+idxx[j]][l];
    //					int t = computeSSD(queryDesp.row(i), trainDesp.row(s));
    //					if (t < distance || distance == -1)
    //					{
    //						distance = t;
    //						idx = s;
    //					}
    //				}
    //
    //			}
    //		}
    //		DMatch t;
    //		if (distance == -1)
    //		{
    //			t.distance = -1;
    //			t.trainIdx = -1;
    //			t.queryIdx = -1;
    //			dmatches[i] = t;
    //		}
    //		else
    //		{
    //			t.distance = distance;
    //			t.queryIdx = i;
    //			t.trainIdx = idx;
    //			dmatches[i] = t;
    //		}
    //	}
    //
    
}