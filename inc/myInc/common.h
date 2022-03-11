#ifndef _COMMON_H_
#define _COMMON_H_

#include <thread>
#include <mutex>
#include <condition_variable>  
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "CameraDrive.h"

class resultCodes
{
public:
	resultCodes()
	{
		for (int i = 0; i < 4; i++)
		{
			code[i] = 0;
		}
		flag = false;
	}
	~resultCodes()
	{
	}
	void setCodeValue(int num, int value)
	{
		std::lock_guard<std::mutex> lk(mut);
		code[num] = value;
	}
	void setFlag(bool tf)
	{
		std::lock_guard<std::mutex> lk(mut);
		flag = tf;
	}
	bool readFlag()
	{
		std::lock_guard<std::mutex> lk(mut);
		return flag;
	}
	unsigned int readCodeValue(int num)
	{
		std::lock_guard<std::mutex> lk(mut);
		return code[num];
	}
private:
	std::mutex mut;
	unsigned int code[4];
	bool flag;
};

class conditionVariableTool
{
public:
	conditionVariableTool()
	{
		grabFlag = false;
		terminnationFlag = false;
	}
	~conditionVariableTool()
	{
	}
	conditionVariableTool(const conditionVariableTool& cdTool) = delete;

	void setGrabFlag(bool tf)
	{
		std::lock_guard<std::mutex> lk(mut);
		grabFlag = tf;
	}
	void setTerminnationFlag(bool tf)
	{
		std::lock_guard<std::mutex> lk(mut);
		terminnationFlag = tf;
	}
	void conditionVariableToolWait()
	{
		std::unique_lock<std::mutex> lk(mut);
		cond.wait(lk, [&] {return grabFlag || terminnationFlag; });
	}
	void conditionVariableToolNotifyOne()
	{
		cond.notify_one();
	}
	void conditionVariableToolNotifyAll()
	{
		cond.notify_all();
	}
	bool readGrabFlag()
	{
		std::lock_guard<std::mutex> lk(mut);
		return grabFlag;
	}
	bool readTerminnationFlag()
	{
		std::lock_guard<std::mutex> lk(mut);
		return terminnationFlag;
	}
private:
	std::mutex mut;
	std::condition_variable cond;
	bool grabFlag;
	bool terminnationFlag;


};


int DisplayConfirmSaveAsMessageBox();
void grabByHk(CameraDrive* ptrMyCameras);

extern resultCodes testResults;
extern conditionVariableTool cdTool;
#endif 