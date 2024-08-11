#pragma once
#include "stdafx.h"

namespace HYJ
{

	class AntiMacro
	{
	public:

		static LRESULT CALLBACK AntiKeyBoardMacroHookFunction(int nCode, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK AntiMouseMacroHookFunction(int nCode, WPARAM wParam, LPARAM lParam);
		bool SetAntiMacroMonitor();
		void UnSetAntiMacroMonitor();

		AntiMacro();
		~AntiMacro();
		static HHOOK mouseMonitor;
		static HHOOK keyboardMonitor;

	private:

		HANDLE MonitorThread = NULL;

	};
	
	HHOOK AntiMacro::mouseMonitor = nullptr;
	HHOOK AntiMacro::keyboardMonitor = nullptr;
}