#include"ProcessProtector.h"
#include "Anti Debugger.h"
#include "FunctionHook.h"
#include <TlHelp32.h>

namespace HYJ
{

	ProcessProtector::ProcessProtector() {};
	
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

	bool ProcessProtector::RegistThreadFilterFunction(void* address) noexcept
	{

		HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
		void* functionAddress = GetProcAddress(kernel32, "BaseThreadInitThunk");
		
		hookmanager->SetHook(functionAddress, address);

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



	}

}