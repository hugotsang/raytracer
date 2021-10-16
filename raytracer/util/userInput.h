#pragma once
#include <windows.h>

class UserInput {
public:
	static UserInput &instance() {
		static auto s_inputInstance = new UserInput();
		return *s_inputInstance;
	}

	bool KeyDown(int vKey) {
		return GetAsyncKeyState(vKey) & 0x8000; // Don't want multiple calls to KeyState
	}

private:
	UserInput() {
	}
};
