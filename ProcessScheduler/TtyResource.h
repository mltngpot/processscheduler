#pragma once
#include <queue>
#include "Process.h"
#include "SchedulerStatistics.h"
class TtyResource
{
public:
	TtyResource(SchedulerStatistics statistics);
	~TtyResource();
	void enqueue(Process process);
	bool isBusy();
	Process tick(int time);
private:
	std::queue<Process> ttyQueue;
	SchedulerStatistics statistics;
};
