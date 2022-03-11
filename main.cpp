#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <algorithm>
#include <numeric>
#include <time.h>
#include <cstdlib>

std::ofstream txtcout("temp.txt");

void getPartRowImageFromSrc(cv::Mat src, std::vector<cv::Mat>& partRowImages, std::vector<cv::Mat>& partRowImagesSrc, const int partRowNum);
void getPartImageFromRow(std::vector<cv::Mat>& partImagesSrc, std::vector<cv::Mat>& partRowImages, std::vector<cv::Mat>& partRowImagesSrc);
cv::Mat getSumOfEachRowOrColumnForBinaryImage(cv::Mat src, bool forRows);
double getStdevOfVector(std::vector<int>& resultSet);
void getContactImage(std::vector<cv::Mat>& partImagesSrc, std::vector<std::vector<cv::Mat>>& contactPairImages);
void getDirectionOfOpening(cv::Mat contactImage, double& diff, int& direction);

int main()
{
	std::string path = "./../../image/";
	for (int num = 1; num < 10; num++)
	{
		//num = 7; 
		cv::Mat src = cv::imread(path + std::to_string(num) + ".bmp");
		if (src.channels() != 1)
		{
			cvtColor(src, src, CV_BGR2GRAY);
		}
		//cv::namedWindow("src", 0);
		//cv::imshow("src", src);
		//cv::waitKey(0);
		printf("********************************  %d.bmp  ************************ \n", num);
		clock_t start;
		start = clock();

		const int partRowNum = 1;//2022-3-7当前测试图片只有1行零件
		std::vector<cv::Mat> partRowImages, partRowImagesSrc;
		getPartRowImageFromSrc(src, partRowImages, partRowImagesSrc, partRowNum);

		clock_t finish = clock();
		printf("getPartRowImage time :%d ms \n", static_cast<unsigned int>(finish - start));

		std::vector<cv::Mat> partImagesSrc;
		getPartImageFromRow(partImagesSrc, partRowImages, partRowImagesSrc);
		finish = clock();
		printf("getPartImage time :%d ms \n", static_cast<unsigned int>(finish - start));

		//对每个零件取出两个触头 一个触头像素大致在90*90
		std::vector<std::vector<cv::Mat>> contactPairImages;
		getContactImage(partImagesSrc, contactPairImages);
		finish = clock();
		printf("getContactImage time :%d ms \n", static_cast<unsigned int>(finish - start));


		for (int i=0;i< contactPairImages.size();i++)
		{
			cv::Mat leftContactImage = contactPairImages[i][0].clone();
			cv::Mat rightContactImage = contactPairImages[i][1].clone();
			//cv::namedWindow("leftContactImage", 0);
			//cv::imshow("leftContactImage", leftContactImage);
			//cv::namedWindow("rightContactImage", 0);
			//cv::imshow("rightContactImage", rightContactImage);
			//cv::waitKey(1);
			cv::transpose(leftContactImage, leftContactImage);//转置，左触头开口向下变成开口向右
			//装置加y轴同时翻转，右触头开口向下变成开口向右
			cv::transpose(rightContactImage, rightContactImage);
			cv::flip(rightContactImage, rightContactImage, 0);


			double leftDiff,rightDiff;
			int leftDirection, rightDirection;
			getDirectionOfOpening(leftContactImage, leftDiff, leftDirection);
			getDirectionOfOpening(rightContactImage, rightDiff, rightDirection);

			int direction;
			if (leftDirection == 0 && rightDirection == 0)
			{
				direction = 0;
				printf("double open on the up \n");
			}
			else if (leftDirection == 1 && rightDirection == 1)
			{
				direction = 1;
				printf("double open on the down \n");
			}
			else
			{
				if (i % 4 < 2)
				{
					direction = leftDirection;
				}
				else
				{
					direction = rightDirection;
				}
			}
			if (direction == 0)
			{
				printf("open on the up \n");
			}
			else if (direction == 1)
			{
				printf("open on the down \n");
			}

			
			//cv::waitKey(0);
		}
		finish = clock();
		printf("test all time :%d ms \n", static_cast<unsigned int>(finish - start));
	}




	return 0;
}





void getPartRowImageFromSrc(cv::Mat src, std::vector<cv::Mat> &partRowImages, std::vector<cv::Mat> &partRowImagesSrc, const int partRowNum)
{
	//图像预处理
	//顶帽去除圆孔内亮
	//clock_t start = clock();
	cv::Mat imgBinary, imageTophat;
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(32, 32));
	morphologyEx(src, imageTophat, cv::MORPH_TOPHAT, kernel);
	cv::threshold(imageTophat, imgBinary, 0, 255, CV_THRESH_OTSU);
	//clock_t finish = clock();
	//printf("MORPH_RECT time :%d ms \n", static_cast<unsigned int>(finish - start));
	//cv::namedWindow("imgBinary", 0);
	//cv::imshow("imgBinary", imgBinary);
	//cv::waitKey(0);

	//对行求和寻找零件y坐标   
	cv::Mat multiResult = getSumOfEachRowOrColumnForBinaryImage(imgBinary, true);
	//cv::namedWindow("multiResult", 0);
	//cv::imshow("multiResult", multiResult);
	//cv::waitKey(0);

	double maxVal = 0; //最大值一定要赋初值，否则运行时会报错
	cv::minMaxLoc(multiResult, NULL, &maxVal, NULL, NULL);
	double* ptrMultiResult = multiResult.ptr<double>(0);
	std::vector<std::pair<int, int>> rowWithLargeValue;
	double thrdTemp = maxVal / 2;
	for (int i = 0; i < multiResult.cols; i++)//保留超过最大值一半的行，用来后面排序找到partRowNum个峰值
	{
		if (ptrMultiResult[i] > thrdTemp)
		{
			rowWithLargeValue.emplace_back(i, ptrMultiResult[i]);
		}
	}
	
	std::sort(rowWithLargeValue.begin(), rowWithLargeValue.end(), [&](auto x, auto y) {return x.second > y.second; });//降序
	std::vector<std::pair<int, int>> exceptWidth;//用于存储已经找到峰值的区间，不再重复计算，以防多个最大值对应同一行零件
	for (int i = 0; i < rowWithLargeValue.size(); i++)
	{
		bool exceptFlag = true;
		for (int j = 0; j < exceptWidth.size(); j++)//遍历确定当前值所在的行是否可以看作新的一行零件
		{
			if (exceptWidth[j].first < rowWithLargeValue[i].first && rowWithLargeValue[i].first < exceptWidth[j].second)
			{
				exceptFlag = false;
			}
		}
		if (exceptFlag == true)
		{
			int startRow = rowWithLargeValue[i].first - 200, endRow = rowWithLargeValue[i].first + 200;//初步范围宽度200+200
			//边界检查,避免越界
			startRow = startRow > 0 ? startRow : 0;
			endRow = endRow < multiResult.cols ? endRow : multiResult.cols;


			//向上寻找黑色边界
			for (int r = rowWithLargeValue[i].first; r > startRow; r--)
			{
				if (ptrMultiResult[r] <= 5 && ptrMultiResult[r + 1] <= 5 &&
					ptrMultiResult[r + 2] <= 5 && ptrMultiResult[r + 3] <= 5 &&
					ptrMultiResult[r + 4] <= 5 && ptrMultiResult[r + 5] <= 5)//这里使用+，确保r+5是已经由r访问过的元素，不会产生越界
				{
					startRow = r;
					break;
				}
			}
			//向下寻找黑色边界
			for (int r = rowWithLargeValue[i].first; r < endRow; r++)
			{
				if (ptrMultiResult[r] <= 5 && ptrMultiResult[r - 1] <= 5 &&
					ptrMultiResult[r - 2] <= 5 && ptrMultiResult[r - 3] <= 5 &&
					ptrMultiResult[r - 4] <= 5 && ptrMultiResult[r - 5] <= 5)//这里的-与上同理，for循环已对应变换为++
				{
					endRow = r;
					break;
				}
			}
			partRowImages.emplace_back(imgBinary(cv::Range(startRow, endRow), cv::Range(0, imgBinary.cols)));
			partRowImagesSrc.emplace_back(src(cv::Range(startRow, endRow), cv::Range(0, imgBinary.cols)));
			exceptWidth.emplace_back(rowWithLargeValue[i].first - 200, rowWithLargeValue[i].first + 200);
			//cv::namedWindow("partRowImage", 0);
			//cv::imshow("partRowImage", partRowImages[partRowImages.size()-1]);
			//cv::waitKey(0);
			printf("centerOfCircleCoordinateY[%d] = %d\n", i, rowWithLargeValue[i].first);
			if (partRowImages.size() == partRowNum)
				break;
		}
	}
	if (partRowImages.size() != partRowNum)
	{
		printf("partRowImages.size() = %d. it's wrong \n", partRowImages.size());
		std::abort();
	}

}


void getPartImageFromRow(std::vector<cv::Mat> &partImagesSrc, std::vector<cv::Mat>& partRowImages, std::vector<cv::Mat>& partRowImagesSrc)
{
	for (int partRowImagesNum = 0; partRowImagesNum < partRowImages.size(); partRowImagesNum++)
	{
		cv::Mat partRowImage = partRowImages[partRowImagesNum];
		cv::Mat partRowImageSrc = partRowImagesSrc[partRowImagesNum];
		partRowImage.convertTo(partRowImage, CV_8U, 255);
		cv::transpose(partRowImage, partRowImage);//转置，行变列，列变行

		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
		morphologyEx(partRowImage, partRowImage, cv::MORPH_OPEN, kernel);//开运算去除游离在外的小白点
		//cv::namedWindow("partRowImage", 0);
		//cv::imshow("partRowImage", partRowImage);
		//cv::waitKey(0);
		uchar* ptrPartRowImage = partRowImage.ptr<uchar>(0);			// 行图像的指针  
		std::vector<std::pair<int, int>> width;
		for (int i = 0; i < partRowImage.rows; i++)//统计图像的白色的跨度，跨度最大行的是转置前图像的圆心列坐标
		{
			int start = 0, end = partRowImage.cols;
			ptrPartRowImage = partRowImage.ptr<uchar>(i);
			for (int j = 0; j < partRowImage.cols / 2; j++)
			{
				if (start == 0 && ptrPartRowImage[j] == 255 && ptrPartRowImage[j + 1] == 255)//j+1最大为 partRowImage.cols / 2，不会越界
				{
					start = j;
				}
				if (end == partRowImage.cols && ptrPartRowImage[partRowImage.cols - j] == 255 && ptrPartRowImage[partRowImage.cols - j - 1] == 255)//j+1最大为 partRowImage.cols / 2,差值最小为0，不会越界
				{
					end = partRowImage.cols - j;
				}
				if (start != 0 && end != partRowImage.cols)
				{
					width.emplace_back(i, end - start);
					//printf("第%d行 跨度=%d \n", i, end - start);
					break;
				}
			}
			//如果遍历结果仍然没有找到白色，说明该行全黑，没有必要加入width。
		}

		std::sort(width.begin(), width.end(), [&](auto x, auto y) {return x.second > y.second; });//降序 ，越大的值越先确定所属零件
		std::vector<std::pair<int, int>> exceptWidth;
		std::vector<int> CoordinateX;
		for (int i = 0; i < width.size(); i++)
		{
			bool exceptFlag = true;
			for (int j = 0; j < exceptWidth.size(); j++)//邻近区域内的较大值对应同一个零件
			{
				if (exceptWidth[j].first < width[i].first && width[i].first < exceptWidth[j].second)
				{
					exceptFlag = false;
				}
			}
			if (exceptFlag == true)
			{
				CoordinateX.emplace_back(width[i].first);
				exceptWidth.emplace_back(width[i].first - 50, width[i].first + 50);
				printf("CoordinateX[%d] = %d\n", i, width[i].first);
				if (CoordinateX.size() == 4)
					break;
			}
		}
		if (CoordinateX.size() != 4)
		{
			printf("partRowImagesNum[%d]  CoordinateX.size() = %d. it's wrong \n", partRowImagesNum, CoordinateX.size());
			std::abort();
		}
		std::sort(CoordinateX.begin(), CoordinateX.end(), std::less<int>());//降序,使零件坐标从左到右排列

		cv::transpose(partRowImage, partRowImage);//转置回来
		kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 35));
		dilate(partRowImage, partRowImage, kernel);
		//cv::namedWindow("partRowImage", 0);
		//cv::imshow("partRowImage", partRowImage);
		//cv::waitKey(0);
		std::vector<cv::Mat> partTempImagesSrc;
		//对列求和
		cv::Mat multiResult = getSumOfEachRowOrColumnForBinaryImage(partRowImage, false);
		double* ptrMultiResult = multiResult.ptr<double>(0);			// 行图像的指针
		//从零件中心向内侧（大椭圆孔中心）寻找黑色边界
		//因为拍摄角度问题，零件中心向相机中心方向的连接是比较紧密的，出现黑色断带的可能性较低
		int cutCols = 0;//用于确定两个零件中间的分界线，初始为0，从外侧向内确定
		for (int i = 0; i < 2; i++)//前两个零件
		{
			for (int j = CoordinateX[i]; j < CoordinateX[i + 1]; j++)
			{
				bool isBlack = true;
				for (int k = j; k < j+20; k++)
				{
					if (ptrMultiResult[k] != 0)//如果这里出现越界的话，应该是CoordinateX[i + 1]的坐标确定有问题，暂时想不好，等遇到再看
					{
						isBlack = false;
						break;
					}
				}
				if(isBlack)
				{
					partTempImagesSrc.push_back(partRowImageSrc(cv::Range(0, partRowImage.rows), cv::Range(cutCols, j + 3)));
					cutCols = j + 3;//j+3留出3列黑色，因为最开始只连续判断5列，取了中间数，改为20后可以再试着修改
					//cv::namedWindow("partImage", 0);
					//cv::imshow("partImage", partImage[partImage.size()-1]);
					//cv::waitKey(0);
					break;
				}
			}
		}

		cutCols = multiResult.cols;//右边两个零件一样从外侧向内
		for (int i = 3; i > 1; i--)//后两个零件
		{
			for (int j = CoordinateX[i]; j > CoordinateX[i - 1]; j--)
			{
				bool isBlack = true;
				for (int k = j; k > j-20; k--)//判定连续若干列为黑
				{
					if (ptrMultiResult[k] != 0)//如果这里出现越界的话，应该是CoordinateX[i - 1]的坐标确定有问题，暂时想不好，等遇到再看
					{
						isBlack = false;
						break;
					}
				}
				if (isBlack)
				{
					partTempImagesSrc.push_back(partRowImageSrc(cv::Range(0, partRowImage.rows), cv::Range(j - 3, cutCols)));
					cutCols = j - 3;//j-3留出3列黑色，因为最开始只连续判断5列，取了中间数，改为20后可以再试着修改
					//cv::namedWindow("partImage", 0);
					//cv::imshow("partImage", partImage[partImage.size() - 1]);
					//cv::waitKey(0);
					break;
				}
			}
		}

		if (partTempImagesSrc.size() !=  4)
		{
			printf("the number of partTempImagesSrc not enough. partTempImagesSrc.size() = %d \n", partTempImagesSrc.size());
			std::abort();
		}
		partImagesSrc.push_back(partTempImagesSrc[0]);
		partImagesSrc.push_back(partTempImagesSrc[1]);	
		partImagesSrc.push_back(partTempImagesSrc[3]);
		partImagesSrc.push_back(partTempImagesSrc[2]); //交换第三第四，使零件按顺序排列
	}
}

cv::Mat getSumOfEachRowOrColumnForBinaryImage(cv::Mat src, bool forRows)
{
	//使用矩阵乘法统计白色点个数，
	//返回为1*src.rows或1*src.cols的图像,
	//返回Mat 为CV_64F型，指针使用float
	cv::Mat srcFloat = src.clone();
	srcFloat.convertTo(srcFloat, CV_64F, 1.0 / 255);//黑为0，白为1
	cv::Mat multiResult;
	if (forRows)
	{
		cv::Mat multiOnes = cv::Mat::ones(srcFloat.cols, 1, CV_64F);
		multiResult = 1.0 * srcFloat * multiOnes;
		cv::transpose(multiResult, multiResult);

	}
	else
	{
		//对列求和
		cv::Mat multiOnes = cv::Mat::ones(1, srcFloat.rows, CV_64F);
		multiResult = 1.0 * multiOnes * srcFloat;
	}
	return multiResult;
}

double getStdevOfVector(std::vector<int> &resultSet)
{
	double sum = std::accumulate(std::begin(resultSet), std::end(resultSet), 0.0);
	double mean = sum / resultSet.size(); //均值

	double accum = 0.0;
	std::for_each(std::begin(resultSet), std::end(resultSet), [&](const double d) {
		accum += (d - mean) * (d - mean);
		});

	double stdev = sqrt(accum / (resultSet.size() - 1)); //方差
	return stdev;
}

void getContactImage(std::vector<cv::Mat> &partImagesSrc, std::vector<std::vector<cv::Mat>> &contactPairImages)
{
	//对每个零件取出两个触头 一个触头像素大致在90*90
	for (auto partImageSrc : partImagesSrc)
	{
		std::vector<cv::Mat> contactImages;
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(32, 32));
		cv::Mat partImageTophat;
		cv::morphologyEx(partImageSrc, partImageTophat, cv::MORPH_TOPHAT, kernel);
		cv::Mat partImageBinary;
		cv::threshold(partImageTophat, partImageBinary, 0, 255, CV_THRESH_OTSU);
		kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
		cv::morphologyEx(partImageBinary, partImageBinary, cv::MORPH_OPEN, kernel);
		kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9));
		cv::morphologyEx(partImageBinary, partImageBinary, cv::MORPH_CLOSE, kernel);
		//cv::namedWindow("partImageBinary", 0);
		//cv::imshow("partImageBinary", partImageBinary);
		//cv::imshow("partImageBinary.bmp", partImageBinary);
		//cv::waitKey(0);


		//使用加权和求中心
		cv::Mat multiResult = getSumOfEachRowOrColumnForBinaryImage(partImageBinary, true);
		double* ptrMultiResult = multiResult.ptr<double>(0);
		double sumOfWhitePoints = 0, productSumOfWhitePointAndCoordinate = 0;
		for (int i = 0; i < multiResult.cols; i++)
		{
			sumOfWhitePoints = sumOfWhitePoints + ptrMultiResult[i];
			productSumOfWhitePointAndCoordinate = productSumOfWhitePointAndCoordinate + ptrMultiResult[i] * i;
		}
		printf("sumOfWhitePoints = %f , productSumOfWhitePointAndCoordinate = %f\n", sumOfWhitePoints, productSumOfWhitePointAndCoordinate);
		int centerOfCircleCoordinateY = static_cast<int>(productSumOfWhitePointAndCoordinate / sumOfWhitePoints);
		printf("coordinateY = %d \n", centerOfCircleCoordinateY);


		partImageBinary.convertTo(partImageBinary, CV_8U, 1);
		uchar* ptrPartImageBinary = partImageBinary.ptr<uchar>(centerOfCircleCoordinateY);
		//从左向里找到白色，为触头
		for (int i = 0; i < partImageBinary.cols - 1; i++)
		{
			if (ptrPartImageBinary[i] == 255 && ptrPartImageBinary[i + 1] == 255)
			{
				//左侧触头 
				int startRow = centerOfCircleCoordinateY - 60, endRow = centerOfCircleCoordinateY + 60;
				int startCol = i - 10, endCol = i + 90;
				//边界检查
				startRow = startRow > 0 ? startRow : 0;
				endRow = endRow < partImageBinary.rows ? endRow : partImageBinary.rows;
				startCol = startCol > 0 ? startCol : 0;
				endCol = endCol < partImageBinary.cols ? endCol : partImageBinary.cols;
				//cv::Mat leftContactImage = partImageBinary(cv::Range(startRow, endRow), cv::Range(startCol, endCol));
				contactImages.emplace_back(partImageSrc(cv::Range(startRow, endRow), cv::Range(startCol, endCol)));
				//cv::namedWindow("leftContactImage", 0);
				//cv::imshow("leftContactImage", leftContactImage);
				//cv::waitKey(0);
				break;
			}
		}
		//从右向里找到白色，为触头
		for (int i = partImageBinary.cols; i > 1; i--)
		{
			if (ptrPartImageBinary[i] == 255 && ptrPartImageBinary[i - 1] == 255)
			{
				//右侧触头
				int startRow = centerOfCircleCoordinateY - 60, endRow = centerOfCircleCoordinateY + 60;
				int startCol = i - 90, endCol = i + 10;
				startRow = startRow > 0 ? startRow : 0;
				endRow = endRow < partImageBinary.rows ? endRow : partImageBinary.rows;
				startCol = startCol > 0 ? startCol : 0;
				endCol = endCol < partImageBinary.cols ? endCol : partImageBinary.cols;
				//cv::Mat rightContactImage = partImageBinary(cv::Range(startRow, endRow), cv::Range(startCol, endCol));
				contactImages.emplace_back(partImageSrc(cv::Range(startRow, endRow), cv::Range(startCol, endCol)));
				//cv::namedWindow("rightContactImage", 0);
				//cv::imshow("rightContactImage", rightContactImage);
				//cv::waitKey(0);
				break;
			}
		}
		if (contactImages.size() != 2)
		{
			printf("the number of contactImages not enough. contactImages.size() = %d \n", contactImages.size());
			std::abort();
		}
		contactPairImages.emplace_back(contactImages);
	}
}

void getDirectionOfOpening(cv::Mat contactImage, double& diff, int& direction)
{
	cv::Mat leftContactImage;//命名left前缀的原因是一开始是检测左侧触头的，懒得改了
	cv::threshold(contactImage, leftContactImage, 0, 255, CV_THRESH_OTSU);


	leftContactImage.convertTo(leftContactImage, CV_8U, 1);
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::morphologyEx(leftContactImage, leftContactImage, cv::MORPH_CLOSE, kernel);
	cv::namedWindow("ContactImage", 0);
	cv::imshow("ContactImage", leftContactImage);
	cv::waitKey(1);

	cv::Mat leftContactImageForDoordinateOfDHole = leftContactImage.clone();
	cv::floodFill(leftContactImageForDoordinateOfDHole, cv::Point(0, 0), 255);
	cv::floodFill(leftContactImageForDoordinateOfDHole, cv::Point(0, leftContactImage.rows - 1), 255);
	cv::floodFill(leftContactImageForDoordinateOfDHole, cv::Point(leftContactImage.cols - 1, 0), 255);
	cv::floodFill(leftContactImageForDoordinateOfDHole, cv::Point(leftContactImage.cols - 1, leftContactImage.rows - 1), 255);

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::bitwise_not(leftContactImageForDoordinateOfDHole, leftContactImageForDoordinateOfDHole);
	findContours(leftContactImageForDoordinateOfDHole, contours, hierarchy, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE, cv::Point());
	// 寻找最大连通域
	double max = 0;
	int maxAreaIdx = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		if (area > max)
		{
			max = area;
			maxAreaIdx = i;
		}
	}
	//计算矩来获取D形状孔中心坐标 x为列坐标，y为行坐标
	cv::Moments momentOfD = cv::moments(contours[maxAreaIdx]);
	int xOfD = static_cast<int>(1.0 * momentOfD.m10 / momentOfD.m00);
	int yOfD = static_cast<int>(1.0 * momentOfD.m01 / momentOfD.m00);
	printf("coordinate of D: x = %d  y = %d \n", xOfD, yOfD);
	//cv::Mat leftContactImageContour = cv::Mat::zeros(leftContactImage.size(), CV_8UC1);
	//drawContours(leftContactImageContour, contours, maxAreaIdx, cv::Scalar(255), -1, 8, hierarchy);

	//寻找初始点
	std::vector<int> leftLine;
	std::vector<int> rightLine;

	uchar* ptrLeftContactImage = leftContactImage.ptr<uchar>(yOfD);
	int stateNum = 0;
	int leftStart, rightStart;
	for (int col = xOfD; col > 1; col--)//寻找左侧初始点
	{
		if (stateNum == 1)
		{
			if (ptrLeftContactImage[col] == 0 && ptrLeftContactImage[col - 1] == 0)
			{
				leftLine.emplace_back(col);
				leftStart = col;
				break;
			}
		}
		else
		{
			if (ptrLeftContactImage[col] == 255 && ptrLeftContactImage[col - 1] == 255)
			{
				stateNum = 1;
			}
		}
	}
	stateNum = 0;
	for (int col = xOfD; col < leftContactImage.cols - 1; col++)//寻找左侧初始点
	{
		if (stateNum == 1)
		{
			if (ptrLeftContactImage[col] == 0 && ptrLeftContactImage[col + 1] == 0)
			{
				rightLine.emplace_back(col);
				rightStart = col;
				break;
			}
		}
		else
		{
			if (ptrLeftContactImage[col] == 255 && ptrLeftContactImage[col + 1] == 255)
			{
				stateNum = 1;
			}
		}
	}

	bool isContinue = true;
	for (int i = yOfD - 1; i > 0; i--)
	{
		ptrLeftContactImage = leftContactImage.ptr<uchar>(i);
		//根生法寻找左边界
		int workCol = leftLine[leftLine.size() - 1];
		if (isContinue && ptrLeftContactImage[workCol] == 0)//黑色向内找
		{
			while (workCol < leftContactImage.cols)
			{
				if (ptrLeftContactImage[workCol] == 255 && ptrLeftContactImage[workCol - 1] == 255)
				{
					leftLine.emplace_back(workCol - 2);//边界保持在黑色像素点上
					break;
				}
				workCol++;
			}
			if (workCol == leftContactImage.cols)
			{
				isContinue = false;
			}
		}
		else if (isContinue)//白色向外找
		{
			while (workCol > 0)
			{
				if (ptrLeftContactImage[workCol] == 0 && ptrLeftContactImage[workCol + 1] == 0)
				{
					leftLine.emplace_back(workCol + 2);//边界保持在黑色像素点上
					break;
				}
				workCol--;
			}
			if (workCol == 0)
			{
				isContinue = false;
			}
		}

		//根生法寻找右边界
		workCol = rightLine[rightLine.size() - 1];
		if (isContinue && ptrLeftContactImage[workCol] == 0)//黑色向内找
		{
			while (workCol > 0)
			{
				if (ptrLeftContactImage[workCol] == 255 && ptrLeftContactImage[workCol + 1] == 255)
				{
					rightLine.emplace_back(workCol + 2);//边界保持在黑色像素点上
					break;
				}
				workCol--;
			}
			if (workCol == 0)
			{
				isContinue = false;
			}
		}
		else if (isContinue)//白色向外找
		{
			while (workCol < leftContactImage.cols)
			{
				if (ptrLeftContactImage[workCol] == 0 && ptrLeftContactImage[workCol - 1] == 0)
				{
					rightLine.emplace_back(workCol - 2);//边界保持在黑色像素点上
					break;
				}
				workCol++;
			}
			if (workCol == leftContactImage.cols)
			{
				isContinue = false;
			}
		}
		if (!isContinue)
		{
			break;
		}
	}

	if (leftLine.size() != rightLine.size())
	{
		printf("error: leftLine.size() != rightLine.size() \n");
		exit(0);
	}
	printf("leftLine.size() = %d , rightLine.size() = %d \n", leftLine.size(), rightLine.size());
	for (int i = 0; i < 10; i++)//去除最后十行边界，因为结束时，存在一侧线快速移动到另一侧的情况
	{
		leftLine.pop_back();
		rightLine.pop_back();
	}
	for (int i = 0; i < leftLine.size(); i++)
	{
		//printf("leftLine[%d] = %d\n", i, leftLine[i]);
		leftLine[i] = xOfD - leftLine[i];
		rightLine[i] = rightLine[i] - xOfD;
	}

	double leftStdev = getStdevOfVector(leftLine);
	double rightStdev = getStdevOfVector(rightLine);

	printf("leftStdev = %f , rightStdev = %f \n", leftStdev, rightStdev);
	diff = leftStdev - rightStdev;
	direction = (diff > 0) ? 0 : 1;//0表示朝左，1表示朝右
}