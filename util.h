#pragma once
#include"stdafx.h"

namespace HYJ
{

	class Util
	{
	public:
		
		static bool MultiStringSearch(std::vector<std::string>& searchList, const std::string& target);	
		
		static bool StringCopy(char* buffer, char* str, size_t size);
		
		static std::unique_ptr<unsigned char[]> GetReadFileAsync(const char* fileName,DWORD bufferSize = 4096);
		
		static unsigned char* GetSha256(const unsigned char* data);
		
		static unsigned char* GetCrc32(const char* data);

		static unsigned int GetFunctionSize(const void* address);

		static std::string ConvertToLowerCaseString(std::string str) noexcept;
		
		static std::string ConvertToUpperCaseString(std::string str) noexcept;
		

	};


}