#include "ProcessScheduler.h"
#include "Task.h"
#include <iostream>
#include <queue>
#include <string>
#include "Process.h"
#include <stdexcept>
#include "SchedulerStatistics.h"
#include <sstream>

using std::cout;
using std::endl;
using std::runtime_error;
using std::queue;

int main()
{
	ProcessScheduler ps = ProcessScheduler();
	ps.run();
	ps.printStats();
	return 0;
}


ProcessScheduler::ProcessScheduler() {
	try {
		currentPid = 1;
		readInput(std::cin);
	}
	catch (runtime_error ex)
	{
		cout << "INPUT EXCEPTION: " << endl;
		cout << ex.what() << endl;
		cout << "Exiting program." << endl;
		exit(-1);
	}
}

void ProcessScheduler::run() {
	for (time = 0; stillProcessing(); time++) {
		cout << endl << endl << "TICK: " << time << endl;
		try {
			dequeueInput();
		}
		catch (runtime_error ex)
		{
			cout << "DEQUEUE EXCEPTION: " << endl;
			cout << ex.what() << endl;
			cout << "Exiting program." << endl;
			exit(-1);
		}
		tick();
		if (time % 100 == 0)
			clearDeletes();
	}
}

void ProcessScheduler::dequeueInput()
{
	while (!inputProcessesQueue.empty() && inputProcessesQueue.front()->getStart() == time) {
		cout << "Dequeueing process at time " << time << endl;
		Process* process = inputProcessesQueue.front();
		inputProcessesQueue.pop();
		process->setState(State::READY);
		process->setPid(currentPid++);
		queueProcess(process);
	}
}

void ProcessScheduler::queueProcess(Process* process)
{
	if (process == nullptr ||
		process->getState() == State::RUNNING)
		return;

	if (process->getCurrentTask() == nullptr) {
		if (process->getType() == ProcessType::REALTIME)
			if (process->pastDeadline(time))
				SchedulerStatistics::realTimeProcessMissedDeadline();
			else
				SchedulerStatistics::realTimeProcessCompleted();
		else
			SchedulerStatistics::interactiveProcessCompleted();

		cout << "Deleting process (" << process->getPid() << ")" << endl;
		deleteQueue.push(process);
		return;
	}
		
	currentPid++;

	switch (process->getCurrentTask()->getType()) {
	case TaskType::CPU:
		cout << "Enqueueing (CPU) process (" << process->getPid() << ")" << endl;
		cpuResource.enqueue(process);
		break;
	case TaskType::DISK:
		cout << "Enqueueing (DISK) process (" << process->getPid() << ")" << endl;
		diskResource.enqueue(process);
		break;
	case TaskType::TTY:
		cout << "Enqueueing (TTY) process (" << process->getPid() << ")" << endl;
		ttyResource.enqueue(process);
		break;
	default:
		break;
	}
}

void ProcessScheduler::tick()
{
	queueProcess(cpuResource.tick(time));
	queueProcess(diskResource.tick(time));
	queueProcess(ttyResource.tick(time));
}

bool ProcessScheduler::stillProcessing() {
	return !inputProcessesQueue.empty() || cpuResource.isBusy() || diskResource.isBusy() || ttyResource.isBusy();
}

void ProcessScheduler::readInput(std::istream& in) {
	while (in.good()) {
		std::string identifier;
		int time;
		in >> identifier >> time;
		switch (identifier[0]) {
		case 'I':
			if (identifier != "INTERACTIVE") throw runtime_error("Unexpected identifier: " + identifier);
			inputProcessesQueue.push(new Process(ProcessType::INTERACTIVE, time));
			break;
		case 'R':
			if (identifier != "REAL-TIME") throw runtime_error("Unexpected identifier: " + identifier);
			int deadline;
			in >> identifier >> deadline;
			if (identifier != "DEADLINE") throw runtime_error("Unexpected identifier: " + identifier);
			inputProcessesQueue.push(new Process(ProcessType::REALTIME, time, deadline));
			break;
		case 'C':
			if (identifier != "CPU" || inputProcessesQueue.size() == 0) throw runtime_error("Unexpected identifier: " + identifier);
			inputProcessesQueue.back()->addTask(new Task(TaskType::CPU, time));
			break;
		case 'T':
			if (identifier != "TTY" || inputProcessesQueue.size() == 0) throw runtime_error("Unexpected identifier: " + identifier);
			inputProcessesQueue.back()->addTask(new Task(TaskType::TTY, time));
			break;
		case 'D':
			if (identifier != "DISK" || inputProcessesQueue.size() == 0) throw runtime_error("Unexpected identifier: " + identifier);
			inputProcessesQueue.back()->addTask(new Task(TaskType::DISK, time));
			break;
		default:
			throw runtime_error("Unexpected identifier: " + identifier);
			break;
		}
	}
	return;
}

void ProcessScheduler::printStats() {
	cout << this->statistics.toString() << endl;
	return;
}

void ProcessScheduler::clearDeletes()
{
	while (!deleteQueue.empty()) {
		Process* process = deleteQueue.front();
		deleteQueue.pop();
		delete process;
	}
}

