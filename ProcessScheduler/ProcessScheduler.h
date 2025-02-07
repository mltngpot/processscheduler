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
	void dequeueInput();
	void queueProcess(Process* process);
	void tick();
	bool stillProcessing();
	void printStats();
private:
	std::queue<Process*> inputProcessesQueue;
	std::queue<Process*> deleteQueue;
	CpuResource cpuResource;
	TtyResource ttyResource;
	DiskResource diskResource;
	SchedulerStatistics statistics;
	int currentPid;
	int time;

	void clearDeletes();
};