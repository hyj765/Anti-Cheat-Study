#pragma once
#include "stdafx.h"

namespace HYJ
{

	class FunctionHook
	{
	public:
		
		static bool SetHook(void* targetFunctionAddress, void* hookFunctionAddress, unsigned char* originalBuff = nullptr);
		
		static bool UnHook(void* targetFunctionAddress, unsigned char* originalBytes);
		
		static bool FunctionBlock(void* address, int threadNumber);
		
		static bool FunctionUnBlock(void* address,int threadNumber);
	
		static std::map<void*, unsigned char> originalCode;		
	};

}