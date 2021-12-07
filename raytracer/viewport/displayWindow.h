#pragma once

// STD Header Files
#include <memory>

// Windows Header Files
#include <Windows.h>

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

class DisplayWindow {
public:
	static DisplayWindow &instance() {
		if (s_winInstance == nullptr) {
			s_winInstance = new DisplayWindow(0, 0, "test");
		}
		return *s_winInstance;

	}	
	
	static void createInstance(int width, int height) {
		if (s_winInstance == nullptr) {
			s_winInstance = new DisplayWindow(width, height, "test");
		}
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

	void drawPixel(Rgb pixel) {
		m_imageVect.push_back(pixel);
		m_imageVect;
	}

	void drawWindow() {
		// Set up a BitmapInfo structure to represent the pixel format of the display buffer
		BITMAPINFOHEADER bitmap_info_header
		{
				sizeof(BITMAPINFOHEADER),								// size of its own data,
				m_width, -m_height,		// width and height
				1, 32, BI_RGB,				// planes must always be set to 1 (docs), 32-bit pixel data, uncompressed 
				0, 0, 0, 0, 0				// rest can be set to 0 as this is uncompressed and has no palette
		};

		BITMAPINFO bitmap_info{bitmap_info_header, { 0,0,0,0 }};	// No palette data required for this bitmap

		// Write to display buffer
		uint32_t *buf = (uint32_t *)m_dBuf;
		for (const Rgb &pixel : m_imageVect) {
		uint32_t argb = (0xFF << 24) 
						| (static_cast<uint8_t>(pixel.x()) << 16)
						| (static_cast<uint8_t>(pixel.y()) << 8)
						| (static_cast<uint8_t>(pixel.z()));
		*buf = argb;
		++buf;
		}

		HDC hDC = GetDC(m_hWnd);
		StretchDIBits(hDC,
			0, 0, m_width, m_height, // Dest
			0, 0, m_width, m_height, // Src
			m_dBuf,
			&bitmap_info,
			DIB_RGB_COLORS,
			SRCCOPY);
	}

	double getAspectRation() const {
		return static_cast<double>(m_width) / static_cast<double>(m_height);
	}
private:
	DisplayWindow(int width, int height, const char *path)
		: m_width{width},
		  m_height{height},
		  m_hInstance {GetModuleHandle(nullptr)} {

		WNDCLASS wndClass = {};
		wndClass.lpszClassName = CLASS_NAME;
		wndClass.hInstance = m_hInstance;
		wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.lpfnWndProc = WindowProc;

		RegisterClass(&wndClass);

		DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

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

		m_dBuf = VirtualAlloc(
			0,
			m_width * m_height * sizeof(UINT32),
			MEM_RESERVE | MEM_COMMIT,
			PAGE_READWRITE
		);

		ShowWindow(m_hWnd, SW_SHOW);
	}

	~DisplayWindow() {
		UnregisterClass(CLASS_NAME, m_hInstance);
	}

	const wchar_t *CLASS_NAME = L"Window";
	static DisplayWindow *s_winInstance;
	void *m_dBuf;
	std::vector<Rgb> m_imageVect;
	const int m_width;
	const int m_height;
	HINSTANCE m_hInstance;
	HWND m_hWnd;
};
DisplayWindow *DisplayWindow::s_winInstance = nullptr;
