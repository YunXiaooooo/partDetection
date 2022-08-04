#pragma once
#ifndef PROCESSCONTROL_H
#define PROCESSCONTROL_H

#include "ThreadPool.h"
#include "imageProcess.h"
#include <exception> 
#include <QObject>

std::vector<int> processTask(std::shared_ptr<imageProcess> ptrImageprocess, cv::Mat& img, const int num);


class processControl :public QObject
{
	Q_OBJECT
public:
	processControl();	
	~processControl()
	{}
	processControl(const processControl&) = delete;
	processControl& operator=(const processControl&) = delete;

	void waitTestResults();
	std::vector<std::vector<int>> getTestResults()
	{
		return testResults;
	}
private:
	const int threadNum;
	const int taskNum = 12;
	std::unique_ptr<ThreadPool> pool = nullptr;
	std::shared_ptr<imageProcess> ptrImageprocess = nullptr;
	std::queue<std::future<std::vector<int>>> testFutures;
	std::queue<int> testOrder;
	std::vector<std::vector<int>> testResults;

public slots:
	void enqueue(cv::Mat& img, const int num)
	{
		testFutures.push(pool->enqueue(processTask, ptrImageprocess, img, num));
		testOrder.push(num);
	}
};






#endif