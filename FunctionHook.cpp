#pragma once
#include "FunctionHook.h"

namespace HYJ
{

	bool FunctionHook::SetHook(void* targetFunctionAddress, void* hookFunctionAddress, unsigned char* originalBuff) 
	{
		unsigned char HookCode[12] = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0 };
		
		DWORD oldProtect;
		
		if (!VirtualProtect(targetFunctionAddress, 12, PAGE_EXECUTE_READWRITE, &oldProtect))
		{
			return false;
		}
		
		if (originalBuff != nullptr)
		{
			memcpy(targetFunctionAddress, originalBuff, 12);
		}

	    memcpy((HookCode + 2), &hookFunctionAddress, sizeof(DWORD64));

		memcpy(targetFunctionAddress, HookCode, 12);


		if (!VirtualProtect(targetFunctionAddress, 12, oldProtect, &oldProtect))
		{
			return false;
		}

		return true;
	}

	bool FunctionHook::UnHook(void* targetFunctionAddress, unsigned char* originalBytes) 
	{
		DWORD oldProtect;
		if (!VirtualProtect(targetFunctionAddress, 12, PAGE_EXECUTE_READWRITE, &oldProtect))
		{
			return false;
		}
		
		memcpy(targetFunctionAddress, originalBytes, 12);

		if (VirtualProtect(targetFunctionAddress, 12, oldProtect, &oldProtect))
		{
			return false;
		}

		return true;
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