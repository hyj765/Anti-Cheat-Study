#pragma once

namespace HYJ
{

	class FunctionHook
	{
	public:
		void SetHook(void* targetFunctionAddress, void* hookFunctionAddress);
		bool FunctionBlock(void* address);

	private:

	};

}