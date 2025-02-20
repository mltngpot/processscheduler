#include "SchedulerStatistics.h"

int SchedulerStatistics::realTimeProcessesCompleted = 0;
int SchedulerStatistics::realTimeProcessesMissedDeadline = 0;
int SchedulerStatistics::interactiveProcessesCompleted = 0;
int SchedulerStatistics::diskAccesses = 0;
int SchedulerStatistics::cpuIdleTime = 0;
int SchedulerStatistics::diskIdleTime = 0;
int SchedulerStatistics::totalRunTime = 0;

void SchedulerStatistics::realTimeProcessCompleted()
{
	SchedulerStatistics::realTimeProcessesCompleted++;
}

void SchedulerStatistics::realTimeProcessMissedDeadline()
{
	SchedulerStatistics::realTimeProcessesMissedDeadline++;
}

void SchedulerStatistics::interactiveProcessCompleted()
{
	SchedulerStatistics::interactiveProcessesCompleted++;
}

void SchedulerStatistics::diskAccess(int duration)
{
	if (duration == 0)
		SchedulerStatistics::diskIdle();
	else
		SchedulerStatistics::diskAccesses += duration;
}

void SchedulerStatistics::cpuIdle()
{
	SchedulerStatistics::cpuIdleTime++;
}

void SchedulerStatistics::diskIdle()
{
	SchedulerStatistics::diskIdleTime++;
}

void SchedulerStatistics::totalRuntime(int duration)
{
	SchedulerStatistics::totalRunTime += duration;
}

void SchedulerStatistics::newDiskAccess()
{
	SchedulerStatistics::diskAccesses++;
}

std::string SchedulerStatistics::toString()
{
	std::string endl = "\n";
	return "Statistics:" + endl +
		"Real-time processes completed : " + std::to_string(SchedulerStatistics::realTimeProcessesCompleted) + endl +
		"Real-time processes missed deadline: " + std::to_string(SchedulerStatistics::realTimeProcessesMissedDeadline) + endl +
		"Interactive processes completed: " + std::to_string(SchedulerStatistics::interactiveProcessesCompleted) + endl +
		"Disk accesses: " + std::to_string(SchedulerStatistics::diskAccesses) + endl +
		"CPU idle time: " + std::to_string(SchedulerStatistics::cpuIdleTime) + endl +
		"Disk idle time: " + std::to_string(SchedulerStatistics::diskIdleTime) + endl +
		"Total runtime: " + std::to_string(SchedulerStatistics::totalRunTime) + endl;
}
