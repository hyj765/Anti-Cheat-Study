#include"ProcessProtector.h"
#include "Anti Debugger.h"
#include "FunctionHook.h"
#include <TlHelp32.h>
#include "ThreadManager.h"

namespace HYJ
{

	ProcessProtector::ProcessProtector() : mainThreadId(GetCurrentThreadId()) {};
	
	bool ProcessProtector::HookLoadLibrary(void* functionAddress)
	{
		DWORD threadId = GetCurrentThreadId();
		
		if (threadId != mainThreadId && ThreadManager::getInstance().FindHandleByThreadId(threadId) == false)
		{
			return false;
		}

		HMODULE kernel32Handle =LoadLibraryA("kernel32.dll");
		void* address = GetProcAddress(kernel32Handle, "LoadLibarayA");
	
		hookmanager->SetHook(address, functionAddress);

		return true;
	}

	bool ProcessProtector::BlockFunction(void* address) 
	{
		bool status = false;
		EnterCriticalSection(&criticalSection);
		
		threadNumber = GetCurrentThreadId();

		if ( hookmanager->FunctionBlock(address, threadNumber))
		{
			status = true;
		}
		
		threadNumber = 0;
		
		LeaveCriticalSection(&criticalSection);

		return status;
	}

	bool ProcessProtector::OpenFunction(void* address)
	{
		bool status = false;
		EnterCriticalSection(&criticalSection);
		threadNumber = GetCurrentThreadId();
		
		if (OpenFunction(address))
		{
			status = true;
		}

		threadNumber = 0;
		LeaveCriticalSection(&criticalSection);

		return status;
	}

	
	void __fastcall ProcessProtector::RegistThreadFilterFunction(DWORD LdrReserved, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter)
	{

		

	}

	bool ProcessProtector::AntiDllinjection()
	{
		HMODULE kernel32 = GetModuleHandleA("kernel32.dll");

		BaseThreadInitThunkType functionAddress = reinterpret_cast<BaseThreadInitThunkType>(GetProcAddress(kernel32, "BaseThreadInitThunk"));
		

		//hookmanager->SetHook(functionAddress, address,originalThreadThunkBytes);

	}

	bool ProcessProtector::MutiClientCheck() 
	{

		wchar_t buffer[MAX_PATH];
		if (GetModuleFileName(NULL, buffer, MAX_PATH) == 0)
		{
			return false;
		}



		int count = 0;
		HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapShot == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		
		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(pe);

		if (!Process32First(snapShot, &pe))
		{
			return false;
		}

	}

	bool ProcessProtector::MemoryProtectionDep() noexcept
	{
		
		if (SetProcessDEPPolicy(PROCESS_DEP_ENABLE))
		{
			return true;
		}

		return false;
	}

}