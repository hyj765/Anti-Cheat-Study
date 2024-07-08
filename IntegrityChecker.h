#pragma once
#include <Windows.h>
#include<unordered_map>

namespace HYJ
{

	class IntegrityChecker
	{
	public:



	private:
		IntegrityChecker();
		~IntegrityChecker();
		std::unordered_map<std::string, std::string> hashList;
	};














}