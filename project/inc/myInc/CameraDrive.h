#pragma once
#ifndef CAMERADRIVE_H
#define CAMERADRIVE_H

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "MvCameraControl.h"

#include <sys/timeb.h>

class CameraDrive {
public:
	CameraDrive();
	~CameraDrive();
	CameraDrive(const CameraDrive&) = delete;
	CameraDrive& operator=(const CameraDrive&) = delete;
	// 获取外部接入的相机数量
	const int getExternalCameraNum() ;

	// 初始化所有相机为硬件触发
	bool camerasInitInHardTriggerMode();

	// 开始启动抓图回调函数
	bool startGrabForExternalTrigger() const;

	// 初始化所有相机为软件触发
	bool camerasInitInSoftTriggerMode();

	// 软件抓图，前提是必须要软件触发初始化
	cv::Mat grabForSoftTriggerMode(const int cameraIndex) const;

	// 关闭相机
	bool camerasClose() const;

private:

	bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo) const;

	// convert data stream in Mat format
	bool Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char * pData, cv::Mat & srcImg) const;

	int RGB2BGR(unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight) const;

	std::vector<void*> handle;	// 存放相机句柄
	std::vector<unsigned int> g_nPayloadSize;	// 各相机包的大小
};

#endif