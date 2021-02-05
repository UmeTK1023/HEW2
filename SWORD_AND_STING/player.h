#pragma once

#include "main.h"

#define	PLAYER_WIDTH		(20.0f)						// 地面の幅(X方向)
#define	PLAYER_DEPTH		(10.0f)						// 地面の奥行(Z方向)
#define  PLAYER_HEIGHT	(60.0f)					


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT Player_Initialize(void);
void Player_Finalize(void);
void Player_Update(void);
void Player_Draw(void);
D3DXVECTOR3 GetPosPlayer();
D3DXMATRIX GetMatrixPlayer();
D3DXVECTOR3 GetPdistance();