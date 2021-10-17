#pragma once

// STD Header Files
#include <memory>

// Windows Header Files
#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

class GraphicsWindow {
public:
	static GraphicsWindow &instance() {
		if (s_winInstance == nullptr)
			s_winInstance = new GraphicsWindow(0, 0, "test");
		return *s_winInstance;
	}

	bool processMessages() {
		MSG msg = {};
		while (PeekMessage(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				return false;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return true;
	}

	void destroy() {
		delete s_winInstance;
		s_winInstance = nullptr;
	}
private:
	GraphicsWindow(int bufferWidth, int bufferHeight, const char *path)
		: m_hInstance{GetModuleHandle(nullptr)} {

		WNDCLASS wndClass = {};
		wndClass.lpszClassName = CLASS_NAME;
		wndClass.hInstance = m_hInstance;
		wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.lpfnWndProc = WindowProc;

		RegisterClass(&wndClass);

		DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

		int width = 640;
		int height = 480;

		RECT rect;
		rect.left = 250;
		rect.top = 250;
		rect.right = rect.left + width;
		rect.bottom = rect.top + height;

		AdjustWindowRect(&rect, style, false);

		m_hWnd = CreateWindowEx(
			0,
			CLASS_NAME,
			L"Ray tracer",
			style,
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			NULL,
			NULL,
			m_hInstance,
			NULL
		);

		ShowWindow(m_hWnd, SW_SHOW);

		//// A working buffer for our display. Each pixel is stored as an unsigned 32-bit integer: alpha<<24 | red<<16 | green<<8 | blue
		//m_playBuffer.width = bufferWidth;
		//m_playBuffer.height = bufferHeight;
		//m_playBuffer.pPixels = new Pixel[static_cast<size_t>(bufferWidth) * bufferHeight];
		//m_playBuffer.preMultiplied = false;
	}

	~GraphicsWindow() {
		UnregisterClass(CLASS_NAME, m_hInstance);
	}

	const wchar_t *CLASS_NAME = L"Window";
	static GraphicsWindow *s_winInstance;

	HINSTANCE m_hInstance;
	HWND m_hWnd;
};
GraphicsWindow *GraphicsWindow::s_winInstance = nullptr;
