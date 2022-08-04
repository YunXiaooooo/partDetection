#pragma once
#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

class imageProcess
{
public:
	imageProcess() 
	{
	
	}
	~imageProcess()
	{
	
	}
	imageProcess(const imageProcess&) = delete;
	imageProcess& operator=(const imageProcess&) = delete;

	std::vector<int> imageProcessTask(cv::Mat& src, const int grabNum);

private:
	void imageProcessFor12(cv::Mat& src, const int grabNum, std::vector<int>& result);



};




#endif
