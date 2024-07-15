#pragma once
#include<Windows.h>
#include<atomic>

namespace HYJ
{
	class FunctionHook;
	class AntiDebugger;

	class ProcessProtector
	{
	public:
		ProcessProtector();


		bool MutiClientCheck() noexcept;
		/*
		  function Blocking function it's change first assembly code to ret
		  so the function which this function is applied is returned without being excuted
		*/
		bool BlockFunction(void* Address);
		/*
			this function restore to blocking function's assembly code 
		*/
		bool OpenFunction(void* Address);
		// even this two function's are using critical section but it take a lot of cost
	private:
		FunctionHook* hookmanager;
		AntiDebugger* AntiDebugger;
		CRITICAL_SECTION criticalSection;
		std::atomic<int> threadNumber = 0;
	};
}