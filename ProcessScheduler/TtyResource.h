#pragma once
#include <queue>
#include "Resource.h"
class TtyResource
{
public:
	TtyResource();
	~TtyResource();
	void enqueue(Process process);
	bool isBusy();
	Process tick(int time);
private:
	std::queue<Process> ttyQueue;

};
