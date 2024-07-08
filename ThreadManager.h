#pragma once
#include<Windows.h>
#include<mutex>
#include<map>
#include<tuple>
#include<vector>

namespace HYJ
{
	class ThreadManager;

	template <typename Function, typename... Args>
	class Task
	{
	public:
		Task(Function fn, Args... s) : f(fn), arguments(s...) {};

		std::pair<HANDLE, DWORD> runThread();
		

	private:

		static DWORD WINAPI ThreadProc(LPVOID lpParameter);
		
		Function f;
		std::tuple<Args...> arguments;

	};


	class ThreadManager
	{
	public:
		static ThreadManager& getInstance() noexcept
		{
			static ThreadManager instance;
			return instance;
		}

		template <typename Function, typename... Args>
		DWORD CreateThreads(Function&& func, bool WaitFlag,Args&&... args);
		bool IsThreadCreateByThreadManager(int threadId);
		inline void InsertTaskList(DWORD threadId, HANDLE threadHandle) noexcept { taskLists.emplace_back(std::make_pair(threadId, threadHandle)); }
		void NotificationThreadExit(int threadId) noexcept;
	

		inline HANDLE FindHandleByThreadId(DWORD threadId)
		{


			for (int i = 0; i < taskLists.size(); ++i)
			{
				if (taskLists[i].first == threadId)
				{
					return taskLists[i].second;
				}
			}

			return NULL;

		}

		inline void RemoveHandle(DWORD threadId) noexcept
		{
			if (taskLists.size() == 0)
			{
				return;
			}

			for (int i = 0; i < taskLists.size(); ++i)
			{
				if (taskLists[i].first == threadId)
				{
					taskLists.erase(taskLists.begin() + i);
				}
			}
		}

	private:
		std::mutex mtx;
		std::vector<std::pair<DWORD, HANDLE>> taskLists;
		ThreadManager() = default;
		~ThreadManager();
		ThreadManager(const ThreadManager&) = delete;
		ThreadManager(ThreadManager&&) = delete;
		ThreadManager& operator=(ThreadManager&&) = delete;
	};



	template <typename Function, typename... Args>
	DWORD ThreadManager::CreateThreads(Function&& func, bool WaitFlag ,Args&&... args)
	{
		HANDLE threadHandle = NULL;
		DWORD threadId = 0;

		
		auto task = Task(func, args...);
		std::pair<HANDLE, DWORD> threadInfo = task.runThread();
		if (threadInfo.second == 0 || threadInfo.first == INVALID_HANDLE_VALUE)
		{
			return -1;
		}


		InsertTaskList(threadInfo.second, threadInfo.first);
		
		if (WaitFlag)
		{
			WaitForSingleObject(threadInfo.first, INFINITE);
		}

		return threadInfo.second;
	}

}