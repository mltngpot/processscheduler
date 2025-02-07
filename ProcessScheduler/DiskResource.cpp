#include "DiskResource.h"
#include "Process.h"
#include <iostream>

DiskResource::DiskResource()
{
}

DiskResource::~DiskResource()
{
}

void DiskResource::enqueue(Process* process)
{
	if (this->diskQueue.empty())
		process->setState(State::RUNNING);
	else
		process->setState(State::WAITING);

	this->diskQueue.push(process);
	queuedProcesses++;

	SchedulerStatistics::newDiskAccess();
}

bool DiskResource::isBusy()
{
    return !diskQueue.empty() ||
		currentProcess != nullptr;
}

Process* DiskResource::tick(int time)
{
	if (currentProcess == nullptr && !diskQueue.empty()) {
		currentProcess = diskQueue.front();
		diskQueue.pop();
	}
	if (currentProcess == nullptr)
		return nullptr;

	currentProcess->tick();

	SchedulerStatistics::diskAccess(queuedProcesses);

	Process* returnProcess = nullptr;
	if (currentProcess->getState() == State::READY) {
		returnProcess = currentProcess;
		if (!this->diskQueue.empty())
		{
			currentProcess = this->diskQueue.front();
			currentProcess->setState(State::RUNNING);
			this->diskQueue.pop();
		}
		else
			currentProcess = nullptr;
		queuedProcesses--;
	}

	printStatus();

    return returnProcess;
}

void DiskResource::printStatus()
{
	std::cout << "Disk ("<< queuedProcesses << "): ";
	if (currentProcess != nullptr)
		std::cout << "(" << currentProcess->getPid() << ")" << std::endl;
	else
		std::cout << "IDLE" << std::endl;
	if (currentProcess != nullptr)
		std::cout << "returning " << currentProcess->toString() << std::endl;
}
