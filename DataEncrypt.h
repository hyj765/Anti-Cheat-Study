#pragma once
#include "stdafx.h"
#include <wincrypt.h>
#pragma comment(lib, "crypt32.lib")
namespace HYJ
{

	class DataEncrypt
	{
		/*
			input data = target data which you want to encrypt
			output size_t size = original Data Size it's need to be when you decrypt to encryption data;
		*/
		static bool DataEnCryption(std::vector<unsigned char>& data, size_t size);

		/*
			input data = the data that you want to get decryption
			size_t data size = original length actually this parameter is optional
			because padding data fill with zero;
		*/
		static bool DataDeCryption(std::vector<unsigned char>& data, size_t dataSize);

		static const std::string StringXOREncrpytionDecryption(const std::string& str, char* key) noexcept;
		
		template<typename DATATYPE>
		static const DATATYPE DataEncryptDecryptToXor(DATATYPE& data, char key) noexcept;

		static void AddPaddingToBlockSize(std::vector<BYTE>& data, size_t blockSize);

	};

	template<typename DATATYPE>
	const DATATYPE DataEncrypt::DataEncryptDecryptToXor(DATATYPE& data, char key) noexcept
	{
		return data ^ key;
	}


}