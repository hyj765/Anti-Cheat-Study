#include "ThreadManager.h"
#include <functional>
#include <vector>
#include <thread>
#include <future>
#include <random>

namespace HYJ
{

	template <typename Function, typename... Args>
	std::pair<HANDLE, DWORD> Task<Function, Args...>::runThread()
	{

		DWORD threadId = 0;
		HANDLE hThread = CreateThread(nullptr, 0, ThreadProc, static_cast<void*>(this), 0, &threadId);

		return std::make_pair(hThread, threadId);
	}

	ThreadManager::~ThreadManager()
	{
		std::vector<HANDLE> handles;

		for (std::pair<int,HANDLE> task:taskLists)
		{
			handles.push_back(task.second);
		}

		if (!handles.empty())
		{
			WaitForMultipleObjects(handles.size(), handles.data(), TRUE, INFINITE);
		}

		for (HANDLE handle : handles)
		{
			CloseHandle(handle);
		}
	}

	bool ThreadManager::IsThreadCreateByThreadManager(int threadId)
	{
		if (taskLists.size() == 0)
		{
			return false;
		}
		
		for (int i = 0; i < taskLists.size(); ++i)
		{
			if (threadId == taskLists[i].first)
			{
				return true;
			}

		}

		return false;
	}

	void ThreadManager::NotificationThreadExit(int threadId) noexcept
	{
		std::lock_guard<std::mutex> lock(mtx);
		auto hThread = FindHandleByThreadId(threadId);
		CloseHandle(hThread);
	}


	template <typename Function, typename... Args>
	DWORD WINAPI Task<Function, Args...>::ThreadProc(LPVOID lpParameter)
	{
		Task* self = static_cast<Task*>(lpParameter);
		std::apply(self->f, self->arguments);

		ThreadManager::getInstance().NotificationThreadExit(GetCurrentThreadId());

		return 0;
	}

	template <typename Function, typename... Args>
	DWORD ThreadManager::CreateThreads(Function&& func, bool WaitFlag, Args&&... args)
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

	bool ThreadManager::TerminateThreadByHandle(HANDLE hThread)
	{
		if (hThread == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		return TerminateThread(hThread,0);
	}

	bool ThreadManager::TerminateThreadByThreadId(DWORD threadId)
	{
		HANDLE hThread = NULL;
		if ((hThread = FindHandleByThreadId(threadId)) == NULL)
		{
			return false;
		}
		
		return TerminateThreadByHandle(hThread);
	}

}
