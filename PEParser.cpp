#include "PEParser.h"
#include <iostream>

namespace HYJ
{
	/*
		PeParser firstable allocate each PE Headers
	*/
	PEParser::PEParser()
	{
		imagebase= GetModuleHandle(NULL);
		dosHeader = GetDosHeader(imagebase);
		ntHeader = GetNtHeader(imagebase, dosHeader);
		fileHeader = GetFileHeader(ntHeader);
		optionalHeader = GetOptionalHeader(ntHeader);
		sectionHeaders = GetAllSectionHeader(ntHeader);

	}

	PIMAGE_DOS_HEADER PEParser::GetDosHeader(void* imagebase) noexcept
	{
		if (dosHeader != nullptr)
		{
			return dosHeader;
		}

		if (imagebase == nullptr)
		{
			return nullptr;
		}

		PIMAGE_DOS_HEADER pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(imagebase);
		if (pDosHeader != nullptr)
		{
			return pDosHeader;
		}

		return nullptr;
	}

	PIMAGE_NT_HEADERS PEParser::GetNtHeader(void* imagebase, const PIMAGE_DOS_HEADER dosHeader) noexcept
	{
		if (ntHeader != nullptr)
		{
			return ntHeader;
		}

		if (imagebase == nullptr || dosHeader == nullptr)
		{
			return nullptr;
		}

		PIMAGE_NT_HEADERS pNtHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<unsigned char*>(imagebase) + dosHeader->e_lfanew);
		return pNtHeader;
	}

	PIMAGE_FILE_HEADER PEParser::GetFileHeader(const PIMAGE_NT_HEADERS ntHeaders) noexcept
	{
		if (fileHeader != nullptr)
		{
			return fileHeader;
		}

		if (ntHeaders == nullptr)
		{
			return nullptr;
		}

		return &ntHeaders->FileHeader;
	}

	PIMAGE_OPTIONAL_HEADER PEParser::GetOptionalHeader(const PIMAGE_NT_HEADERS ntHeader) noexcept
	{
		if (optionalHeader != nullptr)
		{
			return optionalHeader;
		}

		if (ntHeader == nullptr)
		{
			return nullptr;
		}

		return &ntHeader->OptionalHeader;
	}


	std::vector<PIMAGE_SECTION_HEADER> PEParser::GetAllSectionHeader(const PIMAGE_NT_HEADERS ntHeader) noexcept
	{
		
		if (sectionHeaders.size() != 0)
		{
			return sectionHeaders;
		}

		PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(ntHeader);
		std::vector<PIMAGE_SECTION_HEADER> sectionHeaderArray;
		for (int i = 0; i < ntHeader->FileHeader.NumberOfSections; ++i)
		{
			char* name = reinterpret_cast<char*>(pSectionHeader->Name);
			sectionHeaderArray.push_back(pSectionHeader);
			pSectionHeader++;
		}

		return sectionHeaderArray;
	}

	
	/*
	* this function return sectionHeader in sectionheaders
	* so previously getallsectionheader function must be proceeding
	*/
	PIMAGE_SECTION_HEADER PEParser::GetSectionHeader(const char* sectionHeaderName) noexcept
	{
		if (sectionHeaders.size() == 0)
		{
			return nullptr;
		}

		for (int i = 0; i < sectionHeaders.size(); ++i)
		{
			char* name = reinterpret_cast<char*>(sectionHeaders[i]->Name);
			if (strcmp(name, sectionHeaderName) == 0)
			{
				return sectionHeaders[i];
			}

		}

		return nullptr;
	}

	
	std::unique_ptr<unsigned char[]> PEParser::GetSectionBody(const PIMAGE_SECTION_HEADER sectionHeader) noexcept
	{
		std::unique_ptr<unsigned char[]> bytes = std::make_unique<unsigned char[]>(sectionHeader->Misc.VirtualSize);
		
		long sectionVirtualAddress = static_cast<long>(sectionHeader->VirtualAddress);
		memcpy(bytes.get(), (reinterpret_cast<char*>(imagebase) + sectionVirtualAddress), sectionHeader->Misc.VirtualSize);
		
		return bytes;
	}

	


}