#pragma once
#ifndef MYTHREADPOOL_H
#define MYTHREADPOOL_H

#include "ThreadPool.h"
#include "imageProcess.h"
#include <exception> 
#include <QObject>

std::vector<int> task(std::shared_ptr<imageProcess> ptrImageprocess, cv::Mat& img, int num);


class myThreadpool :public QObject{
	Q_OBJECT

public:
	myThreadpool():num(4)
	{
		if (pool == nullptr)
		{
			try {
				pool = std::make_unique<ThreadPool>(num);
			}
			catch (std::exception& e)
			{
				printf("New ThreadPool Falue (%s) \n", e.what());
				std::exit(0);
			}
		}
		testResults.resize(num);
	}
	~myThreadpool()
	{
	}
	myThreadpool(myThreadpool&) = delete;
	myThreadpool& operator=(myThreadpool&) = delete;

	void waitTestResults()
	{
		assert(testFutures.size()== num);
		int i = 0;
		while (!testFutures.empty())
		{
			std::future<std::vector<int>> fut = std::move(testFutures.front());
			testFutures.pop();
			testResults[i] = fut.get();
			++i;
		}
	}
	std::vector<std::vector<int>> getTestResults()
	{
		return testResults;
	}

private:
	const int num;
	std::unique_ptr<ThreadPool> pool = nullptr;
	std::queue < std::future<std::vector<int>>> testFutures;
	std::vector<std::vector<int>> testResults;

signals:

public slots:
	void enqueue(std::shared_ptr<imageProcess> ptrImageprocess, cv::Mat &img, int num)
	{
		testFutures.push(pool->enqueue(task, ptrImageprocess, img, num));
	}
};


#endif