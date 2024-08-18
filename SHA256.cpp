#include "KISA_SHA256.h"
#include "SHA256.h"


namespace HYJ
{

	
	std::unique_ptr<unsigned char[]> SHA256::CalculateSha256(const unsigned char* text, size_t textlength)
	{	
		std::unique_ptr<unsigned char[]> buffer = std::make_unique<unsigned char[]>(36);
		SHA256_Encrpyt(text, textlength, buffer.get());

		return buffer;
	}


}