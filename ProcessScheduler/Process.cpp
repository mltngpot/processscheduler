#include "Process.h"
#include "ProcessType.cpp"
#include <stdexcept>
#include <iostream>

const Process Process::IDLE = Process(ProcessType::IDLE, 0);

Process::Process(ProcessType type, int start) : currentTask(TaskType::CPU, 0)
{
	validate(start, 0);
	initialize(type, start, 0);
}

Process::Process(ProcessType type, int start, int deadline) : currentTask(TaskType::CPU, 0)
{
	validate(start, deadline);
	initialize(type, start, deadline);
}

Process::~Process()
{
}

void Process::validate(int start, int deadline)
{
	if (start < 0) throw std::runtime_error::runtime_error("Invalid process start time.");
	if (deadline < 0) throw std::runtime_error::runtime_error("Invalid realtime process deadline.");
}

void Process::initialize(ProcessType type, int start, int deadline)
{
	this->start = start;
	this->type = type;
	this->deadline = deadline;
	this->state = State::QUEUED;
	this->pid = 0;
}

std::string Process::stateToString(State state)
{
	switch (state) {
	case State::READY:
		return "READY";
	case State::RUNNING:
		return "RUNNING";
	case State::TERMINATED:
		return "TERMINATED";
	case State::QUEUED:
		return "QUEUED";
	case State::WAITING:
		return "WAITING";
	default:
		return "UNKNOWN";
	}
}

void Process::addTask(Task task)
{
	if (currentTask.getRuntime() == 0)
		currentTask = task;
	else
		taskList.push(task);
}

Task Process::getCurrentTask()
{
	return currentTask;
}

ProcessType Process::getType()
{
	return type;
}

int Process::getStart()
{
	return start;
}

bool Process::pastDeadline(int time)
{
	if (type == ProcessType::REALTIME && time > deadline)
		return true;
	return false;
}

State Process::getState()
{
	return state;
}

void Process::setState(State state)
{
	std::cout << "(pid: " << pid << ") Setting state to " << stateToString(state) << std::endl;
	this->state = state;
}


void Process::tick()
{
	if (type == ProcessType::IDLE) 
		return;
	if (getState() == State::READY) 
		setState(State::RUNNING);

	currentTask.tick();
	if (currentTask.getRuntime() == 0) {
		std::cout << "(pid: " << pid << ") " << currentTask.getType() <<  " Task Completed." <<  std::endl;
		if (taskList.empty())
			setState(State::TERMINATED);
		else {
			setState(State::READY);
			currentTask = taskList.front();
			taskList.pop();
		}
	}
}

int Process::getPid()
{
	return pid;
}

void Process::setPid(int pid)
{
	this->pid = pid;
}
