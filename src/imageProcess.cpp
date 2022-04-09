#include "imageProcess.h"
#include "common.h"
void imageProcessFor12(cv::Mat src, int grabNum, std::vector<int>& result)//传入grabNum方便调试查看哪副图片出错
{
	/*图像处理
		1/4托盘一共12个零件按以下标号

		0    1   2    3 
		4    5   6    7
		8    9   10   11

		vector<int> result规模为12，将每个位置的零件状态存入:
			1表示正常；2表示方向相反；3表示空，没有零件
	*/
	clock_t start = clock();	
	cv::imwrite("src"+std::to_string(grabNum)+".jpg", src);

	



	clock_t ends = clock();
	printf("imageProcess Running Time = %f \n", ((double)(ends) - start) / CLOCKS_PER_SEC);
	for (auto &r : result)
	{
		static int c = 0;
		c++;
		c = (c > 3) ? 0 : c;
		r = c;
	}
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
std::vector<int> imageProcessTask(cv::Mat src, int grabNum)
{
	cv::Mat srcClone = src.clone();
	std::vector<int> DirectionRecognitionResult(12);
	imageProcessFor12(srcClone, grabNum, DirectionRecognitionResult);


	for (int i=0,len= DirectionRecognitionResult.size();i<len;i++)
	{
		printf("image num = %d, location = %d , state = %d\n", grabNum, i+ grabNum*12, DirectionRecognitionResult[i]);
	}
	return DirectionRecognitionResult;
}
