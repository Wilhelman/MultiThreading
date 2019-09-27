#include "ModuleTaskManager.h"


void ModuleTaskManager::threadMain()
{
	while (true)
	{
		// TODO 3:
		// - Wait for new tasks to arrive
		// - Retrieve a task from scheduledTasks
		// - Execute it
		// - Insert it into finishedTasks

		

		Task* tmp_task = nullptr;
		
		{
			std::unique_lock<std::mutex> lock(mtx); // Lock the mutex

			if(scheduledTasks.empty())
				event.wait(lock);

			tmp_task = scheduledTasks.front();
			scheduledTasks.pop();
		}

		tmp_task->execute();
		finishedTasks.push(tmp_task);
		
	}
}

bool ModuleTaskManager::init()
{
	// TODO 1: Create threads (they have to execute threadMain())
	for (unsigned int i = 0; i < MAX_THREADS; ++i)
	{
		threads[i] = std::thread([this] { this->threadMain(); });
	}
	return true;
}

bool ModuleTaskManager::update()
{
	// TODO 4: Dispatch all finished tasks to their owner module (use Module::onTaskFinished() callback)
	std::unique_lock<std::mutex> lock(mtx); // Lock the mutex
	for (unsigned int i = 0u; i < finishedTasks.size(); i++)
	{
		Task* tmp_task = finishedTasks.front();
		tmp_task->owner->onTaskFinished(tmp_task);
		finishedTasks.pop();
	}

	return true;
}

bool ModuleTaskManager::cleanUp()
{
	// TODO 5: Notify all threads to finish and join them
	
	event.notify_all();
	for (unsigned int i = 0u; i < MAX_THREADS; i++)
	{
		threads[i].join();
	}
	return true;
}

void ModuleTaskManager::scheduleTask(Task *task, Module *owner)
{
	task->owner = owner;
	
	// TODO 2: Insert the task into scheduledTasks so it is executed by some thread
	std::unique_lock<std::mutex> lock(mtx); // Lock the mutex
	scheduledTasks.push(task);
	event.notify_one();
}
