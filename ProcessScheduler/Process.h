#pragma once
#include <queue>
#include "ProcessType.cpp"
#include "Task.h"
#include "State.cpp"

class Process
{
public:
	Process(ProcessType type, int start);
	Process(ProcessType type, int start, int deadline);
	~Process();
	void addTask(Task task);
	ProcessType getType();
	int getStart();
	bool pastDeadline(int time);
	State getState();
	void setState(State state);
	Task currentTask();
	void tick();
	int getPid();
	void setPid(int pid);
	static const Process IDLE;
private:
	ProcessType type;
	State state;
	int start;
	int deadline;
	int pid;
	std::queue<Task> taskList;

	void validate(int start, int deadline);
	void initialize(ProcessType type, int start, int deadline);
};

