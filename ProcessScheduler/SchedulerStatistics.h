#pragma once
#include <string>
class SchedulerStatistics
{
public:
	static void realTimeProcessCompleted();
	static void realTimeProcessMissedDeadline();
	static void interactiveProcessCompleted();
	static void diskAccess(int duration);
	static void cpuIdle();
	static void diskIdle();
	static void totalRuntime(int duration);
	static void newDiskAccess();
	static std::string toString();
private:
	static int realTimeProcessesCompleted;
	static int realTimeProcessesMissedDeadline;
	static int interactiveProcessesCompleted;
	static int diskAccesses;
	static int cpuIdleTime;
	static int diskIdleTime;
	static int totalRunTime;
};

