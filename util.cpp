#pragma once
#include "util.h"
#include<iostream>
#include<Windows.h>


namespace HYJ
{
	bool Util::MultiStringSearch(std::vector<std::string>& searchList, const std::string& target)
	{
		for (const std::string& searchString : searchList)
		{
			if (strstr(searchString.c_str(), target.c_str()) != 0)
			{
				return true;
			}
		}

		return false;
	}

	bool Util::StringCopy(char* buffer, char* str, size_t size)
	{
		if (strcpy_s(buffer, size, str) == 0)
		{
			return true;
		}
		return false;
	}

	std::unique_ptr<unsigned char[]> Util::GetReadFileAsync(const char* fileName, DWORD bufferSize)
	{
		HANDLE hFile = CreateFileA(fileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return nullptr;
		}

		DWORD fileSize = 0;
		if ((fileSize = GetFileSize(hFile, NULL)) == NULL)
		{
			return nullptr;
		}

		std::unique_ptr<unsigned char[]> buffer = std::make_unique<unsigned char[]>(fileSize);
		OVERLAPPED ol = { 0 };
		ol.Offset = 0;
		ol.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		DWORD readBytes = 0;
		DWORD totalRead = 0;

		while (totalRead < fileSize)
		{
			bool status = ReadFile(hFile, buffer.get() + totalRead, fileSize, &readBytes, &ol);
			if (!totalRead)
			{
				if (GetLastError() == ERROR_IO_PENDING)
				{
					if (!GetOverlappedResult(hFile, &ol, &readBytes, TRUE))
					{
						CloseHandle(ol.hEvent);
						CloseHandle(hFile);
						return nullptr;
					}
				}
				else
				{
					CloseHandle(ol.hEvent);
					CloseHandle(hFile);
					return nullptr;
				}
			}

			if (readBytes == 0)
			{
				break;
			}

			totalRead += readBytes;
			ol.Offset += readBytes;
			ResetEvent(ol.hEvent);
		}


		CloseHandle(ol.hEvent);
		CloseHandle(hFile);

		if (totalRead != fileSize)
		{
			return nullptr;
		}

		return buffer;
	}

	unsigned char* Util::GetSha256(const unsigned char* data)
	{
		//unsigned char hash[SHA256_DIGEST_LENGTH] = { 0 };
		
		//SHA256(data, sizeof(data), hash);

		return nullptr;
	}

	unsigned char* Util::GetCrc32(const char* data)
	{

		

		return nullptr;

	}

}