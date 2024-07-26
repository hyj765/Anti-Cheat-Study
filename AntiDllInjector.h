#pragma once
#include "stdafx.h"


namespace HYJ
{
	//class IntegrityChecker;

	class DllInjectionChecker
	{
	public:

		bool MemoryInfoMationCheck(LPTHREAD_START_ROUTINE lpStartAddress);

		bool IsCreateRemoteThread(LPTHREAD_START_ROUTINE lpStartAddress);

		bool ImportAddressTableCheck(const char* functionName, const char* moduleName);
		
		static void PushWhiteListDllName(std::string dllName) noexcept;

		static bool IsExistInWhiteList(std::string dllName) noexcept;


		/*
			Example Base Function
			WhiteList LoadLibrary
		*/
		static HMODULE WINAPI WhiteListLoadLibrary(LPCSTR lpFileName);
		
		static std::set<std::string> WhiteList;
		
		static unsigned char LoadLibraryA_originalCode[12];

	private:
		
	};

	



}