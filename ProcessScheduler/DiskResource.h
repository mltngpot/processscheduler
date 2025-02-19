#pragma once
#include <queue>
#include "Process.h"
#include "SchedulerStatistics.h"
class DiskResource
{
public:
	DiskResource(SchedulerStatistics statistics);
	~DiskResource();
	void enqueue(Process process);
	bool isBusy();
	Process tick(int time);
private:
	std::queue<Process> diskQueue;
	SchedulerStatistics statistics;
};

