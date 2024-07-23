#pragma once
#include<Windows.h>
#include<atomic>

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
		ProcessProtector& GetInstance()
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
	private:
		ProcessProtector();
		~ProcessProtector();
		ProcessProtector& operator=(ProcessProtector& ref);

		void __fastcall RegistThreadFilterFunction(DWORD LdrReserved, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter);
		typedef void(__fastcall* BaseThreadInitThunkType)(DWORD, LPTHREAD_START_ROUTINE, PVOID);
		FunctionHook* hookmanager;
		AntiDebugger* AntiDebugger;
		int threadNumber = 0;
		DWORD mainThreadId;
		CRITICAL_SECTION criticalSection;
		unsigned char originalThreadThunkBytes[12] = { 0, };

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
	};
}