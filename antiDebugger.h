#pragma once
#include "stdafx.h"
#include <atomic>

namespace HYJ
{	
  
	class AntiDebugger
	{
    public:

        AntiDebugger();

        ~AntiDebugger();

        bool CheckParentIsDebugger();

		bool isProcessDebugged() noexcept;
		
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