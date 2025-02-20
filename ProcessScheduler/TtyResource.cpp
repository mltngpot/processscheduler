#include "TtyResource.h"
#include "Process.h"
#include <iostream>

TtyResource::TtyResource() : currentProcess(Process::IDLE)
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
    return !this->ttyQueue.empty() ||
		currentProcess.getType() != ProcessType::IDLE;
}

Process TtyResource::tick(int time)
{
	currentProcess.tick();
	if (currentProcess.getState() == State::READY || currentProcess.getState() == State::TERMINATED)
	{
		if (!this->ttyQueue.empty()) {
			currentProcess = this->ttyQueue.front();
			currentProcess.setState(State::WAITING);
		}
	}

	printStatus();

    return currentProcess;
}

void TtyResource::printStatus()
{
	std::cout << "TTY: ";
	if (currentProcess.getType() != ProcessType::IDLE)
		std::cout << "(" << currentProcess.getPid() << ")" << std::endl;
	else
		std::cout << "IDLE" << std::endl;
	std::cout << "returning " << currentProcess.toString() << std::endl << std::endl;
}
