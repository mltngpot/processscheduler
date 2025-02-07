#pragma once
#include "Resource.h"
#include <queue>
class DiskResource
{
public:
	DiskResource();
	~DiskResource();
	void enqueue(Process process);
	bool isBusy();
	Process tick(int time);
private:
	std::queue<Process> diskQueue;
};

