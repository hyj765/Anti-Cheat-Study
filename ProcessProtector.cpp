#include"ProcessProtector.h"
#include "Anti Debugger.h"

namespace HYJ
{

	ProcessProtector::ProcessProtector() {};
	

	void ProcessProtector::blockLoadLibrary() noexcept
	{
		HMODULE hkernel32=GetModuleHandleA("kernel32.dll");
		
	}

}