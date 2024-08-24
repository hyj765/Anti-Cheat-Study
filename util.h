#pragma once
#include"stdafx.h"

namespace HYJ
{

	class Util
	{
	public:
		
		static bool MultiStringSearch(std::vector<std::string>& searchList, const std::string& target);	
		
		static bool StringCopy(char* buffer, char* str, size_t size);
		
		static std::unique_ptr<unsigned char[]> GetReadFileAsync(const char* fileName,size_t* outFileSize,DWORD bufferSize = 4096);
		
		static const std::string GetSha256(const unsigned char* data, size_t size);

		static size_t GetFunctionSize(const void* address);

		static std::string GetFunctionHash(const void* address, size_t functionSize);

		static std::string ConvertToLowerCaseString(std::string str) noexcept;
		
		static std::string ConvertToUpperCaseString(std::string str) noexcept;
		

	};
	


}