#pragma once
#include "stdafx.h"
#include "WinApiTypes.h"

namespace HYJ
{	
  
    /*
    struct WinAPIList
    {
        WinAPITypeList::pNtqueryInformationProcess pNtqueryInfomation;
    };

    */
    /*
	struct WinModuleList
	{
		HMODULE hKernel32;
		HMODULE hNtDll;
		HMODULE hUser32;
	};
    */

	class AntiDebugger
	{

	public:

        bool CheckParentIsDebugger();

		bool isProcessDebugged() noexcept;
		
        bool CheckDebugPort() noexcept;
        
        bool CheckDebugFlags()noexcept;
        
        bool CheckDebugObjectHandle()noexcept;
        
        bool CheckProcessHeapFlags() noexcept;
        
        bool CheckHardWareCheckPoint() noexcept;
        
        template<typename FUNCTION, typename... ARGS>
        static bool CheckFunctionProceedingTime(FUNCTION function, DWORD LimiteProceedingTime, ARGS&&... args);

	private:
		AntiDebugger();

        WinAPITypeList::pNtqueryInformationProcess NtQueryInformationProcess;

	//	struct WinAPIList winApis;
	//	struct WinModuleList winModules;
        

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