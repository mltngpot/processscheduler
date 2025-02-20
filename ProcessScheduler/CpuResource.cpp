#include "CpuResource.h"
#include "Process.h"
#include <iostream>

CpuResource::CpuResource() :
	currentRealtime(ProcessType::IDLE, 0),
	currentInteractive(ProcessType::IDLE, 0)
{
}

CpuResource::~CpuResource()
{
}

void CpuResource::enqueue(Process process)
{
	if (process.getType() == ProcessType::INTERACTIVE)
	{
		this->interactiveCpuQueue.push(process);
	}
	else
	{
		this->realTimeCpuQueue.push(process);
		if (currentInteractive.getType() != ProcessType::IDLE && currentInteractive.getStart() == State::RUNNING)
			currentInteractive.setState(State::READY);
	}

	if (currentRealtime.getType() == ProcessType::IDLE || currentInteractive.getType() == ProcessType::IDLE)
		updateCurrentProcess(0);
}

bool CpuResource::isBusy()
{
	return !this->interactiveCpuQueue.empty() ||
		!this->realTimeCpuQueue.empty() ||
		currentRealtime.getType() != ProcessType::IDLE ||
		currentInteractive.getType() != ProcessType::IDLE;
}

Process CpuResource::tick(int time)
{
	Process process = currentProcess();
	if (process.getType() == ProcessType::IDLE) {
		SchedulerStatistics::cpuIdle();
		return process;
	}
	process.tick();
	if (process.getState() == State::READY || process.getState() == State::TERMINATED) // Process is done running
	{
		updateCurrentProcess(time);
	}

	printStatus();

	return process;
}

void CpuResource::printStatus()
{
	std::cout << "currentRealtime: ";
	if (currentRealtime.getType() != ProcessType::IDLE)
		std::cout << "(" << currentRealtime.getPid() << ")";
	else
		std::cout << "IDLE";
	std::cout << "\tcurrentInteractive: ";
	if (currentInteractive.getType() != ProcessType::IDLE)
		std::cout << "(" << currentInteractive.getPid() << ")";
	else
		std::cout << "IDLE";
	std::cout << std::endl;
	std::cout << "returning " << currentProcess().toString() << std::endl;
}

Process CpuResource::currentProcess()
{
	Process process = Process::IDLE;
	if (currentRealtime.getType() != ProcessType::IDLE)
		process = currentRealtime;
	else if (currentInteractive.getType() != ProcessType::IDLE)
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
	if (!realTimeCpuQueue.empty() || currentRealtime.getType() != ProcessType::IDLE)
	{
		while (!realTimeCpuQueue.empty() && currentRealtime.getState() != State::RUNNING) {
			currentRealtime = realTimeCpuQueue.front();
			realTimeCpuQueue.pop();
			currentRealtime.setState(State::RUNNING);
			currentRealtime.pastDeadline(time);
		}

		if (currentRealtime.getState() != State::RUNNING)
		{
			currentRealtime = Process::IDLE;
		}
	}

	// It could be Idle now so lets look at Interactives
	if (currentRealtime.getType() == ProcessType::IDLE)
	{
		if (currentInteractive.getType() != ProcessType::IDLE)
		{
			currentInteractive.setState(State::RUNNING);
		}
		else if (!interactiveCpuQueue.empty()) {
			currentInteractive = interactiveCpuQueue.front();
			interactiveCpuQueue.pop();
			currentInteractive.setState(State::RUNNING);
		}

		if (currentInteractive.getState() != State::RUNNING)
		{
			currentInteractive = Process::IDLE;
		}
	}
	else if (currentInteractive.getType() != ProcessType::IDLE)
		currentInteractive.setState(State::READY);

}