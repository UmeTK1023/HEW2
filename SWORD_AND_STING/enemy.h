#pragma once


#include "main.h"

#define	ENEMY_WIDTH		(20.0f)						// 地面の幅(X方向)
#define	ENEMY_DEPTH		(10.0f)						// 地面の奥行(Z方向)
#define  ENEMY_HEIGHT	(60.0f)					


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT Enemy_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void Enemy_Finalize(void);
void Enemy_Update(void);
void Enemy_Draw(void);
D3DXVECTOR3 GetPosEnemy();
D3DXVECTOR3 GetEdistance();