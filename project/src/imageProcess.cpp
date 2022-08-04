#include "imageProcess.h"
#include <windows.h>
void imageProcess::imageProcessFor12(cv::Mat& src, const int grabNum, std::vector<int>& result)//传入grabNum方便调试查看哪副图片出错
{
	/*图像处理
		1/12托盘一共4个零件按以下标号

		3   2 
		1   0

		vector<int> result规模为4，将每个位置的零件状态存入:
			1表示正常；2表示方向相反；3表示空，没有零件
	*/
	clock_t start = clock();
	cv::imwrite("src" + std::to_string(grabNum) + ".jpg", src);


	Sleep(2000);//ms


	clock_t ends = clock();
	printf("imageProcess Running Time = %f \n", ((double)(ends)-start) / CLOCKS_PER_SEC);
	result.clear();
	result.resize(4, 1);
}
std::vector<int> imageProcess::imageProcessTask(cv::Mat& src, const int grabNum)
{
	cv::Mat srcClone = src.clone(); //避免修改原图
	std::vector<int> DirectionRecognitionResult(4,4);
	try{
		imageProcessFor12(srcClone, grabNum, DirectionRecognitionResult);
	}
	catch (std::exception& e)
	{
		printf("图像处理异常！(%s) \n", e.what());
	}


	for (int i = 0, len = DirectionRecognitionResult.size(); i < len; i++)
	{
		printf("image num = %d, location = %d , state = %d\n", grabNum, i + grabNum * 12, DirectionRecognitionResult[i]);
	}
	return DirectionRecognitionResult;
}