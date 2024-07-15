#include"ProcessProtector.h"
#include "Anti Debugger.h"
#include "FunctionHook.h"

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

}