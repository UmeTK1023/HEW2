#pragma once


#include "main.h"

#define	ENEMY_WIDTH		(20.0f)						// �n�ʂ̕�(X����)
#define	ENEMY_DEPTH		(10.0f)						// �n�ʂ̉��s(Z����)
#define  ENEMY_HEIGHT	(60.0f)					


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT Enemy_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void Enemy_Finalize(void);
void Enemy_Update(void);
void Enemy_Draw(void);
D3DXVECTOR3 GetPosEnemy();
D3DXVECTOR3 GetEdistance();