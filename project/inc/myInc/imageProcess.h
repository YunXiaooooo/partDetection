#pragma once
#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include <algorithm>
#include "local_parameter.h"
#include <string>

//#include "DL_interface.h"
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
		param_file.init("config_param");
		D_hole_XY_in_two_part_image.init("D1_XY_in_two_part_image,D2_XY_in_two_part_image,D3_XY_in_two_part_image,D4_XY_in_two_part_image", param_file);
		D_hole_XY_in_single_part_image.init("D1_XY_in_single_part_image,D2_XY_in_single_part_image", param_file);

	}
	~imageProcess()
	{
		param_save();
	}
	imageProcess(const imageProcess&) = delete;
	imageProcess& operator=(const imageProcess&) = delete;

	std::vector<int> imageProcessTask(cv::Mat& src, const int grabNum);
	void param_save()
	{
		D_hole_XY_in_two_part_image.update_to_file(param_file);
		D_hole_XY_in_two_part_image.update_to_file(param_file);
		param_file.save_file();
	}

private:
	parameter_file param_file;
	const std::array<double, 3> cammeraAngle{ 26.0,36.6,46.4 };
	
	D_XY_base D_hole_XY_in_two_part_image, D_hole_XY_in_single_part_image;
	const struct boxCoordinates locationForROI { 1024 - 470, 1024 + 470, 1224 - 450, 1224 + 450 };

	const double ratio = 420.0 / 434;
	const int OTSU_threadNumMin = 30;//4�����һ�����otsu��ֵ���������ֵ���ͣ���Ϊ4����λȫ��û�����
	const unsigned int deadNoPartHoleNum = 6000;//�׵�����С�ڸ�ֵ��Ϊ��λ��û�����
	const double minDHoleArea = 150, maxDHoleArea = 700;//������������Ϊ����������λD�ο�
	float lengthCompareWidth = 3.0;
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
	int nearestD(const cv::Point2i& p, double& minValue, const D_XY_base& base);

};




#endif
