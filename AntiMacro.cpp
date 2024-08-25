#include "AntiMacro.h"

namespace HYJ {

	HHOOK AntiMacro::mouseMonitor = nullptr;

	HHOOK AntiMacro::keyboardMonitor = nullptr;

	std::atomic<bool> AntiMacro::doneFlag =false ;

	AntiMacro::AntiMacro() {};

	AntiMacro::~AntiMacro()
	{
		UnSetAntiMacroMonitor();
	}

	LRESULT CALLBACK AntiMacro::AntiKeyBoardMacroHookFunction(int nCode, WPARAM wParam, LPARAM lParam)
	{

		if (nCode == HC_ACTION)
		{
			if(wParam == WM_KEYDOWN)
			{
				KBDLLHOOKSTRUCT* keyBoardStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
				
				if ((keyBoardStruct->flags & LLKHF_INJECTED) || (keyBoardStruct->flags & LLKHF_LOWER_IL_INJECTED))
				{
					return true;
				}
			}
		}

		
		return CallNextHookEx(keyboardMonitor, nCode, wParam, lParam);
	}

	LRESULT CALLBACK AntiMacro::AntiMouseMacroHookFunction(int nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == HC_ACTION)
		{
			if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN)
			{
				KBDLLHOOKSTRUCT* mouseStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

				if ((mouseStruct->flags & LLKHF_INJECTED) || (mouseStruct->flags & LLKHF_LOWER_IL_INJECTED))
				{
					return true;
				}
			}
		}

		return CallNextHookEx(mouseMonitor, nCode, wParam, lParam);
	}

	bool AntiMacro::SetAntiMacroMonitor()
	{
		if ((mouseMonitor = SetWindowsHookExA(WH_MOUSE_LL, AntiMouseMacroHookFunction, NULL, 0)) == nullptr)
		{
			DEBUG_LOG("Anti Macro", "SetAntiMouseMacro Hook Fail");
			return false;
		}

		if ((keyboardMonitor = SetWindowsHookExA(WH_KEYBOARD_LL, AntiKeyBoardMacroHookFunction, NULL, 0)) == nullptr)
		{
			DEBUG_LOG("Anti Macro", "SetAntiKeyBoardMacro Hook Fail");
			return false;
		}


		MSG message;
		while (doneFlag.load() == false && GetMessageA(&message, NULL, 0, 0))
		{
				TranslateMessage(&message);
				DispatchMessageA(&message);

		}


		return true;
	}

	void AntiMacro::StartMonitor()
	{
		doneFlag.store(false);

		monitorThread = ThreadManager::GetInstance().CreateThreads(SetAntiMacroMonitor);
	}

	void AntiMacro::UnSetAntiMacroMonitor()
	{
		if (mouseMonitor)
		{
			UnhookWindowsHookEx(mouseMonitor);
			mouseMonitor = nullptr;
		}

		if (keyboardMonitor)
		{
			UnhookWindowsHookEx(keyboardMonitor);
			keyboardMonitor = nullptr;
		}
	}

	void AntiMacro::QuitMonitor()
	{
		doneFlag.store(true);
	}


}