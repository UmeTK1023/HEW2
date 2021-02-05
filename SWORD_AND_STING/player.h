#pragma once

#include "main.h"

#define	PLAYER_WIDTH		(20.0f)						// �n�ʂ̕�(X����)
#define	PLAYER_DEPTH		(10.0f)						// �n�ʂ̉��s(Z����)
#define  PLAYER_HEIGHT	(60.0f)					


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT Player_Initialize(void);
void Player_Finalize(void);
void Player_Update(void);
void Player_Draw(void);
D3DXVECTOR3 GetPosPlayer();
D3DXMATRIX GetMatrixPlayer();
D3DXVECTOR3 GetPdistance();