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

		bool InsertHashList(std::string&& keyName, std::string&& hash) noexcept;

		bool InsertFileHash(std::string filePath);
		
		bool CompareFunctionIntegrity(const void* functionAddress, std::string functionName);
		
		bool CheckDllIntegrity(const char* dllName, HMODULE dllImageBase = NULL) noexcept;

		bool FunctionIsHooked(void* address) noexcept;

		bool CheckIATAddress(void* address) noexcept;

		IntegrityChecker();

		~IntegrityChecker();

	private:
		
		std::unordered_map<std::string, std::string> hashList;
		
		std::unique_ptr<PEParser> peparser;

	};




}