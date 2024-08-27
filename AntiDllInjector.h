#pragma once
#include "stdafx.h"


namespace HYJ
{
	class PEParser;

	class DllInjectionChecker
	{
	public:

		bool MemoryInfoMationCheck(LPTHREAD_START_ROUTINE lpStartAddress);

		bool CheckLoadModuleList() noexcept;

		bool ImportAddressTableCheck(const char* functionName, const char* moduleName);
		
		void GetIATSnapShot() noexcept;

		static void PushWhiteListDllName(std::string dllName) noexcept;

		static bool IsExistInWhiteList(std::string dllName) noexcept;

		/*
			Example Base Function

			WhiteList LoadLibrary
		*/
		static HMODULE WINAPI WhiteListLoadLibrary(LPCSTR lpFileName);
		
		static std::set<std::string> WhiteList;
		
		static unsigned char LoadLibraryA_originalCode[12];
		
		std::map<std::string, ULONG> originalIATTableSnapShot;

		DllInjectionChecker(const std::shared_ptr<PEParser>& pe);
		~DllInjectionChecker();

	private:			
		const std::vector<std::string> windowsBaseDllList = {"ntdll.dll","kernel32.dll","advapi32.dll","user32.dll","msvcrt.dll","kernelbase.dll","msvcp140d.dll","vcruntime140_1d.dll","vcruntime140d.dll","ucrtbased.dll"};		
		std::shared_ptr<PEParser> peParser;
	};

}