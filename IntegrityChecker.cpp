#include "IntegrityChecker.h"
#include "util.h"


namespace HYJ
{

	char* IntegrityChecker::GetSectionHash() noexcept
	{
		

		return NULL;
	}

	unsigned char* IntegrityChecker::GetFileHash(const char* fileName) noexcept
	{
		std::unique_ptr<unsigned char[]> fileData =Util::GetReadFileAsync(fileName);
		if (fileData == nullptr)
		{
			return nullptr;
		}

		unsigned char* hash =Util::GetSha256(fileData.get());

		return hash;
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
			return false;
		}

		unsigned char opcode = *reinterpret_cast<unsigned char*>(address);

		if (!VirtualProtect(address, 1, oldProtect, &oldProtect))
		{
			return false;
		}

		if (opcode == 0xE9 || opcode == 0xFF)
		{
			return false;
		}
		
		return true;
	}

}