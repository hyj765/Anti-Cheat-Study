#pragma once
#include"stdafx.h"
#include<mutex>
#include<map>
#include<tuple>

namespace HYJ
{

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
		
		void NotificationThreadExit(int threadId) noexcept;
	
		bool TerminateThreadByHandle(HANDLE hThread);

		bool TerminateThreadByThreadId(DWORD threadId);

		inline void InsertTaskList(DWORD threadId, HANDLE threadHandle) noexcept { taskLists.emplace_back(std::make_pair(threadId, threadHandle)); }

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
		std::vector<DWORD> asyncTaskList;
		
		ThreadManager() = default;
		
		~ThreadManager();
		
		ThreadManager(const ThreadManager&) = delete;
		
		ThreadManager(ThreadManager&&) = delete;
		
		ThreadManager& operator=(ThreadManager&&) = delete;
	};

}