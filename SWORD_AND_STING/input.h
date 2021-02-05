//
//#pragma once
//
//#include <Windows.h>
//#define DIRECTINPUT_VERSION (0x0800)
//#include <dinput.h>
//#include <string>
//
///* game pad情報 */
//#define BUTTON_UP		0x00000001l	// 方向キー上(.IY<0)
//#define BUTTON_DOWN		0x00000002l	// 方向キー下(.IY>0)
//#define BUTTON_LEFT		0x00000004l	// 方向キー左(.IX<0)
//#define BUTTON_RIGHT	0x00000008l	// 方向キー右(.IX>0)
//#define BUTTON_A		0x00000010l	// Ａボタン(.rgbButtons[0]&0x80)
//#define BUTTON_B		0x00000020l	// Ｂボタン(.rgbButtons[1]&0x80)
//#define BUTTON_C		0x00000040l	// Ｃボタン(.rgbButtons[2]&0x80)
//#define BUTTON_X		0x00000080l	// Ｘボタン(.rgbButtons[3]&0x80)
//#define BUTTON_Y		0x00000100l	// Ｙボタン(.rgbButtons[4]&0x80)
//#define BUTTON_Z		0x00000200l	// Ｚボタン(.rgbButtons[5]&0x80)
//#define BUTTON_L		0x00000400l	// Ｌボタン(.rgbButtons[6]&0x80)
//#define BUTTON_R		0x00000800l	// Ｒボタン(.rgbButtons[7]&0x80)
//#define BUTTON_START	0x00001000l	// ＳＴＡＲＴボタン(.rgbButtons[8]&0x80)
//#define BUTTON_M		0x00002000l	// Ｍボタン(.rgbButtons[9]&0x80)
//#define GAMEPADMAX		4			// 同時に接続するジョイパッドの最大数をセット
//
//
//bool Keyboard_Initialize(HINSTANCE hInstance, HWND hWnd);
//void Keyboard_Finalize(void);
//void Keyboard_Update(void);
//
//bool Keyboard_IsPress(int nKey);
//bool Keyboard_IsTrigger(int nKey);
//bool Keyboard_IsRelease(int nKey);
//
////---------------------------- game pad
//bool GamePad_Initialize(HINSTANCE hInstance, HWND hWnd);
//void GamePad_Finalize(void);
//void GamePad_Update(void);
//
//BOOL GamePad_IsPress(int padNo, DWORD button);
//BOOL GamePad_IsTrigger(int padNo, DWORD button);
//
//using namespace std;
//
//// キー最大値
//#define	NUM_KEY_MAX			(256)
//
//// JoyCon 最大値
//#define JOYCON_MAX 2
//
//// JoyConインデックス
//#define LEFT_JOYCON 0
//#define RIGHT_JOYCON 1
//
//// Joyconスティック
//#define JOYCON_STICK_DOWN 0x00004000l  
//#define JOYCON_STICK_UP 0x00008000l  
//#define JOYCON_STICK_RIGHT 0x00016000l  
//#define JOYCON_STICK_LEFT 0x00032000l 
//
//// JoyCon左
//#define JOYCON_DOWN 0x00000001l  
//#define JOYCON_UP 0x00000002l 
//#define JOYCON_RIGHT 0x00000004l 
//#define JOYCON_LEFT 0x00000008l 
//#define JOYCON_SR_LEFT 0x00000010l 
//#define JOYCON_SL_LEFT 0x00000020l 
//#define JOYCON_L 0x00000040l 
//#define JOYCON_ZL 0x00000080l 
//#define JOYCON_MIN 0x000000100l
//#define JOYCON_L3  0x00000800l 
//#define JOYCON_SCREENSHOT 0x00002000l 
//
//// JoyCon右
//#define JOYCON_Y 0x00000001l 
//#define JOYCON_X 0x00000002l 
//#define JOYCON_B 0x00000004l  
//#define JOYCON_A 0x00000008l 
//#define JOYCON_SR_RIGHT 0x000000010l  
//#define JOYCON_SL_RIGHT 0x000000020l  
//#define JOYCON_R  0x000000040l 
//#define JOYCON_ZR  0x000000080l 
//#define JOYCON_PLUS 0x000000200l
//#define JOYCON_R3 0x000000400l 
//#define JOYCON_HOME   0x0000001000l 
//
//class Keyboard {
//
//private:
//
//	// デバイス
//	LPDIRECTINPUTDEVICE8 DevKeyboard = NULL;
//	// キー状態
//	BYTE	 KeyState[NUM_KEY_MAX];
//	// キー状態（Trigger）
//	BYTE	 KeyStateTrigger[NUM_KEY_MAX];
//	// キー状態（Release）
//	BYTE	 KeyStateRelease[NUM_KEY_MAX];
//
//public:
//
//	bool Initialize(HINSTANCE hInstance, HWND hWnd);
//
//	void Finalize(void);
//
//	void Update();
//
//	bool IsPress(int nKey);
//
//	bool IsTrigger(int nKey);
//
//	bool IsRelease(int nKey);
//};
//
//class JoyCon {
//
//private:
//
//	// 加速度
//	DWORD	 Accelerometer;
//	// ジャイロ(x,y,z)
//	DWORD Gyro[3];
//	// ボタン状態
//	DWORD State;
//	// ボタン状態（Trigger）
//	DWORD Trigger;
//
//public:
//
//	LPDIRECTINPUTDEVICE8 Device = NULL;
//
//	bool Initialize(HINSTANCE hInstance, HWND hWnd);
//
//	void Finalize();
//
//	void Update();
//
//	BOOL IsPress(DWORD button);
//
//	BOOL IsTrigger(DWORD button);
//
//	DWORD GetTrigger();
//
//	// 加速度設定
//	void SetAccelerometer(DWORD data);
//	// ジャイロ設定
//	void SetGyro(DWORD x, DWORD y, DWORD z);
//
//	// GetジャイロX
//	int GetGyro_X();
//	// GetジャイロY
//	int GetGyro_Y();
//	// GetジャイロZ
//	int GetGyro_Z();
//	// Get加速度
//	DWORD GetAccelerometer();
//};
//
//// キーボード
//extern Keyboard keyboard;
//// JoyCon
//extern JoyCon joycon[JOYCON_MAX];

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

#pragma once

#include <Windows.h>
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <string>

using namespace std;

// キー最大値
#define	NUM_KEY_MAX			(256)

// JoyCon 最大値
#define JOYCON_MAX 2

// JoyConインデックス
#define LEFT_JOYCON 0
#define RIGHT_JOYCON 1

// Joyconスティック
#define JOYCON_STICK_DOWN 0x00004000l  
#define JOYCON_STICK_UP 0x00008000l  
#define JOYCON_STICK_RIGHT 0x00010000l  
#define JOYCON_STICK_LEFT 0x00020000l 

// JoyCon左
#define JOYCON_DOWN 0x00000001l  
#define JOYCON_UP 0x00000002l 
#define JOYCON_RIGHT 0x00000004l 
#define JOYCON_LEFT 0x00000008l 
#define JOYCON_SR_LEFT 0x00000010l 
#define JOYCON_SL_LEFT 0x00000020l 
#define JOYCON_L 0x00000040l 
#define JOYCON_ZL 0x00000080l 
#define JOYCON_MIN 0x000000100l
#define JOYCON_L3  0x00000800l 
#define JOYCON_SCREENSHOT 0x00002000l 

// JoyCon右
#define JOYCON_Y 0x00000001l 
#define JOYCON_X 0x00000002l 
#define JOYCON_B 0x00000004l  
#define JOYCON_A 0x00000008l 
#define JOYCON_SR_RIGHT 0x000000010l  
#define JOYCON_SL_RIGHT 0x000000020l  
#define JOYCON_R  0x000000040l 
#define JOYCON_ZR  0x000000080l 
#define JOYCON_PLUS 0x000000200l
#define JOYCON_R3 0x000000400l 
#define JOYCON_HOME   0x0000001000l 

class Keyboard {

private:

	// デバイス
	LPDIRECTINPUTDEVICE8 DevKeyboard = NULL;
	// キー状態
	BYTE	 KeyState[NUM_KEY_MAX];
	// キー状態（Trigger）
	BYTE	 KeyStateTrigger[NUM_KEY_MAX];
	// キー状態（Release）
	BYTE	 KeyStateRelease[NUM_KEY_MAX];

public:

	bool Initialize(HINSTANCE hInstance, HWND hWnd);

	void Finalize(void);

	void Update();

	bool IsPress(int nKey);

	bool IsTrigger(int nKey);

	bool IsRelease(int nKey);
};

class JoyCon {

private:

	// 加速度
	DWORD	 Accelerometer;
	// ジャイロ(x,y,z)
	DWORD Gyro[3];
	// ボタン状態
	DWORD State;
	// ボタン状態（Trigger）
	DWORD Trigger;

	//スティック状態//
	LONG Stick_X;
	LONG Stick_Y;

	//スティックの遊び幅(最大2000.0)//
	static float Stick_play;

public:

	LPDIRECTINPUTDEVICE8 Device = NULL;

	bool Initialize(HINSTANCE hInstance, HWND hWnd);

	void Finalize();

	void Update();

	BOOL IsPress(DWORD button);

	BOOL IsTrigger(DWORD button);

	DWORD GetTrigger();

	// 加速度設定
	void SetAccelerometer(DWORD data);
	// ジャイロ設定
	void SetGyro(DWORD x, DWORD y, DWORD z);

	// GetジャイロX
	float GetGyro_X();
	// GetジャイロY
	float GetGyro_Y();
	// GetジャイロZ
	float GetGyro_Z();
	// Get加速度
	DWORD GetAccelerometer();

	// スティク入力の数値取得//
	float GetStick_X();
	float GetStick_Y();		//(Stick_play / 20)%の遊びを入れて、-1~1に正規化してfloat型でスティック入力の値を返す関数//
};

// キーボード
extern Keyboard keyboard;
// JoyCon
extern JoyCon joycon[JOYCON_MAX];

