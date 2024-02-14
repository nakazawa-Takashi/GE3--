#pragma once
#include"engine/Base/WinApp/WinApp.h"
#include <cassert>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Xinput.h>
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"dinput8.lib")

class Input{
public:


	static Input* GetInstance();

	static void Initialize();

	static void Update();

	bool PushKey(uint8_t keyNum);

	bool PressedKey(uint32_t keyNum);

	bool GetJoystickState(XINPUT_STATE& out) const;

	bool PressedButton(XINPUT_STATE& out, WORD button);

	//void SetJoystickDeadZone(int32_t deadZoneL, int32_t deadZoneR);

	struct ButtonState {
		bool isPressed;
		bool wasPressed;

		ButtonState() : isPressed(false), wasPressed(false) {}
	};

	void UpdateButtonState(ButtonState& state, bool isPressed);


private:

	IDirectInput8* directInput = nullptr;
	IDirectInputDevice8* keyboard = nullptr;

	BYTE keys[256] = {};
	BYTE preKeys[256] = {};

	bool isInitialize = false;

	Input::ButtonState state_;


	//0x80=押している状態
	//0x00=押してない状態
	Input() = default;
	~Input() = default;
	Input(const Input&) = delete;
	const Input& operator=(const Input&) = delete;
};