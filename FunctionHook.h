#pragma once
#include "stdafx.h"

namespace HYJ
{

	class FunctionHook
	{
	public:
		
		static bool SetHook(void* targetFunctionAddress, void* hookFunctionAddress, unsigned char* originalBuff = nullptr);
		
		static bool UnHook(void* targetFunctionAddress, unsigned char* originalBytes);
		
		bool FunctionBlock(void* address, int threadNumber);
		
		bool FunctionUnBlock(void* address,int threadNumber);
	
	private:
		
		std::map<void*, unsigned char> originalCode;

	};

}