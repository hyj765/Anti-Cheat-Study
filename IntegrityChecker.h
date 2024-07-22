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
		
		bool DllIntegrity() noexcept;

		

	private:
		IntegrityChecker();
		~IntegrityChecker();
		std::unordered_map<std::string, std::string> hashList;
		
	};














}