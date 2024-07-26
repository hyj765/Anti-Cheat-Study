#pragma once
#include "stdafx.h"

namespace HYJ
{

	class DataEncrpyt
	{

		template<typename T>
		static bool DataEnCryption(T data, BYTE* outPut)
		{
			/*

			size_t EncryptionDataSize = ((sizeof(data) + CRYPTPROTECTMEMORY_BLOCK_SIZE -1) / CRYPTPROTECTMEMORY_BLOCK_SIZE)* CRYPTPROTECTMEMORY_BLOCK_SIZE;

			CryptProtectMemory(data,EncryptionDataSize, CRYPTPROTECTMEMORY_SAME_PROCESS);

			memcpy(output, data, EncryptionDataSize);

			*/

			return false;
		}

		template<typename T>
		static void DataDeCryption(T data)
		{


		}

		static const std::string StringXOREncrpytionDecryption(const std::string& str, char* key) noexcept;




	};



}