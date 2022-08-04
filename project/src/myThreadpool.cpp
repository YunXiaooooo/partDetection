#include "myThreadPool.h"

std::vector<int> task(std::shared_ptr<imageProcess> ptrImageprocess, cv::Mat &img, int num)
{
	cv::Mat image = img.clone();
	return ptrImageprocess->imageProcessTask(image, num);
}