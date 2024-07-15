#include "FunctionHook.h"
#include <Windows.h>
#include <map>

namespace HYJ
{

	void FunctionHook::SetHook(void* targetFunctionAddress, void* hookFunctionAddress)
	{
		unsigned char HookCode[12] = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0 };
		
		DWORD oldProtect;
		
		VirtualProtect(targetFunctionAddress, 12, PAGE_EXECUTE_READWRITE, &oldProtect);
		
	    memcpy((HookCode + 2), &hookFunctionAddress, sizeof(DWORD64));

		memcpy(targetFunctionAddress, HookCode, 12);

		VirtualProtect(targetFunctionAddress, 12, oldProtect, &oldProtect);
	}

	bool FunctionHook::FunctionBlock(void* address, int threadNumber)
	{
		DWORD oldProtect;

		if (threadNumber != GetCurrentThreadId())
		{
			return false;
		}

	
		if (!VirtualProtect(address, 1, PAGE_EXECUTE_READWRITE, &oldProtect))
		{
			return false;
		}
		unsigned char originalBytes;
		memcpy(&originalBytes, address, 1);
		originalCode[address] = originalBytes;
		*static_cast<unsigned char*>(address) = static_cast<unsigned char>(0xE3);

		if (!VirtualProtect(address, 1, oldProtect, &oldProtect))
		{
			return false;
		}

		return true;
	}

	bool FunctionHook::FunctionUnBlock(void* address, int threadNumber)
	{
		DWORD oldProtect;

		if (threadNumber != GetCurrentThreadId())
		{
			return false;
		}


		if (!VirtualProtect(address, 1, PAGE_EXECUTE_READWRITE, &oldProtect))
		{
			return false;
		}
	
		unsigned char originalBytes = originalCode[address];
		memcpy(address, &originalBytes, 1);

		if (!VirtualProtect(address, 1, oldProtect, &oldProtect))
		{
			return false;
		}

		return true;
	}













}