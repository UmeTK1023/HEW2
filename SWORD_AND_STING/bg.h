
#pragma once

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT Bg_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void Bg_Finalize(void);
void Bg_Update(void);
void Bg_Draw(void);
