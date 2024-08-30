#pragma once
#include"stdafx.h"
#include <array>

namespace HYJ
{

	class Util
	{
	public:
		
		static bool MultiStringSearch(std::vector<std::string>& searchList, const std::string& targetString);	
		
		template<std::size_t SIZE>
		static bool MultiStringSearchConstChar(const std::array<const char*, SIZE>& searchList, const char* targetString);

		static bool StringCopy(char* buffer, char* str, size_t size);
		
		static std::unique_ptr<unsigned char[]> GetReadFileAsync(const char* fileName,size_t* outFileSize);
		
		static const std::string GetSha256(const unsigned char* data, size_t size);

		static size_t GetFunctionSize(const void* address);

		static std::string GetFunctionHash(const void* address, size_t functionSize);

		static std::string ConvertToLowerCaseString(std::string str) noexcept;
		
		static std::string ConvertToUpperCaseString(std::string str) noexcept;
		

	};
	
	template<std::size_t SIZE>
	bool Util::MultiStringSearchConstChar(const std::array<const char*, SIZE>& searchList, const char* targetString)
	{
		for (const char* searchString : searchList)
		{
			if (strstr(ConvertToLowerCaseString(searchString).c_str(), ConvertToLowerCaseString(targetString).c_str()) != 0)
			{
				return true;
			}
		}


		return false;
	}
}