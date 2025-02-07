#pragma once
#include <queue>
#include "Process.h"
#include <string>
#include "CpuResource.h"
#include "DiskResource.h"
#include "TtyResource.h"
#include <istream>
#include "SchedulerStatistics.h"

class ProcessScheduler
{
public:
	ProcessScheduler();
	void readInput(std::istream& in);
	void run();
	void dequeueInput(int time);
	void queueProcess(Process process);
	void tick(int time);
	bool stillProcessing();
	void printStats();
private:
	std::queue<Process> inputProcessesQueue;
	CpuResource cpuResource;
	TtyResource ttyResource;
	DiskResource diskResource;
	SchedulerStatistics statistics;
	int currentPid;
};