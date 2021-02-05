

#include "main.h"
#include "myDirect3D.h"


typedef struct FadeVertex_tag
{
	D3DXVECTOR4 position;
	D3DCOLOR color;
} FadeVertex;
#define FVF_FADE_VERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)


static D3DXCOLOR g_FadeColor(0.0f, 0.0f, 0.0f, 1.0f);
static float g_Alpha = 0.0f;
static float g_AddAlpha = 0.0f;
static bool g_bOut = false;
static bool g_bIsFade = false;


void Fade_Initialize(void)
{
	g_bIsFade = false;
	g_Alpha = 0.0f;
}

void Fade_Finalize(void)
{
}

void Fade_Update(void)
{
	if( !g_bIsFade ) {
		return;
	}

	g_Alpha += g_AddAlpha;

	if( g_bOut ) {
		if( g_Alpha >= 1.0f ) {
			g_Alpha = 1.0f;
			g_bIsFade = false;
		}
	}
	else {
		if( g_Alpha <= 0.0f ) {
			g_Alpha = 0.0f;
			g_bIsFade = false;
		}
	}
}

void Fade_Draw(void)
{
	if( g_Alpha == 0.0f ) {
		return;
	}

	g_FadeColor.a = g_Alpha;
	D3DCOLOR c = g_FadeColor;

	FadeVertex v[] = {
		{ D3DXVECTOR4(        0.0f,          0.0f, 0.0f, 1.0f), c }, 
		{ D3DXVECTOR4(SCREEN_WIDTH,          0.0f, 0.0f, 1.0f), c }, 
		{ D3DXVECTOR4(        0.0f, SCREEN_HEIGHT, 0.0f, 1.0f), c }, 
		{ D3DXVECTOR4(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 1.0f), c }, 
	};

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	pDevice->SetFVF(FVF_FADE_VERTEX);
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(FadeVertex));
}

void Fade_Start(bool bOut, int frame, D3DCOLOR color)
{
	g_bOut = bOut;
	g_AddAlpha = 1.0f / frame;
	g_FadeColor = color;
	g_bIsFade = true;

	if( g_bOut ) {
		g_Alpha = 0.0f;
	}
	else {
		g_Alpha = 1.0f;
		g_AddAlpha = -g_AddAlpha;
	}
}

bool Fade_IsFade(void)
{
	return g_bIsFade;
}
