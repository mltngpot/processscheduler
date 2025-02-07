#pragma once
#include "TaskType.cpp"
class Task
{
public:
	Task(TaskType type, int runtime);
	~Task();
	TaskType getType();
	int getRuntime();
	void tick();
private:
	TaskType type;
	int runtime;
};

