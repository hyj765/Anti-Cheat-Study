#include "PEParser.h"

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
	PIMAGE_SECTION_HEADER PEParser::GetSectionHeader(const char* sectionName) noexcept
	{

		if (sectionHeaders.size() == 0)
		{
			DEBUG_LOG("peparser", "Number of SectionHeaders is Zero");
			return nullptr;
		}

		for (int i = 0; i < sectionHeaders.size(); ++i)
		{
			char* name = reinterpret_cast<char*>(sectionHeaders[i]->Name);

			if (strcmp(Util::ConvertToLowerCaseString(name).c_str(), Util::ConvertToLowerCaseString(sectionName).c_str()) == 0)
			{
				return sectionHeaders[i];
			}

		}

		DEBUG_LOG("peparser", "There is no sectionHeader with matching names were found");

		return nullptr;
	}


	PIMAGE_SECTION_HEADER PEParser::GetSectionHeader(const std::vector<PIMAGE_SECTION_HEADER>& sectionHeaderList,const char* sectionHeaderName) noexcept
	{
		if (sectionHeaderList.size() == 0)
		{
			DEBUG_LOG("peparser", "Number of SectionHeaders is Zero");
			
			return nullptr;
		}

		for (int i = 0; i < sectionHeaderList.size(); ++i)
		{
			char* name = reinterpret_cast<char*>(sectionHeaderList[i]->Name);

			if (strcmp(name, sectionHeaderName) == 0)
			{
				return sectionHeaderList[i];
			}

		}

		
		DEBUG_LOG("peparser", "There is no sectionHeader with matching names were found");
		return nullptr;
	}

	std::unique_ptr<unsigned char[]> PEParser::GetSectionBody(const PIMAGE_SECTION_HEADER sectionHeader, size_t* dataSize) noexcept
	{
		*dataSize = sectionHeader->Misc.VirtualSize;

		std::unique_ptr<unsigned char[]> bytes = std::make_unique<unsigned char[]>(sectionHeader->Misc.VirtualSize);
		
		long sectionVirtualAddress = static_cast<long>(sectionHeader->VirtualAddress);

		memcpy(bytes.get(), (reinterpret_cast<char*>(imagebase) + sectionVirtualAddress), sectionHeader->Misc.VirtualSize);
		
		return bytes;
	}

	ULONGLONG PEParser::GetAddressFromImportAddressTable(const char* LibraryName, const char* functionName)
	{
		
		IMAGE_DATA_DIRECTORY importDataDirectory = optionalHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
		
		PIMAGE_IMPORT_DESCRIPTOR importDescriptor = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(static_cast<unsigned char*>(imagebase) + importDataDirectory.VirtualAddress);

		while (importDescriptor->Name != NULL)
		{
		 	
			char* libName= reinterpret_cast<char*>(importDescriptor->Name + static_cast<unsigned char*>(imagebase));
			
			if (strcmp(Util::ConvertToLowerCaseString(libName).c_str(), Util::ConvertToLowerCaseString(LibraryName).c_str()) == 0)
			{
				PIMAGE_THUNK_DATA originalFirstThunk = reinterpret_cast<PIMAGE_THUNK_DATA>(static_cast<unsigned char*>(imagebase) + importDescriptor->OriginalFirstThunk);
				PIMAGE_THUNK_DATA firstThunk = reinterpret_cast<PIMAGE_THUNK_DATA>(static_cast<unsigned char*>(imagebase) + importDescriptor->FirstThunk);
				while (originalFirstThunk->u1.AddressOfData != NULL)
				{
					PIMAGE_IMPORT_BY_NAME curfunctionName = reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(static_cast<unsigned char*>(imagebase) + originalFirstThunk->u1.AddressOfData);
					
					if (strcmp(Util::ConvertToLowerCaseString(curfunctionName->Name).c_str(), Util::ConvertToLowerCaseString(functionName).c_str()) == 0)
					{
						
						return firstThunk->u1.Function;
					}

					firstThunk++;
					originalFirstThunk++;
				}
				break;
			}

			importDescriptor++;
		}


		return NULL;
	}
	
	std::unique_ptr<unsigned char[]> PEParser::ExtractSectionHeaderFromDll(HMODULE dllImageBase, size_t* dataSize)
	{
		
		PIMAGE_DOS_HEADER dllDosHeader =GetDosHeader(dllImageBase);
		if (dllDosHeader == nullptr)
		{

			DEBUG_LOG("PeParser", "can not access to Dllbase address in Extract Section Header Function");
			
			return nullptr;
		}

		PIMAGE_NT_HEADERS dllNtHeader = GetNtHeader(dllImageBase, dllDosHeader);
		if (dllNtHeader == nullptr)
		{
			DEBUG_LOG("peparser", "can not Access to Ntheader in Extract Section Header function ");
			
			return nullptr;
		}

		std::vector<PIMAGE_SECTION_HEADER> sectionHeaderList = GetAllSectionHeader(dllNtHeader);
		if (sectionHeaderList.size() == 0)
		{	
			DEBUG_LOG("peparser", "Number of SectionHeaders is Zero");
			return nullptr;
		}

		PIMAGE_SECTION_HEADER dllsectionHeader = GetSectionHeader(sectionHeaderList,".text");
		if (dllsectionHeader == nullptr)
		{
			DEBUG_LOG("peparser", "Can not Found with matching to .text");
			return nullptr;
		}

		return GetSectionBody(dllsectionHeader, dataSize);
	}

}