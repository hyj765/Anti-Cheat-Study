#include "KISA_SHA256.h"
#include "SHA256.h"
#include <iostream>


namespace HYJ
{

	
	unsigned char* SHA256::CalculateSha256(const unsigned char* text, size_t textlength)
	{
		unsigned char buffer[32];
		SHA256_Encrpyt(text, textlength, buffer);

		return buffer;
	}


}