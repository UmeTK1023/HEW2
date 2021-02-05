#pragma once

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT Weapon_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void Weapon_Finalize(void);
void Weapon_Update(void);
void Weapon_Draw(void);

int GetLightTable();
void SetLighTable(int n);


bool GetguardU();
bool GetguardR();
bool GetguardL();
bool Getavoid();
