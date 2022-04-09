#ifndef _IMAGEPROCESS_H_
#define _IMAGEPROCESS_H_
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

void imageProcessFor12(cv::Mat src, int grabNum, std::vector<int>& result);
std::vector<int> imageProcessTask(cv::Mat src, int grabNum);
#endif