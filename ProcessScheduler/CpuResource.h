#pragma once
#include <queue>
#include "Process.h"
#include "SchedulerStatistics.h"
class CpuResource
{
public:
	CpuResource(SchedulerStatistics statistics);
	~CpuResource();
	void enqueue(Process process);
	bool isBusy();
	Process tick(int time);
	Process currentProcess();
private:
	void updateCurrentProcess(int time);
	std::queue<Process> realTimeCpuQueue;
	std::queue<Process> interactiveCpuQueue;
	Process currentRealtime;
	Process currentInteractive;
	SchedulerStatistics statistics;
};
