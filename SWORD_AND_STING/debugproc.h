
#pragma once

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT DebugProc_Initialize(void);
void DebugProc_Finalize(void);
void DebugProc_Update(void);
void DebugProc_Draw(void);

void DebugProc_Print(char *fmt, ...);
