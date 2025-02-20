#include "DiskResource.h"
#include "Process.h"
#include <iostream>

DiskResource::DiskResource() : currentProcess(Process::IDLE)
{
}

DiskResource::~DiskResource()
{
}

void DiskResource::enqueue(Process process)
{
	if (this->diskQueue.empty())
		process.setState(State::RUNNING);
	else
		process.setState(State::WAITING);

	this->diskQueue.push(process);
	queuedProcesses++;

	SchedulerStatistics::newDiskAccess();
}

bool DiskResource::isBusy()
{
    return !diskQueue.empty() ||
		currentProcess.getType() != ProcessType::IDLE;
}

Process DiskResource::tick(int time)
{
	currentProcess.tick();

	SchedulerStatistics::diskAccess(queuedProcesses);

	if (currentProcess.getState() == State::READY || currentProcess.getState() == State::TERMINATED) { 
		if (!this->diskQueue.empty())
		{
			currentProcess = this->diskQueue.front();
			currentProcess.setState(State::RUNNING);
			this->diskQueue.pop();
		}
		queuedProcesses--;
	}

	printStatus();

    return currentProcess;
}

void DiskResource::printStatus()
{
	std::cout << "Disk ("<< queuedProcesses << "): ";
	if (currentProcess.getType() != ProcessType::IDLE)
		std::cout << "(" << currentProcess.getPid() << ")" << std::endl;
	else
		std::cout << "IDLE" << std::endl;
	std::cout << "returning " << currentProcess.toString() << std::endl;
}
