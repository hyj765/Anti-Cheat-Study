#pragma once
#include <Windows.h>
#include<unordered_map>


namespace HYJ
{

	class IntegrityChecker
	{
	public:

		char* GetSectionHash() noexcept;
		
		unsigned char* GetFileHash(const char* fileNmae) noexcept;

		bool CompareFileIntegrity() noexcept;
		
		bool CompareFunctionIntegrity() noexcept;
		
		bool GetDllIntegrity() noexcept;

		static bool FunctionIsHooked(void* address) noexcept;

		static bool CheckIATAddress(void* address) noexcept;

	private:
		IntegrityChecker();
		~IntegrityChecker();
		std::unordered_map<std::string, std::string> hashList;
		
	};














}