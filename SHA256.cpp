#include "KISA_SHA256.h"
#include "SHA256.h"


namespace HYJ
{

#define SHA256_LENGTH 32
	
	std::string SHA256::CalculateSha256(const unsigned char* text, size_t textlength)
	{	
		unsigned char buffer[SHA256_LENGTH];
		SHA256_Encrpyt(text, static_cast<UINT>(textlength), buffer);

		return std::string(reinterpret_cast<const char*>(buffer), SHA256_LENGTH);
	}


}