#pragma once
#include<Windows.h>


namespace HYJ
{
	class FunctionHook;
	class AntiDebugger;

	class ProcessProtector
	{
	public:
		ProcessProtector();
		void blockLoadLibrary() noexcept;
	
	private:
	
		friend FunctionHook;
	
	};
}