#pragma once
#include "stdafx.h"

namespace HYJ
{
	class AntiDebugger;

	class AntiMacro;

	class DllInjectionChecker;

	class IntegrityChecker;

	class PEParser;

	/*
		ProcessProtector class include each Classes Anti Dll, Anti Debugger, Integrity Check class

	*/
	class ProcessProtector
	{
	public:
		
		static ProcessProtector& GetInstance() noexcept
		{
			static ProcessProtector instance;
			return instance;
		}
		
		/*
			Memory Protection Option Dep Enable Function
		*/	
		bool MemoryProtectionDep() noexcept;
		/*
		
		*/
		bool AntiDllinjection();
		
		bool MutiClientCheck();
		/*
		  function Blocking function it's change first assembly code to ret
		  so the function which this function is applied is returned without being excuted
		*/
		bool HookLoadLibrary(void* functionAddress);

		bool BlockFunction(void* Address);
		/*
			this function restore to blocking function's assembly code 
		*/
		bool OpenFunction(void* Address);
		// even this two function's are using critical section but it take a lot of cost
		bool CheckHardWareBreakPoint() noexcept;

		bool CheckIsDebuggerAttach() noexcept;

		bool DebuggerDetach() noexcept;

		bool IsProcessCreateByDebugger() noexcept;
	private:

		ProcessProtector();

		~ProcessProtector();
		
		ProcessProtector(const ProcessProtector&) = delete; // copy constructor delete
		
		ProcessProtector(ProcessProtector&&) noexcept = delete; // move constructor delete
		
		ProcessProtector& operator=(ProcessProtector& ref) = delete; // copy operator delete
		
		ProcessProtector& operator=(ProcessProtector&&) noexcept = delete; // move operator delete
		
		
		
		int threadNumber = 0;
		
		std::thread::id mainThreadId;
		
		CRITICAL_SECTION criticalSection;
		
		static void __fastcall RegistThreadFilterFunction(DWORD LdrReserved, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter);
		
		static WinAPITypeList::BaseThreadInitThunkType functionAddress;

		static unsigned char originalThreadThunkBytes[12];

		std::unique_ptr<AntiDebugger> antiDebugger;

		std::unique_ptr<AntiMacro> antiMacro;

		std::unique_ptr<DllInjectionChecker> antiDllInjector;

		std::unique_ptr<IntegrityChecker> integrityChecker;

		std::shared_ptr<PEParser> peparser;

	};


}