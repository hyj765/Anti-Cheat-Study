#include "AntiDllInjector.h"
#include <psapi.h>
#include "FunctionHook.h"
namespace HYJ
{
	std::set<std::string> DllInjectionChecker::WhiteList;
	unsigned char DllInjectionChecker::LoadLibraryA_originalCode[12] = { 0, };

	bool DllInjectionChecker::MemoryInfoMationCheck(LPTHREAD_START_ROUTINE lpStartAddress)
	{
		MEMORY_BASIC_INFORMATION memInfo;

		if (!VirtualQuery(lpStartAddress, &memInfo, sizeof(memInfo)))
		{
			return false;
		}

		if (memInfo.Type == MEM_PRIVATE)
		{
			return true;
		}

		return false;
	}

	bool IsCreateRemoteThread(LPTHREAD_START_ROUTINE lpStartAddress)
	{

		return false;
	}

	bool DllInjectionChecker::ImportAddressTableCheck(const char* functionName, const char* moduleName)
	{
		


		return true;
	}
	
	void DllInjectionChecker::PushWhiteListDllName(std::string dllName) noexcept
	{
		WhiteList.insert(dllName);
	}

	bool DllInjectionChecker::IsExistInWhiteList(std::string dllName) noexcept
	{
		if (WhiteList.find(dllName) != WhiteList.end())
		{
			return true;
		}

		return false;
	}

	HMODULE WINAPI DllInjectionChecker::WhiteListLoadLibrary(LPCSTR lpFileName)
	{

		if (DllInjectionChecker::IsExistInWhiteList(lpFileName) == false)
		{
			return NULL;
		}

		HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
		void* address = GetProcAddress(kernel32,"LoadLibraryA");
		FunctionHook::UnHook(address, LoadLibraryA_originalCode);
		HMODULE hModule= LoadLibraryA(lpFileName);
		FunctionHook::SetHook(address, WhiteListLoadLibrary);

		return hModule;
	}


}