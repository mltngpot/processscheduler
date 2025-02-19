#include "DiskResource.h"
#include "Process.h"

DiskResource::DiskResource(SchedulerStatistics statistics)
{
	this->statistics = statistics;
}

DiskResource::~DiskResource()
{
}

void DiskResource::enqueue(Process process)
{
	if (this->diskQueue.empty())
	{
		process.setState(State::RUNNING);
	}
	this->diskQueue.push(process);
	statistics.newDiskAccess();
}

bool DiskResource::isBusy()
{
    return !diskQueue.empty();
}

Process DiskResource::tick(int time)
{
	Process process = Process::IDLE;
	if (!this->diskQueue.empty()) {
		process = this->diskQueue.front();
		statistics.diskAccess(this->diskQueue.size());
	}
	else {
		this->statistics.diskIdle();
		return process;
	}

	process.tick();
	if (process.getState() == State::READY) {
		this->diskQueue.pop();
		if (!this->diskQueue.empty())
			this->diskQueue.front().setState(State::RUNNING);
	}
    return process;
}
