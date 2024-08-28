#include "ProcessProtector.h"
#include "AntiDllInjector.h"
using namespace std;


int main()
{

	HYJ::ProcessProtector::GetInstance().HookLoadLibrary(HYJ::DllInjectionChecker::WhiteListLoadLibrary);

	return 0;
}