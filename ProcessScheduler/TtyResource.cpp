#include "TtyResource.h"
#include "Process.h"
#include <iostream>

TtyResource::TtyResource()
{
}

TtyResource::~TtyResource()
{
}

void TtyResource::enqueue(Process* process)
{
	if (this->ttyQueue.empty())
	{
		process->setState(State::WAITING);
	}
	ttyQueue.push(process);
}

bool TtyResource::isBusy()
{
    return !this->ttyQueue.empty() ||
		currentProcess != nullptr;
}

Process* TtyResource::tick(int time)
{
	if (currentProcess == nullptr && !ttyQueue.empty()) {
		currentProcess = ttyQueue.front();
		ttyQueue.pop();
	}
	if (currentProcess == nullptr)
		return nullptr;

	currentProcess->tick();
	Process* returnProcess = nullptr;
	if (currentProcess->getState() == State::READY)
	{
		returnProcess = currentProcess;
		if (!this->ttyQueue.empty()) {
			currentProcess = this->ttyQueue.front();
			currentProcess->setState(State::WAITING);
		}
		else
			currentProcess = nullptr;
	}

	printStatus();

    return returnProcess;
}

void TtyResource::printStatus()
{
	std::cout << "TTY: ";
	if (currentProcess != nullptr)
		std::cout << "(" << currentProcess->getPid() << ")" << std::endl;
	else
		std::cout << "IDLE" << std::endl;
	if (currentProcess != nullptr)
		std::cout << "returning " << currentProcess->toString() << std::endl << std::endl;
}
