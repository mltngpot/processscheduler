#pragma once
#include <queue>
#include <string>
#include "ProcessType.cpp"
#include "Task.h"
#include "State.cpp"

class Process
{
public:
	Process(ProcessType type, int start);
	Process(ProcessType type, int start, int deadline);
	~Process();
	void addTask(Task* task);
	Task* getCurrentTask();
	ProcessType getType();
	int getStart();
	bool pastDeadline(int time);
	State getState();
	void setState(State state);
	void tick();
	int getPid();
	void setPid(int pid);
	std::string toString();
private:
	void validate(int start, int deadline);
	void initialize(ProcessType type, int start, int deadline);
	static std::string stateToString(State state); // Change to static

	Task* currentTask;
	ProcessType type;
	State state;
	int start;
	int deadline;
	int pid;
	std::queue<Task*> taskList;

};
