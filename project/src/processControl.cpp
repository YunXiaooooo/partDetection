#include "processControl.h"

processControl::processControl() :threadNum(4)
{
	if (pool == nullptr)
	{
		try {
			pool = std::make_unique<ThreadPool>(threadNum);
		}
		catch (std::exception& e)
		{
			printf("New ThreadPool Falue (%s) \n", e.what());
			std::exit(0);
		}
	}
	if (ptrImageprocess == nullptr)
	{
		try {
			ptrImageprocess = std::make_shared<imageProcess>();
		}
		catch (std::exception& e)
		{
			printf("New Imageprocess Failure (%s)\n", e.what());
			std::exit(0);
		}
	}
	testResults.resize(threadNum);
	for (auto& r : testResults)
	{
		r.resize(12);
	}
}


void processControl::waitTestResults()
{
	testResults.clear();
	testResults.resize(taskNum);
	for (auto& r : testResults)
	{
		r.resize(4);
	}

	assert(testFutures.size() == testOrder.size());
	while (!testFutures.empty())
	{
		std::future<std::vector<int>> fut = std::move(testFutures.front());
		size_t i = testOrder.front();
		testFutures.pop();
		testOrder.pop();
		testResults[i] = fut.get();//如果出现多次相同grabNum,抓拍信号只保留最近一次的检测结果
	}
}

std::vector<int> processTask(std::shared_ptr<imageProcess> ptrImageprocess, cv::Mat& img, const int num)
{
	cv::Mat image = img.clone();
	return ptrImageprocess->imageProcessTask(image, num);
}