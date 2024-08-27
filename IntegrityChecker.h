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

		IntegrityChecker(const std::shared_ptr<PEParser>& pe);

		~IntegrityChecker();

	private:
		
		std::unordered_map<std::string, std::string> hashList;
		
		std::shared_ptr<PEParser> peParser;

	};




}