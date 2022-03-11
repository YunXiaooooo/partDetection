#ifndef _IMAGEPROCESS_H_
#define _IMAGEPROCESS_H_
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

std::vector<int> imageProcessFor12(cv::Mat src, int grabNu);
unsigned int imageProcessTask(cv::Mat src, int grabNu);
#endif