#include "DiskResource.h"
#include "Process.h"

DiskResource::DiskResource()
{
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
}

bool DiskResource::isBusy()
{
    return !this->diskQueue.empty();
}

Process DiskResource::tick(int time)
{
	Process process = diskQueue.front();
	process.tick();
	if (process.getState() == State::READY) {
		diskQueue.pop();
		if (!diskQueue.empty())
			diskQueue.front().setState(State::RUNNING);
	}
    return process;
}
