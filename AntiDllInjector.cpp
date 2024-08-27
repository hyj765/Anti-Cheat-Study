#include "AntiDllInjector.h"
#include <psapi.h>
#include "FunctionHook.h"
#include "PEParser.h"

namespace HYJ
{
	constexpr int MAX_MODULE_HANDLE = 1024;

	DllInjectionChecker::DllInjectionChecker(const std::shared_ptr<PEParser>& pe): peParser(pe) {}

	DllInjectionChecker::~DllInjectionChecker() {}

	std::set<std::string> DllInjectionChecker::WhiteList;
	unsigned char DllInjectionChecker::LoadLibraryA_originalCode[12] = { 0, };
	
	/*
		if lpStartAddress that thread address's infotype is mem_private then it's mean that this location was allocate by someone else 
	*/
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


	/*
		Process Dll WhiteList Check
	*/
	bool DllInjectionChecker::CheckLoadModuleList() noexcept
	{

		HANDLE CurProcessHandle = GetCurrentProcess();
		HMODULE ModulesHandle[MAX_MODULE_HANDLE];
		DWORD revModuleNumber = 0;
		if (EnumProcessModules(CurProcessHandle, ModulesHandle, sizeof(ModulesHandle), &revModuleNumber))
		{
			for (int i = 0; i < (revModuleNumber / sizeof(HMODULE)); i++)
			{
				char ModuleName[MAX_PATH];
				bool checkDllFlag = false;
				if (GetModuleFileNameExA(CurProcessHandle, ModulesHandle[i], ModuleName, MAX_PATH))
				{
					for (const std::string baseDllName: windowsBaseDllList)
					{
						if (strcmp(baseDllName.c_str(), ModuleName) == 0)
						{
							checkDllFlag = true;
							break;
						}
					}

					if ( checkDllFlag ) continue;
				}



				for (const std::string& WhiteDllName : WhiteList)
				{
					if (strcmp(WhiteDllName.c_str(), ModuleName) == 0)
					{
						checkDllFlag = true;
						break;
					}
				}

				if (checkDllFlag) continue;
			
				return false;
			}


		}

		return true;
	}

	void DllInjectionChecker::GetIATSnapShot() noexcept
	{
		std::vector < std::pair < std::string, ULONGLONG >> iatFunctionLIST = peParser.get()->GetAllAddressFromIAT();
		for (std::pair < std::string, ULONGLONG > iatFunction : iatFunctionLIST)
		{
			originalIATTableSnapShot.insert(iatFunction);
		}
	}

	bool DllInjectionChecker::ImportAddressTableCheck(const char* functionName, const char* moduleName)
	{
		ULONGLONG functionAddress = peParser.get()->GetAddressFromImportAddressTable(moduleName, functionName);
		
		if (originalIATTableSnapShot.find(functionName) == originalIATTableSnapShot.end())
		{
			return false;
		}

		if (originalIATTableSnapShot[functionName] != functionAddress)
		{
			return false;
		}

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
		if (kernel32 == NULL)
		{
			return NULL;
		}

		void* address = reinterpret_cast<void*>(GetProcAddress(kernel32,"LoadLibraryA"));

		FunctionHook::UnHook(address, LoadLibraryA_originalCode);
		HMODULE hModule= LoadLibraryA(lpFileName);
		FunctionHook::SetHook(address, WhiteListLoadLibrary);

		return hModule;
	}


}