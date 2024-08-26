#pragma once
#include "util.h"
#include "SHA256.h"


namespace HYJ
{
	bool Util::MultiStringSearch(std::vector<std::string>& searchList, const std::string& target)
	{
		for (const std::string& searchString : searchList)
		{
			if (strstr(searchString.c_str(), target.c_str()) != 0)
			{
				return true;
			}
		}

		return false;
	}

	bool Util::StringCopy(char* buffer, char* str, size_t size)
	{
		if (strcpy_s(buffer, size, str) == 0)
		{
			return true;
		}
		return false;
	}

	std::unique_ptr<unsigned char[]> Util::GetReadFileAsync(const char* fileName, size_t* outFileSize)
	{
		HANDLE hEvent = NULL;
		HANDLE hFile = CreateFileA(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING,FILE_FLAG_OVERLAPPED, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return nullptr;
		}

		DWORD fileSize = GetFileSize(hFile, NULL);
		if (fileSize == INVALID_FILE_SIZE)
		{
			return nullptr;
		}
		
		std::unique_ptr<unsigned char[]> buffer = std::make_unique<unsigned char[]>(fileSize);
		hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (hEvent == NULL)
		{
			DEBUG_LOG("util", "fail to allocate event");
			return nullptr;
		}
		OVERLAPPED ol = { 0 };
		
		ol.Offset = 0;
		ol.hEvent = hEvent;
		
		DWORD readBytes = 0;
			
		bool result = ReadFile(hFile, buffer.get(), fileSize, &readBytes, &ol);
		if (result == false && GetLastError() != ERROR_IO_PENDING)
		{
			DEBUG_LOG("util", "fail to FileRead");
			
			CloseHandle(hEvent);
			CloseHandle(hFile);

			return nullptr;
		}
	

		while (!GetOverlappedResult(hFile, &ol, &readBytes, FALSE))
		{
			if (GetLastError() != ERROR_IO_INCOMPLETE)
			{
				DEBUG_LOG("util", "fail to get overlaapedResult()");
				
				CloseHandle(hEvent);
				CloseHandle(hFile);

				return nullptr;
			}
		}

		
		CloseHandle(hEvent);
		CloseHandle(hFile);
		
		*outFileSize = fileSize;

		return buffer;
	}

	const std::string Util::GetSha256(const unsigned char* data, size_t size)
	{
		return SHA256::CalculateSha256(data, size);
	}

	size_t Util::GetFunctionSize(const void* address)	
	{
		const unsigned char* functionAddress = static_cast<const unsigned char*>(address);
		
		unsigned int functionSize = 0;

		do
		{
			++functionSize;
		}while( *(++functionAddress) != 0x3C);


		return functionSize;
	}

	std::string Util::GetFunctionHash(const void* address, size_t functionSize)
	{
		std::unique_ptr<unsigned char[]> functionData = std::make_unique<unsigned char[]>(functionSize);
		
		if (memcpy_s(functionData.get(), functionSize, address, functionSize) != 0)
		{
			DEBUG_LOG("util", "Memcpy fail in GetFunctionHash");
			return "";
		}

		return GetSha256(functionData.get(), functionSize);
	}

	std::string Util::ConvertToLowerCaseString(std::string str) noexcept
	{
		std::string lowerCaseString;
		lowerCaseString.reserve(str.size());

		for (char ch : str)
		{
			lowerCaseString.push_back(tolower(ch));
		}

		return lowerCaseString;
	}

	std::string Util::ConvertToUpperCaseString(std::string str) noexcept
	{

		std::string lowerCaseString;
		lowerCaseString.reserve(str.size());

		for (char ch : str)
		{
			lowerCaseString.push_back(toupper(ch));
		}

		return lowerCaseString;

	}

}