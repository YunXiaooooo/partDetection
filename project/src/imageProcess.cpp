#include "imageProcess.h"
#include <windows.h>
void imageProcess::imageProcessFor12(cv::Mat& src, const int grabNum, std::vector<int>& result)//����grabNum������Բ鿴�ĸ�ͼƬ����
{
	/*ͼ����
		1/12����һ��4����������±��

		3   2 
		1   0

		vector<int> result��ģΪ4����ÿ��λ�õ����״̬����:
			1��ʾ������2��ʾ�����෴��3��ʾ�գ�û�����
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
	cv::Mat srcClone = src.clone(); //�����޸�ԭͼ
	std::vector<int> DirectionRecognitionResult(4,4);
	try{
		imageProcessFor12(srcClone, grabNum, DirectionRecognitionResult);
	}
	catch (std::exception& e)
	{
		printf("ͼ�����쳣��(%s) \n", e.what());
	}


	for (int i = 0, len = DirectionRecognitionResult.size(); i < len; i++)
	{
		printf("image num = %d, location = %d , state = %d\n", grabNum, i + grabNum * 12, DirectionRecognitionResult[i]);
	}
	return DirectionRecognitionResult;
}