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



}