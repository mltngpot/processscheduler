#include "Task.h"
#include <string>
#include <stdexcept>

Task::Task(TaskType type, int runtime)
{
	if (runtime < 0) throw std::runtime_error::runtime_error("Invalid task runtime.");
	this->type = type;
	this->runtime = runtime;
}

Task::~Task()
{
}

TaskType Task::getType()
{
	return type;
}

int Task::getRuntime()
{
	return runtime;
}

void Task::tick()
{
	runtime--;
}
