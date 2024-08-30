#pragma once
#include "stdafx.h"
#include <atomic>
#include <array>

namespace HYJ
{	
  
	class AntiDebugger
	{
    public:

        AntiDebugger();

        ~AntiDebugger();

        bool CheckParentIsDebugger();

		bool IsProcessDebugged() noexcept;
		
        bool CheckDebugPort() noexcept;
        
        bool CheckDebugFlags()noexcept;
        
        bool CheckDebugObjectHandle()noexcept;
        
        bool CheckProcessHeapFlags() noexcept;
        
        bool CheckHardWareCheckPoint() noexcept;

        //bool CheckIsDebuggerPresentModified() noexcept;

        bool BlockDbgBreakPoint() noexcept;

        bool BlockDebuggerAttach() noexcept;
        
        template<typename FUNCTION, typename... ARGS>
        static bool CheckFunctionProceedingTime(FUNCTION function, DWORD LimiteProceedingTime, ARGS&&... args);

	private:
        static constexpr std::array<const char*, 23> debuggersName = { "ida.exe","ollydbg.exe","ida64.exe", "idag.exe", "idag64.exe", "idaw.exe", "idaw64.exe", "idaq.exe", "idaq64.exe", "idau.exe", "idau64.exe", "scylla.exe", "scylla_x64.exe", "scylla_x86.exe", "protection_id.exe", "x64dbg.exe", "x32dbg.exe", "windbg.exe", "reshacker.exe", "ImportREC.exe", "IMMUNITYDEBUGGER.EXE", "devenv.exe" };

        WinAPITypeList::pNtqueryInformationProcess NtQueryInformationProcess = nullptr;
        //WinAPITypeList::isDebuggerPresentType pIsDebuggerPresent;
    };

    
    template<typename FUNCTION, typename... ARGS>
    bool AntiDebugger::CheckFunctionProceedingTime(FUNCTION function, DWORD LimiteProceedingTime, ARGS&&... args) 
    {
        
        DWORD startTime = GetTickCount();
        
        std::invoke(function, std::forward<ARGS>(args)...);
        DWORD proceedingTime = GetTickCount() - startTime;

       

        return (proceedingTime > LimiteProceedingTime);
    }
    

    
}