#pragma once
#include "stdafx.h"

namespace HYJ
{

	class AntiMacro
	{
	public:

		static LRESULT CALLBACK AntiKeyBoardMacroHookFunction(int nCode, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK AntiMouseMacroHookFunction(int nCode, WPARAM wParam, LPARAM lParam);
		
		static bool SetAntiMacroMonitor();
		void StartMonitor();
		void UnSetAntiMacroMonitor();

		AntiMacro();
		~AntiMacro();

		static HHOOK mouseMonitor;
		static HHOOK keyboardMonitor;

	private:
		std::thread::id monitorThread;
	};
	
	HHOOK AntiMacro::mouseMonitor = nullptr;
	HHOOK AntiMacro::keyboardMonitor = nullptr;
}