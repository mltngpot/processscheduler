#pragma once
#include "Process.h"
class Resource
{
public:
	Resource();
	~Resource();
	virtual bool isBusy() = 0;
	virtual void enqueue(Process process) = 0;
};

