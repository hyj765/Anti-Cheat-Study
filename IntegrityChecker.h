#pragma once
#include <Windows.h>
#include<unordered_map>
#include<iostream>


namespace HYJ
{
	class PEParser;

	class IntegrityChecker
	{
	public:

		std::string GetSectionHash(const char* sectionName) noexcept;
		
		const std::string GetFileHash(const char* fileNmae) noexcept;

		bool CompareFileIntegrity(std::string fileHash, std::string fileName) noexcept;
		
		bool CompareFunctionIntegrity(const void* functionAddress, std::string functionName);
		
		bool GetDllIntegrity() noexcept;

		static bool FunctionIsHooked(void* address) noexcept;

		static bool CheckIATAddress(void* address) noexcept;

	private:
		
		IntegrityChecker() :peparser(std::make_unique<PEParser>()) {};
		
		~IntegrityChecker();

		std::unordered_map<std::string, std::string> hashList;
		
		std::unique_ptr<PEParser> peparser;
	};




}