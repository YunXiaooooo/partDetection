#include "imageProcess.h"
#include "common.h"
std::vector<int> imageProcessFor12(cv::Mat src, int grabNum)//传入grabNum方便调试查看哪副图片出错
{
	/*图像处理
		1/4托盘一共12个零件按以下标号

		0    1   2    3 
		4    5   6    7
		8    9   10   11

		将方向不正确的零件标号加入vector<int> result 内返回。
	*/
	std::vector<int> result;
	clock_t start = clock();	
	cv::imwrite("src"+std::to_string(grabNum)+".jpg", src);

	



	clock_t ends = clock();
	printf("imageProcess Running Time = %f \n", ((double)(ends) - start) / CLOCKS_PER_SEC);
	result.push_back(2+ grabNum);
	result.push_back(3+ grabNum);
	result.push_back(8+ grabNum);
	return result;
}

//void imageProcessTask(cv::Mat src, int grabNum)
//{
//	cv::Mat srcClone = src.clone();
//	std::vector<int> DirectionRecognitionResult;
//	DirectionRecognitionResult = imageProcessFor12(srcClone, grabNum);
//
//
//	unsigned int tempResultCode = 0;
//	for (auto r : DirectionRecognitionResult)
//	{
//		tempResultCode = tempResultCode | (1 << r);
//	}
//	printf("image num = %d, resultCode = 0x%x = %d \n", grabNum, tempResultCode, tempResultCode);
//	resultCode[grabNum].setCodeValue(tempResultCode);
//}
unsigned int imageProcessTask(cv::Mat src, int grabNum)
{
	cv::Mat srcClone = src.clone();
	std::vector<int> DirectionRecognitionResult;
	DirectionRecognitionResult = imageProcessFor12(srcClone, grabNum);


	unsigned int tempResultCode = 0;
	for (auto r : DirectionRecognitionResult)
	{
		tempResultCode = tempResultCode | (1 << r);
	}
	printf("image num = %d, resultCode = 0x%x = %d \n", grabNum, tempResultCode, tempResultCode);
	return tempResultCode;
}
