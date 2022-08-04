#include "myTimer.h"

void myTimerTask(std::shared_ptr<communicationToolProxy> toolProxy)
{
	toolProxy->task();
}
