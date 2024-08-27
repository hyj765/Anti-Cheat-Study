#include "IntegrityChecker.h"
#include "util.h"
#include "PEParser.h"

namespace HYJ
{
	IntegrityChecker::IntegrityChecker(const std::shared_ptr<PEParser>& pe) : peParser(pe){}

	IntegrityChecker::~IntegrityChecker(){}

	std::string IntegrityChecker::GetSectionHash(const char* sectionName) noexcept
	{

		auto sectionHeader = peParser.get()->GetSectionHeader(sectionName);
		if (sectionHeader == nullptr)
		{
			DEBUG_LOG("integrity", "fail to get section header");
			return "";
		}

		size_t dataSize = 0;

		std::unique_ptr<unsigned char[]> sectionData = peParser.get()->GetSectionBody(sectionHeader,&dataSize);
		if (dataSize == 0)
		{
			DEBUG_LOG("integrity", "Fail To Get DataSize");
			return "";
		}
		
		return Util::GetSha256(sectionData.get(),dataSize);

	}

	const std::string IntegrityChecker::GetFileHash(const char* fileName) noexcept
	{
		size_t fileSize = 0;
		std::unique_ptr<unsigned char[]> fileData =Util::GetReadFileAsync(fileName,&fileSize);
		if (fileData == nullptr)
		{
			DEBUG_LOG("integrity", "filedata is Empty");
			return "";
		}
		//sizeof(filedata.get()) isn't correct data size so it need to be fix it
		return Util::GetSha256(fileData.get(),fileSize);
	}

	/*	
		if opcode be changed then this code return false;
		if this function return true, it's mean integrity is guaranteed
	*/
	bool IntegrityChecker::FunctionIsHooked(void* address) noexcept
	{
		DWORD oldProtect = 0;
		if (!VirtualProtect(address, 1, PAGE_READONLY, &oldProtect))
		{
			DEBUG_LOG("integrity", "fail to Proceeding Virtualprotect in FunctionHooked");
			return false;
		}

		unsigned char opcode = *reinterpret_cast<unsigned char*>(address);

		if (!VirtualProtect(address, 1, oldProtect, &oldProtect))
		{
			DEBUG_LOG("integrity", "fail to Proceeding Virtualprotect in FunctionHooked");
			return false;
		}

		if (opcode == 0xE9 || opcode == 0xFF)
		{
			return false;
		}
		
		return true;
	}

	bool IntegrityChecker::CompareFileIntegrity(std::string fileHash, std::string fileName) noexcept
	{

		if (hashList.find(fileName) == hashList.end())
		{
			DEBUG_LOG("integrity", "unknown FileName");
			return false;
		}

		if (fileHash != hashList[fileName])
		{
			DEBUG_LOG("integrity", "integrity check fail");
			return false;
		}

		return true;
	}

	bool IntegrityChecker::CompareFunctionIntegrity(const void* functionAddress, std::string functionName)
	{
		size_t functionSize = Util::GetFunctionSize(functionAddress);
		if (functionSize == 0)
		{
			DEBUG_LOG("integrity", "fail to get function size in compare function integrity");
			return false;
		}

		std::string functionHash =Util::GetFunctionHash(functionAddress, functionSize);
		if (functionHash == "")
		{
			DEBUG_LOG("integrity", "fail to get function data in compare function integrity");
			return false;
		}

		if (hashList.find(functionName) == hashList.end())
		{
			DEBUG_LOG("integrity", "there is no hash in hashlist in compare function integrity");
			return false;
		}

		if (functionHash != hashList[functionName])
		{
			return false;
		}
		
		return true;
	}

	bool IntegrityChecker::CheckDllIntegrity(const char* dllName, HMODULE dllImageBase) noexcept
	{

		if (dllImageBase == NULL)
		{
			dllImageBase = LoadLibraryA(dllName);
			if (dllImageBase == NULL)
			{
				DEBUG_LOG("Integrity", "invalid dll path in Checkdllintegrity");
				return false;
			}
		}

		size_t dataSize;
		std::unique_ptr<unsigned char[]> Sectiondata=peParser.get()->ExtractSectionHeaderFromDll(dllImageBase, &dataSize);
				

		std::string dllCodeHash = Util::GetSha256(Sectiondata.get(), dataSize);
		if (dllCodeHash == "")
		{
			DEBUG_LOG("integrity", "fail to get dll Hash Value");
			return false;
		}


		if (hashList.find(std::string(dllName)) == hashList.end())
		{
			DEBUG_LOG("integrity", "dll hashList size is zero");
			return false;
		}

		if (hashList[dllName] != dllCodeHash)
		{
			return false;
		}

		return true;

	}

	bool IntegrityChecker::InsertHashList(std::string&& keyName, std::string&& hash) noexcept
	{
		if (hash == "" || keyName == "")
		{
			DEBUG_LOG("integrity", "HashValue or KeyName is None");
			return false; 
		}

		hashList.insert({keyName,hash});

		return true;
	}

	bool IntegrityChecker::InsertFileHash(std::string fileName)
	{
		size_t fileSize = 0;
		std::unique_ptr<unsigned char[]> data= Util::GetReadFileAsync(fileName.c_str(), &fileSize);
		std::string fileHash= Util::GetSha256(data.get(), fileSize);

		if (fileHash == "")
		{
			DEBUG_LOG("integrity", "HashValue is None");
			return false;
		}

		return InsertHashList(std::move(fileName), std::move(fileHash));
	}


}
