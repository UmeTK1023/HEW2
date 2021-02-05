#pragma once

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT EnemyWeapon_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void EnemyWeapon_Finalize(void);
void EnemyWeapon_Update(void);
void EnemyWeapon_Draw(void);

bool GetEatkU();
bool GetEatkR();
bool GetEatkL();
bool GetEguardU();
bool GetEguardR();
bool GetEguardL();
bool GetEavoid();
int GetELightTable();
int GetAtkAlpha();