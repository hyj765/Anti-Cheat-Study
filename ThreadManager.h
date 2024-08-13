#pragma once
#include"stdafx.h"
#include<mutex>
#include<atomic>


namespace HYJ
{
	class ThreadManager;

	template <typename Function, typename... Args>
	class Task
	{

	public:
		
		Task(Function fn, Args... s) : f(fn), arguments(std::forward<Args>(s)...) {};
		
		void operator()() const
		{
			std::apply(f, arguments);
		}
	private:
		Function f;
		std::tuple<Args...> arguments;
	};


	class ThreadManager
	{
	public:
		static ThreadManager& GetInstance() noexcept
		{
			static ThreadManager instance;
			return instance;
		}

		template <typename Function, typename... Args>
		std::thread::id CreateThreads(Function function,Args... args);

		bool IsThreadCreateByThreadManager(std::thread::id threadId);
		
		inline bool IsThreadJoinAble(std::thread::id threadId)
		{
			for (int i = 0; i < tasks.size(); ++i)
			{
				if (tasks[i].get()->get_id() == threadId)
				{
					if (tasks[i].get()->joinable())
					{
						return true;
					}

					break;
				}
			}

			return false;
		}

		inline void PushTaskList(std::unique_ptr<std::thread> Thread) noexcept
		{
			std::lock_guard lock(mtx);
			tasks.push_back(move(Thread));
		}
		
		inline void RemoveFinishedThreads()
		{
			
			std::lock_guard lock(mtx);

			auto it = std::remove_if(tasks.begin(), tasks.end(),
				[](const std::unique_ptr<std::thread>& th) {
					return th->joinable() == false; 
				});
			tasks.erase(it, tasks.end());

		}
		
		void ThreadNotification(std::thread::id threadId);
		
	private:

		std::mutex mtx;
		std::vector<std::unique_ptr<std::thread>> tasks;
		
		ThreadManager() = default;
		
		~ThreadManager();
		
		ThreadManager(const ThreadManager&) = delete;
		
		ThreadManager(ThreadManager&&) = delete;
		
		ThreadManager& operator=(ThreadManager&&) = delete;
	};

	template <typename Function, typename... Args>
	std::thread::id ThreadManager::CreateThreads(Function function, Args... args)
	{
		
		std::unique_ptr<std::thread> th = std::make_unique<std::thread>(Task(function, std::forward<Args>(args)...));
		std::thread::id threadId = th.get()->get_id();
		
		PushTaskList(std::move(th));

		return threadId;
	}
	
}