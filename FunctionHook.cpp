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













}