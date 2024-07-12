#include "FunctionHook.h"
#include <Windows.h>

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

	bool FunctionHook::FunctionBlock(void* address)
	{
		DWORD oldProtect;
		if (!VirtualProtect(address, 1, PAGE_EXECUTE_READWRITE, &oldProtect))
		{
			return false;
		}

		*static_cast<unsigned char*>(address) = static_cast<unsigned char>(0xE3);

		if (!VirtualProtect(address, 1, oldProtect, &oldProtect))
		{
			return false;
		}

		return true;
	}













}