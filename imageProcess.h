#pragma once
#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <algorithm>
struct boxCoordinates
{
	int startRows;
	int endRows;
	int startCols;
	int endCols;
};


class imageProcess
{
public:
	imageProcess() 
	{
		baseDInit();
	}
	~imageProcess()
	{
		baseDSave();
	}
	imageProcess(const imageProcess&) = delete;
	imageProcess& operator=(const imageProcess&) = delete;

	std::vector<int> imageProcessTask(cv::Mat& src, const int grabNum);
	void baseDSave();

private:
	const std::array<double,3> cammeraAngle{26.0,36.6,46.4};

	const struct boxCoordinates locationForROI { 1024 - 470, 1024 + 470, 1224 - 450, 1224 + 450 };

	const double ratio = 420.0/434;
	const int OTSU_threadNumMin = 65;//4个零件一起进行otsu二值化，如果阈值过低，认为4个孔位全部没有零件
	const unsigned int deadNoPartHoleNum = 13000;//白点数量小于该值认为该位置没有零件
	const double minDHoleArea = 150, maxDHoleArea = 700;//超过这个面积认为轮廓不可能位D形孔
	float lengthCompareWidth = 3.0;
	std::array<cv::Point2f, 4> baseD{ cv::Point2f(327,141),cv::Point2f(305,586), cv::Point2f(326,796), cv::Point2f(307,1236) };
	std::array<cv::Point2f, 2> singleBaseD{ cv::Point2f(341,117),cv::Point2f(342,512) };

	double judgementDeadValue = 0;

private:
	void realImageProcess(cv::Mat& src, const int grabNum, std::vector<int>& result);
	void imagePerspective(cv::Mat& src, const int grabNum);
	cv::Mat getSumOfEachRowOrColumnForBinaryImage(const cv::Mat& src, bool forRows);
	unsigned int getSumOfBinaryImage(const cv::Mat& src);
	void getTwoContactDHoleCoordinates(const cv::Mat& image, std::vector<cv::Point2i>& DCoordinates);
	void getFourContactDHoleCoordinates(const cv::Mat& image, std::vector<cv::Point2i>& DCoordinates, const int imageIndx);
	double getDistance(const cv::Point2f& p1, const cv::Point2f& p2);
	void rotateContact(cv::Mat& contactImage, const cv::Point2i& center);
	cv::Mat RotateImage(const cv::Mat& src, const double angle, const int color, const cv::Point2i& center);
	double getVarianceOfWhitePointDistributionForColumnProjection(const cv::Mat& image);
	cv::Point2i getHoleCoordinate(const cv::Mat& image);
	std::pair<int, int> getLeftAndRightHalfValue(const cv::Mat& image, const bool isUp);
	void leftOrRightJudgment(const std::pair<int, int>& upValue, const std::pair<int, int>& downValue, std::vector<int>& result, int holeNum);
	void getHoleImageCoordinates(std::vector<struct boxCoordinates>& holeImageCoordinates, const std::vector<struct boxCoordinates>& doubleHoleImageCoordinates, double base_thread,
		const cv::Mat& grayRoiAboutTwentyParts, const cv::Mat& binaryRoiAboutTwentyParts);
	void setDemarcationLineCoordinates(std::vector<struct boxCoordinates>& holeImageCoordinates,
		const std::vector<struct boxCoordinates>& doubleHoleImageCoordinates, const size_t imageIndx, const int cur);
	void saveImage(const std::string& name, const cv::Mat& src);
	void showImage(const std::string& name, const cv::Mat& src, const int namewindow, const bool wait);
	double getRoiAboutFourParts(const cv::Mat& src, cv::Mat& grayRoiAboutTwentyParts, cv::Mat& binaryRoiAboutTwentyParts);
	double getLengthCompareWidth(const cv::RotatedRect& box);
	void drawGrayHist(const cv::Mat& src);
	void sharpenImage1(const cv::Mat& image, cv::Mat& result);
	int nearestD(const cv::Point2i& p, double& minValue);
	int nearestSingleD(const cv::Point2i& p, double& minValue);
	void baseDInit();

};




#endif
