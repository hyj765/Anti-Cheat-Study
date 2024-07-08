#pragma once
#include<Windows.h>

namespace HYJ
{
	typedef struct _WINAPI_MODULE_TABLE
	{
		HMODULE hBaseModule;
		HMODULE hKernel32;
		HMODULE hNtdll;
		HMODULE hUser32;
		HMODULE hPsapi;
		HMODULE hDbghelp;
		HMODULE hKernelbase;
		HMODULE hPython;
		HMODULE hAdvapi32;
		HMODULE hWininet;
		HMODULE hWinsta;
		HMODULE hShlwapi;
		HMODULE hShell32;
		HMODULE hCrypt32;
		HMODULE hWs2_32;
		HMODULE hIphlpapi;
		HMODULE hMpr;
		HMODULE hWintrust;
		HMODULE hDnsapi;
		HMODULE hOle32;
		HMODULE hGdiPlus;
		HMODULE hGdi32;
		HMODULE hEvtApi;
		HMODULE hUserEnv;
	}WINAPI_MODULE_TABLE, * PWINAPI_MODULE_TABLE;
	
	struct _WINAPI_API_TABLE
	{

	};

	class DynamicWinapi
	{
		
	};





}

