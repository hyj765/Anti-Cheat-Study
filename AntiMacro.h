#pragma once
#include "stdafx.h"

namespace HYJ
{

	class AntiMacro
	{
	public:
		AntiMacro();

		~AntiMacro();

		static LRESULT CALLBACK AntiKeyBoardMacroHookFunction(int nCode, WPARAM wParam, LPARAM lParam);
		
		static LRESULT CALLBACK AntiMouseMacroHookFunction(int nCode, WPARAM wParam, LPARAM lParam);
		
		static bool SetAntiMacroMonitor();

		void StartMonitor();

		void UnSetAntiMacroMonitor();

		static HHOOK mouseMonitor;

		static HHOOK keyboardMonitor;

	private:
		std::thread::id monitorThread;
	};

}