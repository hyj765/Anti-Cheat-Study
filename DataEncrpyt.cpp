#include "DataEncrypt.h"

namespace HYJ
{
	const std::string DataEncrpyt::StringXOREncrpytionDecryption(const std::string& str, char* key) noexcept
	{

		std::string buffer;
		buffer.resize(str.size());
		int keySize = strlen(key);

		for (int i = 0; i < str.size(); ++i)
		{
			buffer.push_back(str[i] ^ key[i % keySize]);
		}

		return buffer;
	}

}