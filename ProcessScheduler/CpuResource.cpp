#include "CpuResource.h"
#include "Process.h"

CpuResource::CpuResource(SchedulerStatistics statistics) :
	currentRealtime(ProcessType::IDLE, 0),
	currentInteractive(ProcessType::IDLE, 0)
{
	this->statistics = statistics;
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
	}

	if (currentRealtime.getType() == ProcessType::IDLE && currentInteractive.getType() == ProcessType::IDLE)
		updateCurrentProcess(0);
}

bool CpuResource::isBusy()
{
	return !this->interactiveCpuQueue.empty() &&
		!this->realTimeCpuQueue.empty() &&
		currentRealtime.getType() != ProcessType::IDLE &&
		currentInteractive.getType() != ProcessType::IDLE;
}

Process CpuResource::tick(int time)
{
	Process process = currentProcess();
	if (process.getType() == ProcessType::IDLE) {
		statistics.cpuIdle();
		return process;
	}
	process.tick();
	if (process.getState() == State::READY || process.getState() == State::TERMINATED) // Process is done running
	{
		updateCurrentProcess(time);
	}
	return process;
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
	if (currentRealtime.getType() != ProcessType::IDLE)
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

	// It could be Idle now
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
		else
		{
			currentInteractive = Process::IDLE;
		}
	}
	else if (currentInteractive.getType() != ProcessType::IDLE)
		currentInteractive.setState(State::READY);

}