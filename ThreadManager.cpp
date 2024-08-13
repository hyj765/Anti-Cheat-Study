#include "ThreadManager.h"


namespace HYJ
{

	

	ThreadManager::~ThreadManager()
	{
	
		for (int i = 0; i < tasks.size(); ++i)
		{
			tasks[i].get()->join();
		}

		tasks.clear();

	}
	
	bool ThreadManager::IsThreadCreateByThreadManager(std::thread::id threadId)
	{
		if (tasks.size() == 0)
		{
			return false;
		}
		
		for (int i = 0; i < tasks.size(); ++i)
		{
			if (threadId == tasks[i].get()->get_id())
			{
				return true;
			}
		}

		return false;
	}

	void ThreadManager::ThreadNotification(std::thread::id threadId)
	{
		//EraseTask(threadId);
	}



}
