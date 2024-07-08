#include<iostream>
#include<Windows.h>
#include"ThreadManager.h"

using namespace std;
typedef void (WINAPI* BaseThreadInitThunkType)(DWORD, LPTHREAD_START_ROUTINE, LPVOID);

int top = 0;

void temp(int count)
{
	for (int i = 0; i < count; ++i)
	{
		top++;
	}

	return;
}


int main()
{

	
	/*
	HYJ::ThreadManager& s = HYJ::ThreadManager::getInstance();

	if (!HYJ::ThreadManager::getInstance().IsThreadCreateByThreadManager(45646))
	{
		printf("thread not here");
	}

	s.CreateThreads(temp,10000,"happy");
	*/
	/*
	HMODULE hModule=LoadLibraryA("kernel32.dll");
	
	BaseThreadInitThunkType baseThreadthunk = reinterpret_cast<BaseThreadInitThunkType>(GetProcAddress(hModule,"BaseThreadInitThunk"));
	if (baseThreadthunk != NULL)
	{
		cout << "threadthunkaddress: " << static_cast<void*>(baseThreadthunk) << endl;
	}

	MEMORY_BASIC_INFORMATION mbi;
	if (VirtualQuery(baseThreadthunk, &mbi, sizeof(mbi)) == 0)
	{
		FreeLibrary(hModule);
		return 1;
	}

	LPVOID startAddress = baseThreadthunk;
	LPVOID endAddress = static_cast<BYTE*>(mbi.BaseAddress) + mbi.RegionSize;

	SIZE_T dumpsize = min(256, static_cast<SIZE_T>(static_cast<BYTE*>(endAddress) - static_cast<BYTE*>(startAddress)));

	cout << dumpsize;
	FreeLibrary(hModule);
	*/



	/*
	BYTE buffer[32];
	std::string pt = "helloworld";
	static_cast<UINT>(pt.size());
	SHA256_Encrpyt(reinterpret_cast<const BYTE*>(pt.c_str()), static_cast<UINT>(pt.size()),buffer);


	for (int i = 0; i < 32; ++i)
	{
		printf("%02x", buffer[i]);
	}
	*/



}