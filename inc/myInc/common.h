#ifndef _COMMON_H_
#define _COMMON_H_

#include <thread>
#include <memory>
#include <mutex>
#include <condition_variable>  
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "CameraDrive.h"


class palletRecognitionResults
{
public:
	palletRecognitionResults()
	{
		states.resize(4);
		for (auto s : states)
		{
			s.resize(12);
		}
		flag = false;
	}
	~palletRecognitionResults()
	{
	}
	void setStatesAboutOneGrabPic(int num, std::vector<int> stateValue)
	{
		std::lock_guard<std::mutex> lk(mut);
		states[num] = stateValue;
	}
	void setFlag(bool tf)
	{
		flag = tf;
	}
	bool readFlag()
	{
		return flag;
	}
	std::vector<int> readStatesAboutOneGrabPic(int num)
	{
		std::lock_guard<std::mutex> lk(mut);
		return states[num];
	}
	int readStatesAboutOneLocation(int num)
	{
		int picNum = num / 12;
		int locationInPic = num % 12;
		std::lock_guard<std::mutex> lk(mut);
		return states[picNum][locationInPic];
	}
private:
	std::mutex mut;
	std::vector<std::vector<int>> states;
	std::atomic_bool flag;
};

class conditionVariableTool
{
public:
	conditionVariableTool()
	{
		grabFlag = false;
		grabFinishedFlag = false;
		terminnationFlag = false;
	}
	~conditionVariableTool()
	{
	}
	conditionVariableTool(const conditionVariableTool& cdTool) = delete;

	void setGrabFlag(bool tf)
	{
		grabFlag = tf;
	}
	void setgrabFinishedFlag(bool tf)
	{
		grabFinishedFlag = tf;
	}
	void setTerminnationFlag(bool tf)
	{
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
		return grabFlag;
	}
	bool readTerminnationFlag()
	{
		return terminnationFlag;
	}
	bool readGrabFinishedFlag()
	{
		return grabFinishedFlag;
	}
private:
	std::mutex mut;
	std::condition_variable cond;
	std::atomic_bool grabFlag,grabFinishedFlag;
	std::atomic_bool terminnationFlag;


};


int DisplayConfirmSaveAsMessageBox();
void grabByHk(std::unique_ptr<CameraDrive> ptrMyCameras);

extern palletRecognitionResults palletRecognitionStates;
extern conditionVariableTool cdTool;
#endif 