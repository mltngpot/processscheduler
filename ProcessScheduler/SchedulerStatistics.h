#pragma once
#include <string>
class SchedulerStatistics
{
public:
	SchedulerStatistics();
	~SchedulerStatistics();
	void realTimeProcessCompleted();
	void realTimeProcessMissedDeadline();
	void interactiveProcessCompleted();
	void diskAccess(int duration);
	void cpuIdle();
	void diskIdle();
	void totalRuntime(int duration);
	std::string toString();
private:
	int realTimeProcessesCompleted = 0;
	int realTimeProcessesMissedDeadline = 0;
	int interactiveProcessesCompleted = 0;
	int diskAccesses = 0;
	int cpuIdleTime = 0;
	int diskIdleTime = 0;
	int totalRunTime = 0;
};

