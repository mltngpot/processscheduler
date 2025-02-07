#pragma once
#include <queue>
#include "Process.h"
class CpuResource
{
public:
	CpuResource();
	~CpuResource();
	void enqueue(Process process);
	bool isBusy();
	Process tick(int time);
private:
	std::queue<Process> realTimeCpuQueue;
	std::queue<Process> interactiveCpuQueue;
};
