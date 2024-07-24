#pragma once
#include<Windows.h>
#include "WinApiTypes.h"

namespace HYJ
{
	class FunctionHook;
	class AntiDebugger;
	/*
		ProcessProtector class include each Classes Anti Dll, Anti Debugger, Integrity Check class

	*/
	class ProcessProtector
	{
	public:
		/*
		
			anti dll
			
			anti debugger
			
			integrity check
			
			multi process check
			
			additional 
			kernel driver

			

		*/
		ProcessProtector& GetInstance() noexcept
		{
			static ProcessProtector instance;
			return instance;
		}
		
		/*
			Memory Protection Option Dep Enable Function
		*/	
		bool MemoryProtectionDep() noexcept;
		/*
		
		*/
		bool AntiDllinjection();
		
		bool MutiClientCheck();
		/*
		  function Blocking function it's change first assembly code to ret
		  so the function which this function is applied is returned without being excuted
		*/
		bool HookLoadLibrary(void* functionAddress);
		bool BlockFunction(void* Address);
		/*
			this function restore to blocking function's assembly code 
		*/
		bool OpenFunction(void* Address);
		// even this two function's are using critical section but it take a lot of cost

		static void __fastcall RegistThreadFilterFunction(DWORD LdrReserved, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter);
		
		static WinAPITypeList::BaseThreadInitThunkType functionAddress;
		
		static unsigned char originalThreadThunkBytes[12];


	private:
		ProcessProtector();
		
		~ProcessProtector();
		ProcessProtector(const ProcessProtector&) = delete;
		ProcessProtector(ProcessProtector&&) noexcept = delete;
		ProcessProtector& operator=(ProcessProtector& ref) = delete;
		
	

		FunctionHook* hookmanager;
		
		AntiDebugger* AntiDebugger;
		
		int threadNumber = 0;
		
		DWORD mainThreadId;
		
		CRITICAL_SECTION criticalSection;

	

		/*
			datatype이 모인 header 하나 만들고 
		    해당 주소 값 가지고 있는 테이블 안에 인자로 넣어야할 꺼 같음.
		*/

	};

	class DataEncryptor
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

		static const char* StringXOREncrpytionDecryption(const char* str);
		
	};
}