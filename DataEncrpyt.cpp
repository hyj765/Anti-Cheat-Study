#include "DataEncrypt.h"

namespace HYJ
{
	const std::string DataEncrypt::StringXOREncrpytionDecryption(const std::string& str, char* key) noexcept
	{

		std::string buffer;
		buffer.resize(str.size());
		size_t keySize = strlen(key);

		for (int i = 0; i < str.size(); ++i)
		{
			buffer.push_back(str[i] ^ key[i % keySize]);
		}

		return buffer;
	}

	void DataEncrypt::AddPaddingToBlockSize(std::vector<BYTE>& data, size_t blockSize)
	{
		size_t paddingRequired = (blockSize - (data.size() % blockSize)) % blockSize;
		if (paddingRequired > 0) {
			data.insert(data.end(), paddingRequired, 0);
		}
	}

	bool DataEncrypt::DataEnCryption(std::vector<unsigned char>& data, size_t size)
	{
		if (!CryptProtectMemory(data.data(), static_cast<DWORD>(data.size()),CRYPTPROTECTMEMORY_SAME_PROCESS))
		{
			DEBUG_LOG("dataEncryption", "encrpytion fail");
			return false;
		}

		return true;
	}

	bool DataEncrypt::DataDeCryption(std::vector<unsigned char>& data, size_t dataSize)
	{

		if (!CryptUnprotectMemory(data.data(), static_cast<DWORD>(data.size()), CRYPTPROTECTMEMORY_SAME_PROCESS))
		{
			DEBUG_LOG("dataEncryption", "dataDecryption Fail");
			return false;
		}

		data.resize(dataSize);

		return true;
	}

	


}