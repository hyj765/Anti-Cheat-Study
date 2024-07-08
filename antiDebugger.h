#include<Windows.h>

namespace HYJ
{
	class AntiDebugger
	{
		

		inline static AntiDebugger& getInstance()
		{
			static AntiDebugger instance;
			return instance;
		}

		inline bool IsTargetProcessHandleExist() { return (targetProcessHandle != NULL); }
		
		inline bool GetTargetProcessHandle(int pid) noexcept
		{
			targetProcessHandle= OpenProcess(PROCESS_VM_READ, NULL, pid);
			return IsTargetProcessHandleExist();
		}
		inline bool CheckTargetProcessIsDebugged()
		{
			if (IsTargetProcessHandleExist() == false)
			{
				return false;
			}

			if (CheckRemoteDebuggerPresent(targetProcessHandle, &targetIsDebugged))
			{
				return targetIsDebugged;
			}
			
			return false;
		}
		inline bool CurrentProcessIsDebugged() noexcept
		{
			return IsDebuggerPresent();
		}



	private:
		AntiDebugger() {}
		AntiDebugger(AntiDebugger& ref) = delete;
		AntiDebugger(AntiDebugger&& ref) = delete;
		AntiDebugger& operator=(AntiDebugger&& ref) = delete;
		AntiDebugger& operator=(const AntiDebugger& ref) = delete;
		~AntiDebugger() {}
		BOOL targetIsDebugged = false;
		HANDLE targetProcessHandle = NULL;
	};



}