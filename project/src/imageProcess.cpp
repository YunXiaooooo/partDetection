#include "imageProcess.h"
#include "DL_interface.h"

#include <windows.h>

#undef max
#undef min

//��ΪDL_interface.h����imageProcess.h�ᵼ�³�ͻ�޷����룬����DL_module������imageProcess��������
//ʹ��ȫ�ֱ�����Ϊ���
static DL_module dl_model("part_cut_gray_resnet.pt", false);

void imageProcess::realImageProcess(cv::Mat& src, const int grabNum, std::vector<int>& result)//����grabNum������Բ鿴�ĸ�ͼƬ����
{
	/*ͼ����
		1/12����һ��4����������±��
		3   2
		1   0
		vector<int> result��ģΪ4����ÿ��λ�õ����״̬����:
			1��ʾ������2��ʾ�����෴��3��ʾ�գ�û�����; 4��ʾ����쳣����ͷ��಻�Ի��������⣩
	*/
	clock_t start = clock();
	result.clear();
	result.resize(4, 4); // Ĭ���쳣�������⵽�˲Ÿ�����״̬
	imagePerspective(src, grabNum);//����
	//saveImage("src" + std::to_string(grabNum) + ".bmp", src);//����ԭͼ����debug
	//showImage("src", src, 0, true);

	cv::Mat grayRoiAboutFourParts, binaryRoiAboutFourParts;
	double OTSU_threadNum = getRoiAboutFourParts(src, grayRoiAboutFourParts, binaryRoiAboutFourParts);

	saveImage("grayRoiAboutFourParts" + std::to_string(grabNum) + ".bmp", grayRoiAboutFourParts);//����ԭͼ����debug
	printf("OTSU_threadValue: %f \n", OTSU_threadNum);
	//if (OTSU_threadNum < OTSU_threadNumMin)//��ֵ��С��12��λȫ��û�����
	//{
	//	result.clear();
	//	result.resize(4, 3);
	//	return;
	//}

	std::vector<struct boxCoordinates> doubleHoleImageCoordinates(2);
	doubleHoleImageCoordinates[1].startRows = 0;
	doubleHoleImageCoordinates[1].endRows = grayRoiAboutFourParts.rows;
	doubleHoleImageCoordinates[1].startCols = 0;
	doubleHoleImageCoordinates[1].endCols = grayRoiAboutFourParts.cols / 2;

	doubleHoleImageCoordinates[0].startRows = 0;
	doubleHoleImageCoordinates[0].endRows = grayRoiAboutFourParts.rows;
	doubleHoleImageCoordinates[0].startCols = grayRoiAboutFourParts.cols / 2;
	doubleHoleImageCoordinates[0].endCols = grayRoiAboutFourParts.cols;


	std::vector<struct boxCoordinates> holeImageCoordinates(4);
	getHoleImageCoordinates(holeImageCoordinates, doubleHoleImageCoordinates, OTSU_threadNum, grayRoiAboutFourParts, binaryRoiAboutFourParts);
	//��ʼ�Ե���������м��
	DL_module m;
	dl_model.clone(m);
	for (int holeNum = 0; holeNum < 4; ++holeNum)
	{
		try
		{
			printf("\n*************************************************\n");
			cv::Mat holeImage = binaryRoiAboutFourParts(cv::Range(holeImageCoordinates[holeNum].startRows, holeImageCoordinates[holeNum].endRows),
				cv::Range(holeImageCoordinates[holeNum].startCols, holeImageCoordinates[holeNum].endCols));
			cv::Mat grayPart = grayRoiAboutFourParts(cv::Range(holeImageCoordinates[holeNum].startRows, holeImageCoordinates[holeNum].endRows),
				cv::Range(holeImageCoordinates[holeNum].startCols, holeImageCoordinates[holeNum].endCols)).clone();

			double th = cv::threshold(grayPart, holeImage, 0, 255, CV_THRESH_OTSU);
			th = cv::threshold(grayPart, holeImage, th * 0.9, 255, CV_THRESH_BINARY);

			cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
			morphologyEx(holeImage, holeImage, cv::MORPH_CLOSE, kernel);
			kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
			morphologyEx(holeImage, holeImage, cv::MORPH_OPEN, kernel);//������ȥ�����������С�׵�
			//showImage("holeImage", holeImage, 0, false);

			std::vector<cv::Point2i> DCoordinates(2);
			getTwoContactDHoleCoordinates(holeImage, DCoordinates);

			//�ϲ�ȱ��
			struct boxCoordinates upContactCoordiate;
			upContactCoordiate.startRows = std::max(static_cast<int>(DCoordinates[0].y - 89), 0);
			upContactCoordiate.endRows = std::min(static_cast<int>(DCoordinates[0].y + 39), holeImage.rows);
			upContactCoordiate.startCols = std::max(static_cast<int>(DCoordinates[0].x - 64), 0);
			upContactCoordiate.endCols = std::min(static_cast<int>(DCoordinates[0].x + 64), holeImage.cols);

			cv::Point2i upCenter(DCoordinates[0].x - upContactCoordiate.startCols, DCoordinates[0].y - upContactCoordiate.startRows);

			cv::Mat upContactImage = grayPart(cv::Range(upContactCoordiate.startRows, upContactCoordiate.endRows),
				cv::Range(upContactCoordiate.startCols, upContactCoordiate.endCols)).clone();
			//�²�ȱ��
			struct boxCoordinates downContactCoordiate;
			downContactCoordiate.startRows = std::max(static_cast<int>(DCoordinates[1].y - 39), 0);
			downContactCoordiate.endRows = std::min(static_cast<int>(DCoordinates[1].y + 89), holeImage.rows);
			downContactCoordiate.startCols = std::max(static_cast<int>(DCoordinates[1].x - 64), 0);
			downContactCoordiate.endCols = std::min(static_cast<int>(DCoordinates[1].x + 64), holeImage.cols);

			cv::Point2i downCenter(DCoordinates[1].x - downContactCoordiate.startCols, DCoordinates[1].y - downContactCoordiate.startRows);

			cv::Mat downContactImage = grayPart(cv::Range(downContactCoordiate.startRows, downContactCoordiate.endRows),
				cv::Range(downContactCoordiate.startCols, downContactCoordiate.endCols)).clone();
			//showImage("upContactImage", upContactImage, 0, false);
			//showImage("downContactImage", downContactImage, 0, true);
			//saveImage("upContactImage" + std::to_string(grabNum)+"-"+std::to_string(holeNum) + ".bmp", upContactImage);//����ԭͼ����debug
			//saveImage("downContactImage" + std::to_string(grabNum) + "-" + std::to_string(holeNum) + ".bmp", downContactImage);
			cv::flip(downContactImage, downContactImage, 0);//���·�ת
			cv::resize(upContactImage, upContactImage, cv::Size(128, 128));
			cv::resize(downContactImage, downContactImage, cv::Size(128, 128));
			std::pair<int, float> UpClassAndProbability(-1, -1), DownClassAndProbability(-1,-1);
			std::pair<int, float> ClassAndProbability(-1, -1);
			//����
			m.single_interface(upContactImage, UpClassAndProbability);
			m.single_interface(downContactImage, DownClassAndProbability);
			//��������о�
			printf("UpClassAndProbability:%d, %f \n", UpClassAndProbability.first, UpClassAndProbability.second);
			printf("DownClassAndProbability:%d, %f \n", DownClassAndProbability.first, DownClassAndProbability.second);
			if (UpClassAndProbability.first == DownClassAndProbability.first)
			{
				ClassAndProbability.first = UpClassAndProbability.first;
				ClassAndProbability.second = (UpClassAndProbability.second + DownClassAndProbability.second) / 2;
			}
			else
			{
				result[holeNum] = 4;
			}
			if (ClassAndProbability.first >= 0 && ClassAndProbability.second > 0.6)
			{
				result[holeNum] = ClassAndProbability.first + 1;
			}
			else
			{
				result[holeNum] = 4;
			}
		}
		catch (std::exception& e)
		{
			printf("ͼƬ��%d��Ѩλ��%d���쳣: %s\n", grabNum, holeNum, e.what());
			result[holeNum] = 4;
		}
	}
	//Sleep(2000);//ms
	clock_t ends = clock();
	printf("imageProcess Running Time = %f \n", ((double)(ends)-start) / CLOCKS_PER_SEC);

}
std::vector<int> imageProcess::imageProcessTask(cv::Mat& src, const int grabNum)
{
	cv::Mat srcClone;
	if (src.channels() != 1)
	{
		cv::cvtColor(src, srcClone, CV_BGR2GRAY);
	}
	else
	{
		srcClone = src.clone(); //�����޸�ԭͼ
	}

	std::vector<int> DirectionRecognitionResult(4);
	try
	{
		realImageProcess(srcClone, grabNum, DirectionRecognitionResult);
	}
	catch (std::exception& e)
	{
		printf("ͼƬ��%d�������쳣:%s \n", grabNum, e.what());
		DirectionRecognitionResult.clear();
		DirectionRecognitionResult.resize(4, 4);
	}


	for (size_t i = 0, len = DirectionRecognitionResult.size(); i < len; i++)
	{
		printf("image num = %d, location = %d , state = %d\n", grabNum, static_cast<int>(i + static_cast<int64>(grabNum) * 12), DirectionRecognitionResult[i]);
	}
	return DirectionRecognitionResult;
}

void imageProcess::imagePerspective(cv::Mat& src, const int grabNum)
{
	src = RotateImage(src, cammeraAngle[grabNum % 3], 0, cv::Point2i(src.cols / 2, src.rows / 2));
}

cv::Mat imageProcess::getSumOfEachRowOrColumnForBinaryImage(const cv::Mat& src, bool forRows)
{
	//ʹ�þ���˷�ͳ�ư�ɫ�������
	//����Ϊ1*src.rows��1*src.cols��ͼ��,
	//����Mat ΪCV_64F�ͣ�ָ��ʹ��double
	cv::Mat srcFloat = src.clone();
	srcFloat.convertTo(srcFloat, CV_64F, 1.0 / 255);//��Ϊ0����Ϊ1
	cv::Mat multiResult;
	if (forRows)
	{
		cv::Mat multiOnes = cv::Mat::ones(srcFloat.cols, 1, CV_64F);
		multiResult = 1.0 * srcFloat * multiOnes;
		cv::transpose(multiResult, multiResult);

	}
	else
	{
		//�������
		cv::Mat multiOnes = cv::Mat::ones(1, srcFloat.rows, CV_64F);
		multiResult = 1.0 * multiOnes * srcFloat;
	}
	return multiResult;
}

unsigned int imageProcess::getSumOfBinaryImage(const cv::Mat& src)
{
	//ͳ�ư�ɫ�������
	//��������
	cv::Mat temp = getSumOfEachRowOrColumnForBinaryImage(src, true);
	double* ptr = temp.ptr<double>(0);
	unsigned int sum = 0;
	for (int i = 0, length = temp.cols; i < length; ++i)
	{
		sum = sum + static_cast<unsigned int>(ptr[i]);
	}

	return sum;
}

void imageProcess::getTwoContactDHoleCoordinates(const cv::Mat& image, std::vector<cv::Point2i>& DCoordinates)
{
	DCoordinates.resize(2);
	cv::Mat DHoleImage = image.clone();
	DHoleImage.convertTo(DHoleImage, CV_8U, 255);
	if (DHoleImage.at<unsigned char>(0, 0) == 0)
	{
		cv::floodFill(DHoleImage, cv::Point(0, 0), 255);
	}
	if (DHoleImage.at<unsigned char>(DHoleImage.rows - 1, 0) == 0)//���к��м���y��x
	{
		cv::floodFill(DHoleImage, cv::Point(0, DHoleImage.rows - 1), 255);
	}
	if (DHoleImage.at<unsigned char>(0, DHoleImage.cols - 1) == 0)
	{
		cv::floodFill(DHoleImage, cv::Point(DHoleImage.cols - 1, 0), 255);
	}
	if (DHoleImage.at<unsigned char>(DHoleImage.rows - 1, DHoleImage.cols - 1) == 0)
	{
		cv::floodFill(DHoleImage, cv::Point(DHoleImage.cols - 1, DHoleImage.rows - 1), 255);
	}

	//�������
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::bitwise_not(DHoleImage, DHoleImage);
	//saveImage("DHoleImage.bmp", DHoleImage);
	//showImage("DHoleImage", DHoleImage, 1, true);

	findContours(DHoleImage, contours, hierarchy, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE, cv::Point());
	//�������ɸѡ����
	std::vector<std::pair<double, int>> contoursArea;
	for (size_t i = 0, contoursSize = contours.size(); i < contoursSize; i++)
	{
		double temp = contourArea(contours[i]);
		printf("contourArea(contours[i]) =  %f \n", temp);
		cv::RotatedRect box;
		box = cv::minAreaRect(cv::Mat(contours[i]));
		if (getLengthCompareWidth(box) > lengthCompareWidth)
		{
			printf("LengthCompareWidth is too large:%f", getLengthCompareWidth(box));
			continue;
		}
		if (minDHoleArea < temp && temp < maxDHoleArea)
		{
			contoursArea.push_back(std::make_pair(temp, i));
		}
	}
	std::sort(contoursArea.begin(), contoursArea.end(), [&](auto x, auto y) {return x.first > y.first; });//����,������Ӵ�С��


	if (contoursArea.size() < 2)
	{
		printf("getTwoContactDHoleCoordinates error: the image is abnormal! \n");
		DCoordinates.clear();
	}
	else
	{
		std::array<std::vector<std::pair<double, cv::Point2i>>, 2>DScoreAndPoint;
		auto itor = contoursArea.begin();
		while (itor != contoursArea.end())
		{
			printf("area: %f \n", itor->first);
			if (itor->first < maxDHoleArea && itor->first > minDHoleArea)
			{
				cv::Moments momentOfD = cv::moments(contours[itor->second]);
				cv::Point2i temp{ static_cast<int>(1.0 * momentOfD.m10 / momentOfD.m00) ,static_cast<int>(1.0 * momentOfD.m01 / momentOfD.m00) };
				double minDistance = 0;
				int Dindx = nearestD(temp, minDistance, D_hole_XY_in_single_part_image);
				DScoreAndPoint[Dindx].emplace_back(minDistance / itor->first, temp);//�����ɾ����������õ���ԽСԽ�ã������ǰ����������С������Ϊ0
			}
			itor++;
		}
		for (size_t i = 0; i < 2; ++i)
		{
			if (DScoreAndPoint[i].size() == 0)
			{
				DCoordinates[i] = D_hole_XY_in_single_part_image.read(i);
				continue;
			}
			std::sort(DScoreAndPoint[i].begin(), DScoreAndPoint[i].end(), [&](auto d1, auto d2) {return d1.first < d2.first; });//����
			DCoordinates[i] = DScoreAndPoint[i][0].second;
			//�������»�׼����
			D_hole_XY_in_single_part_image.update(i, DCoordinates[i]);
		}
	}
}

double imageProcess::getDistance(const cv::Point2f& p1, const cv::Point2f& p2)
{
	double res = sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
	return res;
}


void imageProcess::rotateContact(cv::Mat& contactImage, const cv::Point2i& center)
{
	cv::Mat DHoleImage = contactImage.clone();
	DHoleImage.convertTo(DHoleImage, CV_8U, 255);
	cv::Mat edges;
	Canny(DHoleImage, edges, 100, 200, 3, false);
	struct boxCoordinates rectBox;
	rectBox.startRows = std::max(0, center.y - 30);
	rectBox.endRows = std::min(contactImage.rows, center.y + 30);
	rectBox.startCols = 0;
	rectBox.endCols = contactImage.cols;
	cv::Mat rectROI = edges(cv::Range(rectBox.startRows, rectBox.endRows), cv::Range(rectBox.startCols, rectBox.endCols));
	//showImage("Canny edge detection", edges, 1, false);
	//showImage("ROI", rectROI, 1, false);


	int maxAngle = 0;
	double maxValue = 0;
	for (int angle = -10; angle < 10; ++angle)
	{
		cv::Mat tempEdges = RotateImage(rectROI, angle, 0, center);
		double value = getVarianceOfWhitePointDistributionForColumnProjection(tempEdges);
		if (value > maxValue)
		{
			maxAngle = angle;
			maxValue = value;
		}
	}
	printf("contact roate angle = %d \n", maxAngle);
	contactImage = RotateImage(contactImage, maxAngle, 0, center);

}

cv::Mat imageProcess::RotateImage(const cv::Mat& src, const double angle, const int color, const cv::Point2i& center)
{
	cv::Mat dst;

	//float scale = 200.0/ src.rows;//��������    
	//cv::resize(src, src, cv::Size(), scale, scale, cv::INTER_LINEAR);    	    	
	//���ͼ��ĳߴ���ԭͼһ��    
	cv::Size dst_sz(src.cols, src.rows);

	//ָ����ת����      
	//cv::Point2f center(static_cast<float>(src.cols / 2.), static_cast<float>(src.rows / 2.));

	//��ȡ��ת����2x3����      
	cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, 1.0);
	//����ѡ�񱳾��߽���ɫ   
	if (color == 0)//��ɫ���
	{
		cv::Scalar borderColor = cv::Scalar(0, 0, 0);
		cv::warpAffine(src, dst, rot_mat, src.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, borderColor);
	}
	else if (color == 255)//��ɫ���
	{
		cv::Scalar borderColor = cv::Scalar(255, 255, 255);
		cv::warpAffine(src, dst, rot_mat, src.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, borderColor);
	}
	else
	{
		//���Ʊ�Ե���
		cv::warpAffine(src, dst, rot_mat, dst_sz, cv::INTER_LINEAR, cv::BORDER_REPLICATE);
	}
	return dst;
}

double imageProcess::getVarianceOfWhitePointDistributionForColumnProjection(const cv::Mat& image)
{
	cv::Mat temp = getSumOfEachRowOrColumnForBinaryImage(image, false);
	double* ptr = temp.ptr<double>(0);
	double var = 0;
	double meanValue = getSumOfBinaryImage(image) / temp.cols;
	for (int i = 0; i < temp.cols; ++i)
	{
		var = var + (meanValue - ptr[i]) * (meanValue - ptr[i]);
	}
	return var;
}


cv::Point2i imageProcess::getHoleCoordinate(const cv::Mat& image)
{
	cv::Mat DHoleImage = image.clone();
	DHoleImage.convertTo(DHoleImage, CV_8U, 255);
	if (DHoleImage.at<unsigned char>(0, 0) == 0)
	{
		cv::floodFill(DHoleImage, cv::Point(0, 0), 255);
	}
	if (DHoleImage.at<unsigned char>(DHoleImage.rows - 1, 0) == 0)
	{
		cv::floodFill(DHoleImage, cv::Point(0, DHoleImage.rows - 1), 255);
	}
	if (DHoleImage.at<unsigned char>(0, DHoleImage.cols - 1) == 0)
	{
		cv::floodFill(DHoleImage, cv::Point(DHoleImage.cols - 1, 0), 255);
	}
	if (DHoleImage.at<unsigned char>(DHoleImage.rows - 1, DHoleImage.cols - 1) == 0)
	{
		cv::floodFill(DHoleImage, cv::Point(DHoleImage.cols - 1, DHoleImage.rows - 1), 255);
	}

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::bitwise_not(DHoleImage, DHoleImage);
	findContours(DHoleImage, contours, hierarchy, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE, cv::Point());
	if (contours.size() == 0)
	{
		printf("�������һ��D�ο׶�λʧ�� \n");
		cv::Point2i D(-1, -1);
		return D;
	}
	// Ѱ�������ͨ��
	double max = 0;
	int maxAreaIdx = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);
		cv::RotatedRect box;
		box = cv::minAreaRect(cv::Mat(contours[i]));

		if (area > max && getLengthCompareWidth(box) < 3)
		{
			max = area;
			maxAreaIdx = i;
		}
	}
	//���������ȡD��״���������� xΪ�����꣬yΪ������
	cv::Moments momentOfD = cv::moments(contours[maxAreaIdx]);
	cv::Point2i D;
	D.x = static_cast<int>(1.0 * momentOfD.m10 / momentOfD.m00);
	D.y = static_cast<int>(1.0 * momentOfD.m01 / momentOfD.m00);
	printf("coordinate of D: x = %d  y = %d \n", D.x, D.y);
	return D;
}

std::pair<int, int> imageProcess::getLeftAndRightHalfValue(const cv::Mat& image, const bool isUp)
{
	cv::Mat ContactImage = image.clone();

	if (!isUp)//�²ഥͷ���·�ת���������Ҳ���
	{
		cv::flip(ContactImage, ContactImage, 0);
	}

	cv::Mat subImage = ContactImage(cv::Range(ContactImage.rows - 2, ContactImage.rows), cv::Range(0, ContactImage.cols));
	subImage.setTo(0);
	//showImage("ContactImage", ContactImage,1,true);

	cv::Point2i D = getHoleCoordinate(ContactImage);
	ContactImage.convertTo(ContactImage, CV_8U, 255);
	if (D.x == -1 || D.y == -1 ||
		ContactImage.cols <= 10 || 10 > D.x || D.x >= ContactImage.cols - 10 ||
		ContactImage.rows <= 10 || 10 > D.y || D.y >= ContactImage.rows - 5)
	{
		printf("�����쳣! \n");
		return std::make_pair(0, 0);
	}
	//ʹ��D�����Ϸ���ɫ�������ж�
	int halfImageStartRows = 0;
	for (int i = 1; i < D.y; ++i)
	{
		if (ContactImage.at<unsigned char>(i, D.x) == 255 && ContactImage.at<unsigned char>(i - 1, D.x) == 0)
		{
			halfImageStartRows = i + 10;
			halfImageStartRows = std::min(halfImageStartRows, D.y);
			break;
		}
	}
	int halfImageEndRows = 0;
	for (int i = D.y; i > halfImageStartRows; --i)
	{
		if (ContactImage.at<unsigned char>(i, D.x) == 255 && ContactImage.at<unsigned char>(i + 1, D.x) == 0)
		{
			halfImageEndRows = i - 1;
			halfImageEndRows = std::max(halfImageStartRows, halfImageEndRows);
			break;
		}
	}
	int halfImageStartCols = 0;
	for (int i = D.x; i > 0; --i)
	{
		if (ContactImage.at<unsigned char>(D.y, i) == 255 && ContactImage.at<unsigned char>(D.y, i + 1) == 0)
		{
			halfImageStartCols = i - 1;
			halfImageStartCols = std::min(halfImageStartCols, D.y);
			break;
		}
	}
	int halfImageEndCols = 0;
	for (int i = D.x; i < ContactImage.cols; ++i)
	{
		if (ContactImage.at<unsigned char>(D.y, i) == 255 && ContactImage.at<unsigned char>(D.y, i - 1) == 0)
		{
			halfImageEndCols = i - 1;
			halfImageEndCols = std::max(halfImageStartCols, halfImageEndCols);
			break;
		}
	}
	cv::Mat left, right;
	try
	{
		left = ContactImage(cv::Range(halfImageStartRows, halfImageEndRows), cv::Range(halfImageStartCols, D.x));
		right = ContactImage(cv::Range(halfImageStartRows, halfImageEndRows), cv::Range(D.x, halfImageEndCols));

	}
	catch (std::exception& e)
	{
		printf("�����쳣�����ҷָ�ʧ�ܣ�[%s]", e.what());
		return std::make_pair(0, 0);
	}
	//showImage("left", left, 1, false);
	//showImage("right", right, 1, false);

	//ʹ�������ж�
	//int halfImageStartRows = 0;
	//for (int i = 1; i < D.y; ++i)
	//{
	//	if (ContactImage.at<unsigned char>(i, D.x) == 255 && ContactImage.at<unsigned char>(i - 1, D.x) == 0)
	//	{
	//		halfImageStartRows = i + 5;
	//		halfImageStartRows = std::min(halfImageStartRows, D.y);
	//		break;
	//	}
	//}

	//cv::Mat edges;
	//Canny(ContactImage, edges, 100, 200, 3, false);
	//int width = std::min(D.x, edges.cols - D.x);
	//cv::Mat left = edges(cv::Range(halfImageStartRows, D.y), cv::Range(0, width));
	//cv::Mat right = edges(cv::Range(halfImageStartRows, D.y), cv::Range(D.x, D.x + width));

	//showImage("left", left,1,false);
	//showImage("right", right, 1, false);
	cv::bitwise_not(left, left);
	cv::bitwise_not(right, right);
	int leftValue = getSumOfBinaryImage(left);
	int rightValue = getSumOfBinaryImage(right);
	//printf("leftValue = %d\n", leftValue);
	//printf("rightValue = %d\n", rightValue);

	return std::make_pair(leftValue, rightValue);
}

void imageProcess::leftOrRightJudgment(const std::pair<int, int>& upValue, const std::pair<int, int>& downValue, std::vector<int>& result, int holeNum)
{
	double diffUp = upValue.first - upValue.second;
	diffUp = diffUp * abs(diffUp);//����ֵ������Ŷȸ���
	double diffDown = downValue.first - downValue.second;
	diffDown = diffDown * abs(diffDown);
	double diff = diffUp + diffDown;
	printf("upValue: %d, %d\n", upValue.first, upValue.second);
	printf("downValue: %d, %d\n", downValue.first, downValue.second);
	printf("diffUp: %f, diffDown: %f \n", diffUp, diffDown);
	printf("diff: %f \n", diff);
	if (holeNum < 2)//ȱ�����ұ���������
	{
		if (diff >= (0 - judgementDeadValue))
		{
			result[holeNum] = 1;//ȱ������
			printf("left \n");
		}
		else
		{
			result[holeNum] = 2;//ȱ������
			printf("right \n");
		}
	}
	else//ȱ�����������
	{
		if (diff <= judgementDeadValue)
		{
			result[holeNum] = 2;//ȱ������
			printf("right \n");
		}
		else
		{
			result[holeNum] = 1;//ȱ������
			printf("left \n");
		}
	}
	//if (upValue.first + downValue.first > upValue.second + downValue.second)
	//{
	//	result[holeNum] = 1;//ȱ������
	//	printf("upValue: %d, %d\n", upValue.first, upValue.second);
	//	printf("downValue: %d, %d\n", downValue.first, downValue.second);
	//	printf("left \n");
	//}
	//else if (upValue.first + downValue.first < upValue.second + downValue.second)
	//{
	//	result[holeNum] = 2;
	//	printf("upValue: %d, %d\n", upValue.first, upValue.second);
	//	printf("downValue: %d, %d\n", downValue.first, downValue.second);
	//	printf("right \n");
	//}
	//else
	//{
	//	printf("���ʧ�ܣ��������! upLeftValue = %d, upRightValue = %d, downLeftValue = %d, downRightValue = %d\n",
	//		upValue.first, upValue.second, downValue.first, downValue.second);
	//}
}


void imageProcess::getHoleImageCoordinates(std::vector<struct boxCoordinates>& holeImageCoordinates, const std::vector<struct boxCoordinates>& doubleHoleImageCoordinates, double base_thread,
	const cv::Mat& grayRoi, const cv::Mat& binaryRoi)
{
	holeImageCoordinates.resize(4);
	assert(doubleHoleImageCoordinates.size() == 2);

	for (size_t imageIndx = 0; imageIndx < 2; ++imageIndx)
	{
		cv::Mat doubleHoleimage = grayRoi(cv::Range(doubleHoleImageCoordinates[imageIndx].startRows, doubleHoleImageCoordinates[imageIndx].endRows),
			cv::Range(doubleHoleImageCoordinates[imageIndx].startCols, doubleHoleImageCoordinates[imageIndx].endCols)).clone();

		//drawGrayHist(doubleHoleimage);
		//cv::Mat eauql;
		//cv::equalizeHist(doubleHoleimage, eauql);
		//drawGrayHist(eauql);
		//showImage("eauql", eauql, 0, true);
		cv::Mat temp;
		double OTSU_threadNum = cv::threshold(doubleHoleimage, doubleHoleimage, 0, 255, CV_THRESH_OTSU);
		OTSU_threadNum = cv::threshold(doubleHoleimage, doubleHoleimage, OTSU_threadNum * 0.9, 255, CV_THRESH_BINARY);
		printf("OTSU_threadNum for double hole : %f \n", OTSU_threadNum);
		if (abs(OTSU_threadNum - base_thread) > 40)//�ֲ���ֵ�쳣����ȫ�ֵĶ�ֵ��ͼƬ
		{
			printf("OTSU_threadNum abnormal! base_thread = %f\n", base_thread);
			doubleHoleimage = binaryRoi(cv::Range(doubleHoleImageCoordinates[imageIndx].startRows, doubleHoleImageCoordinates[imageIndx].endRows),
				cv::Range(doubleHoleImageCoordinates[imageIndx].startCols, doubleHoleImageCoordinates[imageIndx].endCols));
		}

		cv::Mat element;
		element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
		morphologyEx(doubleHoleimage, doubleHoleimage, cv::MORPH_CLOSE, element);
		//showImage("doubleHoleimage", doubleHoleimage, 0, true);

		std::vector<cv::Point2i> DCoordinates;//��ȡD�ο�����,����y�������������
		getFourContactDHoleCoordinates(doubleHoleimage, DCoordinates, imageIndx);
		for (int i = 0; i < 4; ++i)
		{
			printf("DCoordinates[%d]: x=%d, y=%d \n", i, DCoordinates[i].x, DCoordinates[i].y);
		}


		int upcur = (DCoordinates[1].y + DCoordinates[2].y) / 2;//��2���͵�3��֮������������ķֽ���
		int downcur = upcur;
		cv::Mat rowWhiteSum = getSumOfEachRowOrColumnForBinaryImage(doubleHoleimage, true);
		double* ptr = rowWhiteSum.ptr<double>(0);
		//for (int ii = 0; ii < doubleHoleimage.rows; ++ii)//�鿴��ͶӰ���
		//{
		//	printf("%f \n", ptr[ii]);
		//}
		const int deadWhiteNum = 10;
		while (DCoordinates[1].y < upcur || downcur < DCoordinates[2].y)
		{
			if (ptr[downcur - 2] < deadWhiteNum && ptr[downcur - 1] < deadWhiteNum && ptr[downcur] < deadWhiteNum && ptr[downcur + 1] < deadWhiteNum && ptr[downcur + 2] < deadWhiteNum)
			{
				setDemarcationLineCoordinates(holeImageCoordinates, doubleHoleImageCoordinates, imageIndx, downcur);
				break;
			}
			else if (ptr[upcur - 2] < deadWhiteNum && ptr[upcur - 1] < deadWhiteNum && ptr[upcur] < deadWhiteNum && ptr[upcur + 1] < deadWhiteNum && ptr[upcur + 2] < deadWhiteNum)
			{
				setDemarcationLineCoordinates(holeImageCoordinates, doubleHoleImageCoordinates, imageIndx, upcur);
				break;
			}
			upcur = upcur - 5;
			downcur = downcur + 5;
		}
		if (DCoordinates[1].y >= upcur && downcur >= DCoordinates[2].y)
		{
			printf("Image is abnormal��split up and down by default parameters \n");
			int cur = (DCoordinates[1].y + DCoordinates[2].y) / 2;
			setDemarcationLineCoordinates(holeImageCoordinates, doubleHoleImageCoordinates, imageIndx, cur);
		}
	}
}

void imageProcess::setDemarcationLineCoordinates(std::vector<struct boxCoordinates>& holeImageCoordinates,
	const std::vector<struct boxCoordinates>& doubleHoleImageCoordinates, const size_t imageIndx, const int cur)
{
	holeImageCoordinates[imageIndx].startRows = doubleHoleImageCoordinates[imageIndx].startRows + cur;
	holeImageCoordinates[imageIndx].endRows = doubleHoleImageCoordinates[imageIndx].endRows;
	holeImageCoordinates[imageIndx].startCols = doubleHoleImageCoordinates[imageIndx].startCols;
	holeImageCoordinates[imageIndx].endCols = doubleHoleImageCoordinates[imageIndx].endCols;

	holeImageCoordinates[imageIndx + 2].startRows = doubleHoleImageCoordinates[imageIndx].startRows;
	holeImageCoordinates[imageIndx + 2].endRows = doubleHoleImageCoordinates[imageIndx].startRows + cur;
	holeImageCoordinates[imageIndx + 2].startCols = doubleHoleImageCoordinates[imageIndx].startCols;
	holeImageCoordinates[imageIndx + 2].endCols = doubleHoleImageCoordinates[imageIndx].endCols;
}

void imageProcess::getFourContactDHoleCoordinates(const cv::Mat& image, std::vector<cv::Point2i>& DCoordinates, const int imageIndx)
{
	cv::Mat DHoleImage = image.clone();
	DHoleImage.convertTo(DHoleImage, CV_8U, 255);
	if (DHoleImage.at<unsigned char>(0, 0) == 0)
	{
		cv::floodFill(DHoleImage, cv::Point(0, 0), 255);
	}
	if (DHoleImage.at<unsigned char>(DHoleImage.rows - 1, 0) == 0)//���к��м���y��x
	{
		cv::floodFill(DHoleImage, cv::Point(0, DHoleImage.rows - 1), 255);
	}
	if (DHoleImage.at<unsigned char>(0, DHoleImage.cols - 1) == 0)
	{
		cv::floodFill(DHoleImage, cv::Point(DHoleImage.cols - 1, 0), 255);
	}
	if (DHoleImage.at<unsigned char>(DHoleImage.rows - 1, DHoleImage.cols - 1) == 0)
	{
		cv::floodFill(DHoleImage, cv::Point(DHoleImage.cols - 1, DHoleImage.rows - 1), 255);
	}


	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::bitwise_not(DHoleImage, DHoleImage);
	//showImage("DHoleImage", DHoleImage, 0, true);
	//saveImage("DHoleImage.bmp", DHoleImage);

	findContours(DHoleImage, contours, hierarchy, CV_RETR_EXTERNAL, cv::CHAIN_APPROX_NONE, cv::Point());

	std::vector<std::pair<double, int>> contoursArea(contours.size());
	for (size_t i = 0, contoursSize = contours.size(); i < contoursSize; i++)
	{
		cv::RotatedRect box;
		box = cv::minAreaRect(cv::Mat(contours[i]));
		if (getLengthCompareWidth(box) > lengthCompareWidth)
		{
			printf("LengthCompareWidth is too large:%f \n", getLengthCompareWidth(box));
			continue;
		}
		contoursArea[i] = std::make_pair(contourArea(contours[i]), i);
	}
	std::sort(contoursArea.begin(), contoursArea.end(), [&](auto x, auto y) {return x.first > y.first; });//����

	std::array<std::vector<std::pair<double, cv::Point2i>>, 4>DScoreAndPoint;
	auto itor = contoursArea.begin();
	while (itor != contoursArea.end())
	{
		printf("area: %f \n", itor->first);
		if (itor->first < maxDHoleArea && itor->first > minDHoleArea)
		{
			cv::Moments momentOfD = cv::moments(contours[itor->second]);
			cv::Point2i temp{ static_cast<int>(1.0 * momentOfD.m10 / momentOfD.m00) ,static_cast<int>(1.0 * momentOfD.m01 / momentOfD.m00) };
			double minDistance = 0;
			int Dindx = nearestD(temp, minDistance, D_hole_XY_in_two_part_image);
			DScoreAndPoint[Dindx].emplace_back(minDistance / itor->first, temp);//�����ɾ����������õ���ԽСԽ�ã������ǰ����������С������Ϊ0
		}
		itor++;
	}
	//std::sort(DCoordinates.begin(), DCoordinates.end(), [&](auto d1, auto d2) {return d1.y < d2.y; });//����
	DCoordinates.clear();
	DCoordinates.resize(4, cv::Point2i(0, 0));
	for (size_t i = 0; i < 4; ++i)
	{
		if (DScoreAndPoint[i].size() == 0)
		{
			DCoordinates[i] = D_hole_XY_in_two_part_image.read(i);
			continue;
		}
		std::sort(DScoreAndPoint[i].begin(), DScoreAndPoint[i].end(), [&](auto d1, auto d2) {return d1.first < d2.first; });//����
		DCoordinates[i] = DScoreAndPoint[i][0].second;
		//�������»�׼����
		D_hole_XY_in_two_part_image.update(i, DCoordinates[i]);
	}
}

void imageProcess::saveImage(const std::string& name, const cv::Mat& src)
{
	try {
		cv::imwrite(name.c_str(), src);
	}
	catch (std::exception& e)
	{
		printf("save Image(%s) failure!(%s) \n", name.c_str(), e.what());
	}
}

void imageProcess::showImage(const std::string& name, const cv::Mat& src, const int namewindow, const bool wait)
{
	try {
		if (namewindow == 0)
		{
			cv::namedWindow(name.c_str(), namewindow);
		}
		cv::imshow(name.c_str(), src);
		//cv::imwrite("srcP.bmp", src);
		if (wait)
		{
			cv::waitKey(0);
		}
	}
	catch (std::exception& e)
	{
		printf("show Image(%s) failure!(%s) \n", name.c_str(), e.what());
	}
}

double imageProcess::getRoiAboutFourParts(const cv::Mat& src, cv::Mat& grayRoi, cv::Mat& binaryRoi)
{
	//�ٶ����̺������λ�û������ֲ��䣬�����任����Ƭÿ����������̶�

	grayRoi = src(cv::Range(locationForROI.startRows, locationForROI.endRows), cv::Range(locationForROI.startCols, locationForROI.endCols));

	grayRoi = grayRoi.clone();
	binaryRoi = grayRoi.clone();
	//drawGrayHist(binaryRoi);
	//showImage("grayRoi", grayRoi, 0, true);

	grayRoi.convertTo(grayRoi, CV_8U, 1.0);
	//for (int i = 0; i < grayRoi.rows; ++i)
	//{
	//	unsigned char *ptr = grayRoi.ptr<unsigned char>(i);
	//	for (int j = 0; j < grayRoi.cols; ++j)
	//	{
	//		ptr[j] = static_cast<unsigned char>(round(pow(ptr[j], 0.98)));
	//	}
	//}
	//showImage("grayRoi.9", grayRoi, 0, true);
	//drawGrayHist(grayRoi);

	////������,����϶
	cv::Mat element;
	element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	morphologyEx(grayRoi, binaryRoi, cv::MORPH_CLOSE, element);

	//cv::Mat temp;
	double OTSU_threadNum = cv::threshold(binaryRoi, binaryRoi, 0, 255, CV_THRESH_OTSU);

	OTSU_threadNum = cv::threshold(binaryRoi, binaryRoi, OTSU_threadNum*0.9, 255, CV_THRESH_BINARY);
	//showImage("binaryRoi", binaryRoi, 0, true);
	return OTSU_threadNum;
}

double imageProcess::getLengthCompareWidth(const cv::RotatedRect& box)
{
	if (box.size.height >= box.size.width)//�ش��ڵ���1
	{
		return box.size.height / box.size.width;
	}
	else
	{
		return box.size.width / box.size.height;

	}
}

void imageProcess::drawGrayHist(const cv::Mat& src)
{
	int channels[] = { 0 };
	int bins = 256;

	cv::Mat hist;
	int hist_size[] = { bins };
	float range[] = { 0,256 };
	const float* ranges[] = { range };

	cv::calcHist(&src, 1, 0, cv::Mat(), hist, 1, hist_size, ranges);
	double max_val;
	minMaxLoc(hist, 0, &max_val);//�����������Сֵ�����ֵ��λ��
	int scale = 2;
	int hist_height = 256;
	cv::Mat hist_ing = cv::Mat::zeros(hist_height, scale * bins, CV_8UC3);
	for (int i = 0; i < bins; i++)
	{
		float bin_val = hist.at<float>(i);//ͼ��ĻҶ�Ƶ�ʱ�
		int inten = cvRound(bin_val * hist_height / max_val);//���Ƹ߶�
		cv::rectangle(hist_ing, cv::Point(scale * i, hist_height - 1), cv::Point((i + 1) * scale - 1, hist_height - inten), CV_RGB(255, 255, 255));

	}
	cv::imshow("ֱ��ͼ", hist_ing);
	cv::waitKey(0);
}

void imageProcess::sharpenImage1(const cv::Mat& image, cv::Mat& result)
{
	//��������ʼ���˲�ģ��
	cv::Mat kernel(3, 3, CV_32F, cv::Scalar(0));
	kernel.at<float>(1, 1) = 5.0;
	kernel.at<float>(0, 1) = -1.0;
	kernel.at<float>(1, 0) = -1.0;
	kernel.at<float>(1, 2) = -1.0;
	kernel.at<float>(2, 1) = -1.0;

	result.create(image.size(), image.type());

	//��ͼ������˲�
	cv::filter2D(image, result, image.depth(), kernel);
}

int imageProcess::nearestD(const cv::Point2i& p, double& minValue, const D_XY_base& base)
{
	minValue = getDistance(p, base.read(0));
	int indx = 0;
	int len = base.size();
	for (int i = 1; i < len; ++i)
	{
		double value = getDistance(p, base.read(i));
		if (minValue > value)
		{
			minValue = value;
			indx = i;
		}
	}
	return indx;
}
