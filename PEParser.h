#pragma once
#include "stdafx.h"

namespace HYJ
{

	class PEParser
	{
	public:

		PIMAGE_DOS_HEADER GetDosHeader(void* imagebase, bool selfFlag = false) noexcept;
		
		PIMAGE_NT_HEADERS GetNtHeader(void* imagebase, const PIMAGE_DOS_HEADER dosHeader, bool selfFlag = false ) noexcept;
		
		PIMAGE_FILE_HEADER GetFileHeader(const PIMAGE_NT_HEADERS ntHeader, bool selfFlag = false ) noexcept;
		
		PIMAGE_OPTIONAL_HEADER GetOptionalHeader(const PIMAGE_NT_HEADERS ntHeader, bool selfFlag = false ) noexcept;
		
		std::vector<PIMAGE_SECTION_HEADER> GetAllSectionHeader(const PIMAGE_NT_HEADERS ntHeader, bool selfFlag = false ) noexcept;
		
		PIMAGE_SECTION_HEADER GetSectionHeader(const std::vector<PIMAGE_SECTION_HEADER>& sectionHeaderList ,const char* sectionHeaderName) noexcept;
		
		PIMAGE_SECTION_HEADER GetSectionHeader(const char* sectionName) noexcept;
		
		std::unique_ptr<unsigned char[]> GetSectionBody(const PIMAGE_SECTION_HEADER sectionHeader, size_t* dataSize) noexcept;
	
		ULONGLONG GetAddressFromImportAddressTable(const char* LibraryName,const char* functionName);

		std::unique_ptr<unsigned char[]> ExtractSectionHeaderFromDll(HMODULE dllImageBase, size_t* dataSize);
		
		PEParser();

		~PEParser() = default;
	private:
		
		void* imagebase;
		
		IMAGE_DOS_HEADER* dosHeader = nullptr;
		
		IMAGE_NT_HEADERS* ntHeader = nullptr;
		
		IMAGE_FILE_HEADER* fileHeader = nullptr;
		
		IMAGE_OPTIONAL_HEADER* optionalHeader = nullptr;
		
		std::vector<IMAGE_SECTION_HEADER*> sectionHeaders;

	};


}