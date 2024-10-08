#include"ProcessProtector.h"
#include "AntiDebugger.h"
#include "FunctionHook.h"
#include <TlHelp32.h>
#include "ThreadManager.h"
#include "AntiDllInjector.h"
#include "AntiMacro.h"
#include "PEParser.h"
#include "IntegrityChecker.h"

namespace HYJ
{
	unsigned char ProcessProtector::originalThreadThunkBytes[12] = { 0 };

	WinAPITypeList::BaseThreadInitThunkType ProcessProtector::functionAddress;

	ProcessProtector::ProcessProtector() : mainThreadId(std::this_thread::get_id())
		,peparser(std::make_shared<PEParser>())
		,antiDllInjector(std::make_unique<DllInjectionChecker>(peparser))
		,antiDebugger(std::make_unique<AntiDebugger>())
		,integrityChecker(std::make_unique<IntegrityChecker>(peparser))
		,antiMacro(std::make_unique<AntiMacro>())
	{
		InitializeCriticalSection(&criticalSection);
	};
	
	ProcessProtector::~ProcessProtector() 
	{
		DeleteCriticalSection(&criticalSection);
	};

	bool ProcessProtector::HookLoadLibrary(void* functionAddress)
	{
		std::thread::id threadId = std::this_thread::get_id();
		
		if (threadId != mainThreadId && ThreadManager::GetInstance().IsThreadCreateByThreadManager(threadId) == false)
		{
			return false;
		}

		HMODULE kernel32Handle =LoadLibraryA("kernel32.dll");
		if (kernel32Handle == NULL)
		{
			return false;
		}

		void* address = GetProcAddress(kernel32Handle, "LoadLibraryA");
		
		FunctionHook::SetHook(address,functionAddress,DllInjectionChecker::LoadLibraryA_originalCode);
	
		return true;
	}

	bool ProcessProtector::BlockFunction(void* address) 
	{
		bool status = false;
		EnterCriticalSection(&criticalSection);
		
		threadNumber = GetCurrentThreadId();

		if ( FunctionHook::FunctionBlock(address, threadNumber))
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
		
		if (FunctionHook::FunctionUnBlock(address, threadNumber))
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
		if (kernel32 == NULL)
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

	bool ProcessProtector::MultiClientCheck() 
	{

		wchar_t buffer[MAX_PATH];
		if (GetModuleFileName(NULL, buffer, MAX_PATH) == 0)
		{
			return false;
		}


		int count = 0;
		HANDLE snapShot = NULL;
		snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapShot == INVALID_HANDLE_VALUE)
		{
			DEBUG_LOG("ProcessProtector", "Fail to Get Processsnapshot");
			return false;
		}
		
		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(pe);
		if (Process32First(snapShot, &pe) == false)
		{
			DEBUG_LOG("ProcessProtector", "Fail to Get ProcessStructFirst");
			CloseHandle(snapShot);
			return false;
		}

		do {
			
			if (wcscmp(pe.szExeFile, buffer) == 0)
			{
				count++;
				if (count >= 2)
				{
					CloseHandle(snapShot);

					return true;
				}
			}

		} while (Process32Next(snapShot,&pe));

		CloseHandle(snapShot);

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

	bool ProcessProtector::CheckHardWareBreakPoint() noexcept
	{
		return antiDebugger.get()->CheckHardWareCheckPoint();
	}

	bool ProcessProtector::DebuggerDetach() noexcept
	{
		return antiDebugger.get()->BlockDebuggerAttach();
	}

	bool ProcessProtector::IsProcessCreateByDebugger() noexcept
	{	
		return antiDebugger.get()->CheckParentIsDebugger();
	}

	bool ProcessProtector::CheckIsDebuggerAttach() noexcept
	{
		if (antiDebugger.get()->IsProcessDebugged() || antiDebugger.get()->CheckDebugObjectHandle() || antiDebugger.get()->CheckDebugPort() || antiDebugger.get()->CheckDebugFlags() || antiDebugger.get()->CheckProcessNtGlobalFlags())
		{
			return true;
		}

		return false;
	}

	bool ProcessProtector::BlockBreakPoint() noexcept
	{
		EnterCriticalSection(&criticalSection);
		if (!antiDebugger.get()->BlockDbgBreakPoint())
		{
			LeaveCriticalSection(&criticalSection);
			return false;
		}

		LeaveCriticalSection(&criticalSection);

		return true;
	}
}