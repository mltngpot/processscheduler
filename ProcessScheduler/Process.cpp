#include "Process.h"
#include "ProcessType.cpp"
#include <stdexcept>
#include <iostream>

Process::Process(ProcessType type, int start)
{
	validate(start, 0);
	initialize(type, start, 0);
}

Process::Process(ProcessType type, int start, int deadline)
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
	case State::QUEUED:
		return "QUEUED";
	case State::WAITING:
		return "WAITING";
	default:
		return "UNKNOWN";
	}
}

void Process::addTask(Task* task)
{
	taskList.push(task);
}

Task* Process::getCurrentTask()
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
	return this->state;
}

void Process::setState(State state)
{
	std::cout << "(pid: " << pid << ") Setting state to " << stateToString(state) << std::endl;
	this->state = state;
}


void Process::tick()
{
	if (getState() == State::READY)
		setState(State::RUNNING);

	currentTask->tick();
	if (currentTask->getRuntime() == 0) {
		std::cout << "(pid: " << pid << ") " << currentTask->getType() << " Task Completed." << std::endl;
		setState(State::READY);
		delete currentTask;
		currentTask = nullptr;
		if (!taskList.empty()) {
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
	if (taskList.empty())
		delete this;
	else if (currentTask == nullptr)
	{
		currentTask = taskList.front();
		taskList.pop();
	}
}

std::string Process::toString()
{
	std::string str = "pid(" + std::to_string(pid) + ")";
	str += " type(" + std::to_string(type) + ")";
	str += " state(" + stateToString(state) + ")";
	return str;
}
