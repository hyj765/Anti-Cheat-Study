#pragma once
#include<map>

namespace HYJ
{

	class FunctionHook
	{
	public:
		void SetHook(void* targetFunctionAddress, void* hookFunctionAddress);
		bool FunctionBlock(void* address, int threadNumber);
		bool FunctionUnBlock(void* address,int threadNumber);
	private:
		std::map<void*, unsigned char> originalCode;
	};

}