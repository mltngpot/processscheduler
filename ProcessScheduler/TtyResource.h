#pragma once
#include <queue>
#include "Process.h"
#include "SchedulerStatistics.h"
class TtyResource
{
public:
	TtyResource();
	~TtyResource();
	void enqueue(Process* process);
	bool isBusy();
	Process* tick(int time);
	void printStatus();
private:
	std::queue<Process*> ttyQueue;
	Process* currentProcess;
};
