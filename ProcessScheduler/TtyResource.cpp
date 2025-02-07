#include "TtyResource.h"
#include "Resource.h"

TtyResource::TtyResource()
{
}

TtyResource::~TtyResource()
{
}

void TtyResource::enqueue(Process process)
{
	if (this->ttyQueue.empty())
	{
		process.setState(State::WAITING);
	}
	ttyQueue.push(process);
}

bool TtyResource::isBusy()
{
    return !this->ttyQueue.empty();
}

Process TtyResource::tick(int time)
{
	Process process = this->ttyQueue.front();
    process.tick();
	if (process.getState() == State::READY)
	{
		this->ttyQueue.pop();
		if (!this->ttyQueue.empty())
			this->ttyQueue.front().setState(State::WAITING);
	}
    return process;
}
