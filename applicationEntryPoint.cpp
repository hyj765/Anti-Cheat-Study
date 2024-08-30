#include "ProcessProtector.h"
using namespace std;


int main()
{
	if (HYJ::ProcessProtector::GetInstance().CheckHardWareBreakPoint() == false)
	{
		printf("no");
	}

	return 0;
}