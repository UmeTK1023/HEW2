//=============================================================================
//
// �T�E���h���� [sound.h]
//
//=============================================================================
#pragma once


#include <windows.h>
#include "xaudio2.h"      // �T�E���h�����ŕK�v


//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLE,      // �^�C�g��BGM
	SOUND_LABEL_BATTLE,     // �o�g��BGM
	SOUND_LABEL_VICTORY,    // ����BGM
	SOUND_LABEL_DEFEAT,     // �s�kBGM
	SOUND_LABEL_SE_ATTACK,  // �U����
	SOUND_LABEL_SE_GUARD,   // �K�[�h��
	SOUND_LABEL_SE_AVOID,   // �����
	SOUND_LABEL_SE_HIT,		// �q�b�g��
	SOUND_LABEL_SE_OK,		// ���艹

	SOUND_LABEL_MAX,

} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
bool InitSound(HWND hWnd);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);