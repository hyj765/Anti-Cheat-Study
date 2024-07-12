#include "ThreadManager.h"
#include <functional>
#include <vector>

namespace HYJ
{

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
	std::pair<HANDLE, DWORD> Task<Function, Args...>::runThread()
	{
		
		DWORD threadId = 0;
		HANDLE hThread = CreateThread(nullptr, 0, ThreadProc, static_cast<void*>(this), 0, &threadId);

		return std::make_pair(hThread, threadId);
	}

	template <typename Function, typename... Args>
	DWORD WINAPI Task<Function, Args...>::ThreadProc(LPVOID lpParameter)
	{
		Task* self = static_cast<Task*>(lpParameter);
		std::apply(self->f, self->arguments);

		ThreadManager::getInstance().NotificationThreadExit(GetCurrentThreadId());
		return 0;
	}

}