#pragma once
#include<Windows.h>
#include<atomic>

namespace HYJ
{
	class FunctionHook;
	class AntiDebugger;

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
		ProcessProtector();

		bool RegistThreadFilterFunction(void* address) noexcept;

		bool MutiClientCheck();
		/*
		  function Blocking function it's change first assembly code to ret
		  so the function which this function is applied is returned without being excuted
		*/
		bool BlockFunction(void* Address);
		/*
			this function restore to blocking function's assembly code 
		*/
		bool OpenFunction(void* Address);
		// even this two function's are using critical section but it take a lot of cost
	private:
		FunctionHook* hookmanager;
		AntiDebugger* AntiDebugger;
		CRITICAL_SECTION criticalSection;
		std::atomic<int> threadNumber = 0;
		
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