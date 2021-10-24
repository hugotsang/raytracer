#pragma once

class DisplayBuffer {
public:
	static DisplayBuffer &instance() {
		if (s_disBufInstance == nullptr) {
			s_disBufInstance = new DisplayBuffer(0, 0);
		}
		return *s_disBufInstance;

	}

	static void createInstance(int width, int height) {
		if (s_disBufInstance == nullptr) {
			s_disBufInstance = new DisplayBuffer(width, height);
		}
	}

	int getWidth() const {
		return m_width;
	}

	int getHeight() const {
		return m_height;
	}

private:
	const int m_width;
	const int m_height;
	static DisplayBuffer *s_disBufInstance;
	DisplayBuffer(int width, int height)
		: m_width{width},
		  m_height{height}  {}
};

DisplayBuffer *DisplayBuffer::s_disBufInstance = nullptr;