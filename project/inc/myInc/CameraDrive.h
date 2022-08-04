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
	// ��ȡ�ⲿ������������
	const int getExternalCameraNum() ;

	// ��ʼ���������ΪӲ������
	bool camerasInitInHardTriggerMode();

	// ��ʼ����ץͼ�ص�����
	bool startGrabForExternalTrigger() const;

	// ��ʼ���������Ϊ�������
	bool camerasInitInSoftTriggerMode();

	// ���ץͼ��ǰ���Ǳ���Ҫ���������ʼ��
	cv::Mat grabForSoftTriggerMode(const int cameraIndex) const;

	// �ر����
	bool camerasClose() const;

private:

	bool PrintDeviceInfo(MV_CC_DEVICE_INFO* pstMVDevInfo) const;

	// convert data stream in Mat format
	bool Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char * pData, cv::Mat & srcImg) const;

	int RGB2BGR(unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight) const;

	std::vector<void*> handle;	// ���������
	std::vector<unsigned int> g_nPayloadSize;	// ��������Ĵ�С
};

#endif