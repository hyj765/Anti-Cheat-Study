#include "ThreadManager.h"
using namespace std;

void print() {

	printf("hellowrold");
}


int main()
{
	HYJ::ThreadManager::GetInstance().CreateThreads(print);


	return 0;
}