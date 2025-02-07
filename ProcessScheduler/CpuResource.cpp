#include "CpuResource.h"
#include "Process.h"

CpuResource::CpuResource()
{
}

CpuResource::~CpuResource()
{
}

void CpuResource::enqueue(Process process)
{
	if (process.getType() == ProcessType::INTERACTIVE)
	{
		if (interactiveCpuQueue.empty() && realTimeCpuQueue.empty())
		{
			process.setState(State::RUNNING);
		}
		this->interactiveCpuQueue.push(process);
	}
	else
	{
		if (realTimeCpuQueue.empty()) {
			if (interactiveCpuQueue.empty())
			{
				interactiveCpuQueue.front().setState(State::READY);
			}
			process.setState(State::RUNNING);
		}
		this->realTimeCpuQueue.push(process);
	}
}

bool CpuResource::isBusy()
{
	return !this->interactiveCpuQueue.empty() && !this->realTimeCpuQueue.empty();
}

Process CpuResource::tick(int time)
{
	Process process = Process::IDLE;
	if (this->realTimeCpuQueue.empty())
	{
		if (!this->interactiveCpuQueue.empty())
			process = this->interactiveCpuQueue.front();
	}
	else 
	{
		process = this->realTimeCpuQueue.front();
	}
	process.tick();
	if (process.getState() == State::READY && process.getType() != ProcessType::IDLE)
	{
		if (process.getType() == ProcessType::INTERACTIVE)
			this->interactiveCpuQueue.pop();
		else
			this->realTimeCpuQueue.pop();

		if (!this->realTimeCpuQueue.empty())
			this->realTimeCpuQueue.front().setState(State::RUNNING);
		else if (!this->interactiveCpuQueue.empty())
			this->interactiveCpuQueue.front().setState(State::RUNNING);
	}
	return process;
}
