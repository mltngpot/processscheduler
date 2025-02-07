#pragma once
#include <queue>
#include "Process.h"
#include "SchedulerStatistics.h"
class DiskResource
{
public:
	DiskResource();
	~DiskResource();
	void enqueue(Process* process);
	bool isBusy();
	Process* tick(int time);
	void printStatus();
private:
	std::queue<Process*> diskQueue;
	Process* currentProcess;
	int queuedProcesses = 0;
};

