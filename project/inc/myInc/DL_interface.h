#pragma once
#ifndef DL_INTERFACE_H
#define DL_INTERFACE_H
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <mutex>

#include "torch/script.h"
#include "torch/torch.h"

class DL_module
{
public:
	DL_module(const std::string &module_path,const bool _use_cuda):path(module_path),use_cuda(_use_cuda)
	{
		load();
	}
	DL_module()
	{
	}
	~DL_module()
	{
	}
	DL_module(const DL_module&) = delete;
	DL_module& operator=(const DL_module&) = delete;
	void clone(DL_module& m);
	void single_interface(cv::Mat& image, std::pair<int, float>& class_and_probability);

private:
	std::string path;
	bool use_cuda = false;
	std::mutex mut;
	torch::jit::script::Module model;

	void load();

};




#endif