#include"ProcessProtector.h"
#include "AntiDebugger.h"
#include "FunctionHook.h"
#include <TlHelp32.h>
#include "ThreadManager.h"
#include "AntiDllInjector.h"

namespace HYJ
{
	unsigned char ProcessProtector::originalThreadThunkBytes[12] = { 0 };
	WinAPITypeList::BaseThreadInitThunkType ProcessProtector::functionAddress;

	ProcessProtector::ProcessProtector() : mainThreadId(GetCurrentThreadId()), hookManager(std::make_unique<FunctionHook>()), antiDebugger(std::make_unique<AntiDebugger>())
	{
		InitializeCriticalSection(&criticalSection);
	};
	
	ProcessProtector::~ProcessProtector() 
	{
		DeleteCriticalSection(&criticalSection);
	};

	bool ProcessProtector::HookLoadLibrary(void* functionAddress)
	{
		DWORD threadId = GetCurrentThreadId();
		
		if (threadId != mainThreadId && ThreadManager::getInstance().IsThreadCreateByThreadManager(threadId) == false)
		{
			return false;
		}

		HMODULE kernel32Handle =LoadLibraryA("kernel32.dll");
		void* address = GetProcAddress(kernel32Handle, "LoadLibraryA");
		
		FunctionHook::SetHook(address,functionAddress,DllInjectionChecker::LoadLibraryA_originalCode);
	
		return true;
	}

	bool ProcessProtector::BlockFunction(void* address) 
	{
		bool status = false;
		EnterCriticalSection(&criticalSection);
		
		threadNumber = GetCurrentThreadId();

		if ( hookManager->FunctionBlock(address, threadNumber))
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
		
		if (hookManager->FunctionUnBlock(address, threadNumber))
		{
			status = true;
		}

		threadNumber = 0;
		LeaveCriticalSection(&criticalSection);

		return status;
	}

	
	void __fastcall ProcessProtector::RegistThreadFilterFunction(DWORD LdrReserved, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter)
	{
		MEMORY_BASIC_INFORMATION memInfo;
		if (VirtualQuery(lpStartAddress, &memInfo, sizeof(memInfo)))
		{
			if (memInfo.Type == MEM_PRIVATE) //This means that another process has allocated memory to my process through a function such as virtualalloc.
			{
				printf("Invalid Memory Access Detected\n");
				return;
			}
		}



		FunctionHook::UnHook(functionAddress,originalThreadThunkBytes);
		functionAddress(LdrReserved, lpStartAddress, lpParameter);
		FunctionHook::SetHook(functionAddress,RegistThreadFilterFunction);
		
	}

	bool ProcessProtector::AntiDllinjection()
	{
		HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
		if (kernel32 == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		functionAddress = reinterpret_cast<WinAPITypeList::BaseThreadInitThunkType>(GetProcAddress(kernel32, "BaseThreadInitThunk"));
		if (functionAddress == 0)
		{
			return false;
		}

		if (FunctionHook::SetHook(functionAddress, RegistThreadFilterFunction, originalThreadThunkBytes) == false)
		{
			return false;
		}

		return true;
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

		return false;
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