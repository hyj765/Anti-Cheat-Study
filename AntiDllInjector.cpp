#include "AntiDllInjector.h"
#include <psapi.h>

namespace HYJ
{

	bool DllInjectionChecker::MemoryInfoMationCheck(LPTHREAD_START_ROUTINE lpStartAddress)
	{
		MEMORY_BASIC_INFORMATION memInfo;

		if (!VirtualQuery(lpStartAddress, &memInfo, sizeof(memInfo)))
		{
			return false;
		}

		if (memInfo.Type == MEM_PRIVATE)
		{
			return false;
		}

		return true;
	}

	bool DllInjectionChecker::ImportAddressTableCheck(const char* functionName, const char* moduleName)
	{
		


		return true;
	}



}