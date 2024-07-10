#include "Anti Debugger.h"

namespace HYJ
{

	bool AntiDebugger::isProcessDebugged() noexcept
	{
		return IsDebuggerPresent();
	}

	bool AntiDebugger::CheckDebugPort() noexcept
	{
		DWORD dwProcessDebugPort, dwReturned;
		NTSTATUS status = winApis.pNtqueryInfomation(GetCurrentProcess(), ProcessDebugPort, &dwProcessDebugPort, sizeof(DWORD), &dwReturned);
		
		if (status >= 0 )
		{
			if (dwProcessDebugPort == -1)
			{
				return true;
			}
		}

		return false;
	}

	bool AntiDebugger::CheckDebugFlags() noexcept
	{
		DWORD dwProcessFlags, dwReturned;
		NTSTATUS status = winApis.pNtqueryInfomation(GetCurrentProcess(), ProcessDebugFlags, &dwProcessFlags, sizeof(DWORD),&dwReturned);
		if (status >= 0)
		{
			if (dwProcessFlags == 0)
			{
				return true;
			}
		}

		return false;
	}

	bool AntiDebugger::CheckDebugObjectHandle() noexcept
	{
		HANDLE hProcessDebugHandle;
		DWORD dwReturned;
		NTSTATUS status = winApis.pNtqueryInfomation(GetCurrentProcess(), ProcessDebugObjectHandle, &hProcessDebugHandle, sizeof(HANDLE), &dwReturned);
		
		if (status >= 0)
		{
			if (hProcessDebugHandle != NULL)
			{
				return true;
			}
		}

		return false;
	}

	bool AntiDebugger::CheckProcessHeapFlags() noexcept
	{
		void* peb = reinterpret_cast<void*>(__readgsqword(0x60));
		DWORD ntGlobalFlag = *(DWORD*)((unsigned char*)peb + 0x68);
		
		if (ntGlobalFlag == 0x10 || ntGlobalFlag == 0x20 || ntGlobalFlag == 0x40)
		{
			return true;
		}

		return true;
	}


	AntiDebugger::AntiDebugger()
	{
		HMODULE ntModule=GetModuleHandleA("ntdll.dll");
		winModules.hNtDll = ntModule;
		winApis.pNtqueryInfomation = reinterpret_cast<WinAPITypeList::pNtqueryInformationProcess>(GetProcAddress(ntModule, "NtQueryInformationProcess"));

	}










}