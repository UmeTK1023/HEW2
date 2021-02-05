
/*
Copyright <2019> <katsurakanade>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "input.h"

// JoyCon用リミット
#define DEADZONE		50	
#define RANGE_MAX		2000			
#define RANGE_MIN		-2000		

static bool Initialize(HINSTANCE hInstance);
static void Finalize(void);

LPDIRECTINPUT8			g_pInput = NULL;
Keyboard keyboard;
JoyCon joycon[JOYCON_MAX];

// Joycon数
static int					g_padCount = 0;

float			 JoyCon::Stick_play = 200.0f;

bool initialize(HINSTANCE hInstance)
{

	if (g_pInput == NULL) {
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL))) {
			return false;
		}
	}

	return true;
}

void finalize(void)
{
	if (g_pInput != NULL) {
		g_pInput->Release();
		g_pInput = NULL;
	}
}

BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pInput->CreateDevice(lpddi->guidInstance, &joycon[g_padCount++].Device, NULL);

	return DIENUM_CONTINUE;
}

bool Keyboard::Initialize(HINSTANCE hInstance, HWND hWnd)
{

	try
	{
		if (!initialize(hInstance)) {
			throw "DirectInput Object Not Found ";
			return false;
		}
		if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &this->DevKeyboard, NULL)))
		{
			throw "Keyboard NotFound";
			return false;
		}
		if (FAILED(this->DevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		{
			throw "Keyboard DataFormat Setting Failed";
			return false;
		}
		if (FAILED(this->DevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			throw "Keyboard CooperativeLevel Setting Failed";
			return false;
		}
	}
	catch (char *error)
	{
		MessageBox(hWnd, error, "WARNING！", MB_ICONWARNING);
	}

	this->DevKeyboard->Acquire();

	return true;
}

void Keyboard::Finalize(void)
{
	if (DevKeyboard != NULL)
	{
		DevKeyboard->Unacquire();
		DevKeyboard->Release();
		DevKeyboard = NULL;
	}

	finalize();
}

void Keyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	if (SUCCEEDED(DevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		for (int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			KeyStateTrigger[nCnKey] = (KeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			KeyStateRelease[nCnKey] = (KeyState[nCnKey] ^ aKeyState[nCnKey]) & KeyState[nCnKey];

			KeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		DevKeyboard->Acquire();
	}
}

bool Keyboard::IsPress(int nKey)
{
	return (KeyState[nKey] & 0x80) ? true : false;
}

bool Keyboard::IsTrigger(int nKey)
{
	return (KeyStateTrigger[nKey] & 0x80) ? true : false;
}

bool Keyboard::IsRelease(int nKey)
{
	return (KeyStateRelease[nKey] & 0x80) ? true : false;
}

bool JoyCon::Initialize(HINSTANCE hInstance, HWND hWnd)
{

	if (!initialize(hInstance)) {

		MessageBox(hWnd, "DirectInput Object Not Found ", "WARNING！", MB_ICONWARNING);
		return false;
	}

	HRESULT		result;

	g_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);

	result = Device->SetDataFormat(&c_dfDIJoystick);

	for (int i = 0; i < JOYCON_MAX; i++) {

		if (FAILED(result))
			return false;

		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;

		// X軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_X;
		joycon[i].Device->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Y;
		joycon[i].Device->SetProperty(DIPROP_RANGE, &diprg.diph);

		// Rz軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_RZ;
		joycon[i].Device->SetProperty(DIPROP_RANGE, &diprg.diph);

		// Slider1の範囲を設定
		diprg.diph.dwObj = DIJOFS_SLIDER(0);
		joycon[i].Device->SetProperty(DIPROP_RANGE, &diprg.diph);

		// Slider2の範囲を設定
		diprg.diph.dwObj = DIJOFS_SLIDER(1);
		joycon[i].Device->SetProperty(DIPROP_RANGE, &diprg.diph);

		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		//X軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_X;
		joycon[i].Device->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_Y;
		joycon[i].Device->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		// Rz軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_RZ;
		joycon[i].Device->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		// Slider1の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_SLIDER(0);
		joycon[i].Device->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		// Slider2の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_SLIDER(1);
		joycon[i].Device->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		joycon[i].Device->Acquire();
	}

	return true;
}

void JoyCon::Finalize()
{

	if (Device)
	{
		Device->Unacquire();
		Device->Release();
	}

	finalize();
}

void JoyCon::Update()
{
	HRESULT			result;
	DIJOYSTATE2		dijs;

	DWORD lastPadState;
	lastPadState = State;
	State = 0x00000000l;

	result = Device->Poll();

	if (FAILED(result)) {
		result = Device->Acquire();
		while (result == DIERR_INPUTLOST)
			result = Device->Acquire();
	}

	result = Device->GetDeviceState(sizeof(DIJOYSTATE), &dijs);
	if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
		result = Device->Acquire();
		while (result == DIERR_INPUTLOST)
			result = Device->Acquire();
	}

	// JoyCon
	if (dijs.lY >= Stick_play) Stick_Y = dijs.lY;//State |= JOYCON_STICK_UP;
	if (dijs.lY <= -Stick_play) Stick_Y = dijs.lY;//State |= JOYCON_STICK_DOWN;
	if (dijs.lX <= -Stick_play) Stick_X = dijs.lX;//State |= JOYCON_STICK_LEFT;
	if (dijs.lX >= Stick_play) Stick_X = dijs.lX;//State |= JOYCON_STICK_RIGHT;

	if (-Stick_play < dijs.lY && dijs.lY < Stick_play) Stick_Y = 0.0;
	if (-Stick_play < dijs.lX && dijs.lX < Stick_play) Stick_X = 0.0;

	if (dijs.rgbButtons[0] & 0x80)	State |= JOYCON_DOWN;
	if (dijs.rgbButtons[1] & 0x80)	State |= JOYCON_UP;
	if (dijs.rgbButtons[2] & 0x80)	State |= JOYCON_RIGHT;
	if (dijs.rgbButtons[3] & 0x80)	State |= JOYCON_LEFT;
	if (dijs.rgbButtons[4] & 0x80)  State |= JOYCON_SR_LEFT;
	if (dijs.rgbButtons[5] & 0x80)	State |= JOYCON_SL_LEFT;
	if (dijs.rgbButtons[6] & 0x80)  State |= JOYCON_L;
	if (dijs.rgbButtons[7] & 0x80)	State |= JOYCON_ZL;
	if (dijs.rgbButtons[8] & 0x80)	State |= JOYCON_MIN;
	if (dijs.rgbButtons[11] & 0x80) State |= JOYCON_L3;
	if (dijs.rgbButtons[13] & 0x80)	State |= JOYCON_SCREENSHOT;
	if (dijs.rgbButtons[16] & 0x80)	State |= JOYCON_Y;
	if (dijs.rgbButtons[17] & 0x80)	State |= JOYCON_X;
	if (dijs.rgbButtons[18] & 0x80)	State |= JOYCON_B;
	if (dijs.rgbButtons[19] & 0x80)	State |= JOYCON_A;
	if (dijs.rgbButtons[20] & 0x80)	State |= JOYCON_SR_RIGHT;
	if (dijs.rgbButtons[21] & 0x80)	State |= JOYCON_SL_RIGHT;
	if (dijs.rgbButtons[22] & 0x80)	State |= JOYCON_R;
	if (dijs.rgbButtons[23] & 0x80)	State |= JOYCON_ZR;
	if (dijs.rgbButtons[25] & 0x80)	State |= JOYCON_PLUS;
	if (dijs.rgbButtons[26] & 0x80)	State |= JOYCON_R3;
	if (dijs.rgbButtons[28] & 0x80)	State |= JOYCON_HOME;

	// 加速度設定
	SetAccelerometer(dijs.lRz);
	// ジャイロ設定
	SetGyro(dijs.lRz, dijs.rglSlider[0], dijs.rglSlider[1]);

	// Trigger設定
	Trigger = ((lastPadState ^ State)	& State);

}

void JoyCon::SetAccelerometer(DWORD data) {
	Accelerometer = data;
}

void JoyCon::SetGyro(DWORD x, DWORD y, DWORD z) {
	Gyro[0] = x;
	Gyro[1] = y;
	Gyro[2] = z;
}

float JoyCon::GetGyro_X() {
	return ((int)Gyro[0]) * 0.0005f;
}

float JoyCon::GetGyro_Y() {
	return ((int)Gyro[1]) * 0.0005f;
}

float JoyCon::GetGyro_Z() {
	return ((int)Gyro[2]) * 0.0005f;
}

DWORD JoyCon::GetAccelerometer() {
	return Accelerometer;
}

DWORD JoyCon::GetTrigger() {
	return Trigger;
}

BOOL JoyCon::IsPress(DWORD button)
{
	return (button & State);
}

BOOL JoyCon::IsTrigger(DWORD button)
{
	return (button & Trigger);
}

float JoyCon::GetStick_X()
{
	float stick_x;
	if (Stick_X == 0.0)
		return 0.0f;

	if (Stick_X > 0.0)
		stick_x = ((float)(Stick_X - Stick_play) / (float)(2000.0 - Stick_play));
	else
		stick_x = -((float)(Stick_X + Stick_play) / (float)(-2000.0 + Stick_play));

	return stick_x;
}
float JoyCon::GetStick_Y()
{
	float stick_y;
	if (Stick_Y == 0.0)
		return 0.0f;

	if (Stick_Y > 0.0)
		stick_y = ((float)(Stick_Y - Stick_play) / (float)(2000.0 - Stick_play));
	else
		stick_y = -((float)(Stick_Y + Stick_play) / (float)(-2000.0 + Stick_play));

	return stick_y;
}
