#pragma once

#include <d3d9.h>

void Fade_Initialize(void);
void Fade_Finalize(void);
void Fade_Update(void);
void Fade_Draw(void);

void Fade_Start(bool bOut, int frame, D3DCOLOR color);
bool Fade_IsFade(void);

