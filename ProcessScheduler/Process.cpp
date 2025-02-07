#include "Process.h"
#include "ProcessType.cpp"
#include <stdexcept>
#include <iostream>

const Process Process::IDLE = Process(ProcessType::IDLE, 0);

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

void Process::addTask(Task task)
{
	taskList.push(task);
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
	if (type == ProcessType::REALTIME && time > deadline) {
		std::cout << "(pid: " << pid << ") Realtime Process missed deadline." << std::endl;
		setState(State::TERMINATED);
		return true;
	}
	return false;
}

State Process::getState()
{
	return state;
}

void Process::setState(State state)
{
	std::cout << "(pid: " << pid << ") Setting state to " << state << std::endl;
	this->state = state;
}

Task Process::currentTask()
{
	return taskList.front();
}

void Process::tick()
{
	if (type == ProcessType::IDLE) return;

	taskList.front().tick();
	if (taskList.front().getRuntime() == 0) {
		std::cout << "(pid: " << pid << ") " << taskList.front().getType() <<  " Task Completed." <<  std::endl;
		taskList.pop();
		if (taskList.empty())
			setState(State::TERMINATED);
		else
			setState(State::READY);
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
