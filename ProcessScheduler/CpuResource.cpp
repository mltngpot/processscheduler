#include "CpuResource.h"
#include "Process.h"
#include <iostream>

CpuResource::CpuResource()
{
}

CpuResource::~CpuResource()
{
}

void CpuResource::enqueue(Process* process)
{
	if (process->getType() == ProcessType::INTERACTIVE)
	{
		this->interactiveCpuQueue.push(process);
	}
	else
	{
		this->realTimeCpuQueue.push(process);
		if (currentInteractive != nullptr && currentInteractive->getStart() == State::RUNNING)
			currentInteractive->setState(State::READY);
	}

	if (currentRealtime == nullptr || currentInteractive == nullptr)
		updateCurrentProcess(0);
}

bool CpuResource::isBusy()
{
	return currentRealtime != nullptr ||
		   currentInteractive != nullptr||
		   !this->interactiveCpuQueue.empty() ||
		   !this->realTimeCpuQueue.empty();
}

Process* CpuResource::tick(int time)
{
	Process* process = currentProcess();
	if (process == nullptr) {
		SchedulerStatistics::cpuIdle();
		return process;
	}
	process->tick();

	Process* returnProcess = nullptr;
	if (process->getState() == State::READY)
	{
		if (process->getType() == ProcessType::REALTIME)
		{
			currentRealtime = nullptr;
		}
		else
		{
			currentInteractive = nullptr;
		}
		returnProcess = process;
	}
	printStatus();

	return process;
}

void CpuResource::printStatus()
{
	std::cout << "currentRealtime: ";
	if (currentRealtime != nullptr)
		std::cout << "(" << currentRealtime->getPid() << ")";
	else
		std::cout << "IDLE";
	std::cout << "\tcurrentInteractive: ";
	if (currentInteractive != nullptr)
		std::cout << "(" << currentInteractive->getPid() << ")";
	else
		std::cout << "IDLE";
	std::cout << std::endl;
	if (currentProcess() != nullptr)
		std::cout << "returning " << currentProcess()->toString() << std::endl;
}

Process* CpuResource::currentProcess()
{
	Process* process = nullptr;
	if (currentRealtime != nullptr)
		process = currentRealtime;
	else if (currentInteractive != nullptr)
		process = currentInteractive;
	else if (!realTimeCpuQueue.empty() || !interactiveCpuQueue.empty()) {
		updateCurrentProcess(0);
		process = currentProcess();
	}
	return process;
}

void CpuResource::updateCurrentProcess(int time)
{
	// handle realtime processess first
	if (!realTimeCpuQueue.empty() || currentRealtime != nullptr)
	{
		if (!realTimeCpuQueue.empty() && currentRealtime == nullptr) {
			currentRealtime = realTimeCpuQueue.front();
		}

		while (!realTimeCpuQueue.empty() && currentRealtime->getState() != State::RUNNING) {
			currentRealtime = realTimeCpuQueue.front();
			realTimeCpuQueue.pop();
			currentRealtime->setState(State::RUNNING);
			currentRealtime->pastDeadline(time);
		}

		if (currentRealtime->getState() != State::RUNNING)
		{
			currentRealtime = nullptr;
		}
	}

	// It could be Idle now so lets look at Interactives
	if (currentRealtime == nullptr)
	{
		if (currentInteractive != nullptr)
		{
			currentInteractive->setState(State::RUNNING);
		}
		else if (!interactiveCpuQueue.empty()) {
			currentInteractive = interactiveCpuQueue.front();
			interactiveCpuQueue.pop();
			currentInteractive->setState(State::RUNNING);
		}

		if (currentInteractive->getState() != State::RUNNING)
		{
			currentInteractive = nullptr;
		}
	}
	else if (currentInteractive != nullptr)
		currentInteractive->setState(State::READY);

}