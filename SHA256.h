#pragma once
#include <iostream>



namespace HYJ 
{
	class SHA256 {

	public:
		static std::unique_ptr<unsigned char[]> CalculateSha256(const unsigned char* text, size_t textlength);
	};

}