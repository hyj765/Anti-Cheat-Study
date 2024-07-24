#pragma once
#include "stdafx.h"

namespace HYJ
{


	class DllInjectionChecker
	{
	public:

		bool MemoryInfoMationCheck(LPTHREAD_START_ROUTINE lpStartAddress);
		bool ImportAddressTableCheck(const char* functionName, const char* moduleName);


	private:
		




	};


	



}