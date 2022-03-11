#include "CameraDrive.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "common.h"

void grabByHk(CameraDrive* ptrMyCameras)
{
	while (1)
	{
		if (terminationFlag.readFlag() == true)
			break;//退出线程
		if (grabFlag.readFlag() == true)
		{
			cv::Mat img = ptrMyCameras->grabForSoftTriggerMode(0);//传入0是因为仅有一个相机


		}
	}
}