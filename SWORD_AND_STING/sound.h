//=============================================================================
//
// サウンド処理 [sound.h]
//
//=============================================================================
#pragma once


#include <windows.h>
#include "xaudio2.h"      // サウンド処理で必要


//*****************************************************************************
// サウンドファイル
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLE,      // タイトルBGM
	SOUND_LABEL_BATTLE,     // バトルBGM
	SOUND_LABEL_VICTORY,    // 勝利BGM
	SOUND_LABEL_DEFEAT,     // 敗北BGM
	SOUND_LABEL_SE_ATTACK,  // 攻撃音
	SOUND_LABEL_SE_GUARD,   // ガード音
	SOUND_LABEL_SE_AVOID,   // 回避音
	SOUND_LABEL_SE_HIT,		// ヒット音
	SOUND_LABEL_SE_OK,		// 決定音

	SOUND_LABEL_MAX,

} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);