#pragma once

// Windows Header Files
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>

class Window {
public:
	static Window &instance() {
		static auto s_windowInstance = new Window();
		return s_winInstance;
	}
private:
	Window() {
	}
}