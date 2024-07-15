#pragma once
#include <Windows.h>
#include<vector>
#include<string>
#include<iostream>

namespace HYJ
{

	class PEParser
	{
	public:

		PIMAGE_DOS_HEADER GetDosHeader(void* imagebase) noexcept;
		
		PIMAGE_NT_HEADERS GetNtHeader(void* imagebase, const PIMAGE_DOS_HEADER dosHeader) noexcept;
		
		PIMAGE_FILE_HEADER GetFileHeader(const PIMAGE_NT_HEADERS ntHeader) noexcept;
		
		PIMAGE_OPTIONAL_HEADER GetOptionalHeader(const PIMAGE_NT_HEADERS ntHeader) noexcept;
		
		std::vector<PIMAGE_SECTION_HEADER> GetAllSectionHeader(const PIMAGE_NT_HEADERS ntHeader) noexcept;
		
		PIMAGE_SECTION_HEADER GetSectionHeader(const char* sectionHeaderName) noexcept;
		
		std::unique_ptr<unsigned char[]> GetSectionBody(const PIMAGE_SECTION_HEADER sectionHeader) noexcept;
	
		

		PEParser();

	private:
		
		void* imagebase;
		
		IMAGE_DOS_HEADER* dosHeader = nullptr;
		
		IMAGE_NT_HEADERS* ntHeader = nullptr;
		
		IMAGE_FILE_HEADER* fileHeader = nullptr;
		
		IMAGE_OPTIONAL_HEADER* optionalHeader = nullptr;
		
		std::vector<IMAGE_SECTION_HEADER*> sectionHeaders;
	};


}