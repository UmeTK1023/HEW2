#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include <d3d9.h>
#include "sound.h"
#include "load.h"
#include "main.h"

static bool g_bEnd = false;
static int g_AbuttonAlpha;
static int FrameCount;
static bool Alphaflg;

D3DXVECTOR2 Apos;

void Load_Initialize(void)
{
	FrameCount = 0;
	g_AbuttonAlpha = 0;
	Alphaflg = false;
	Apos = D3DXVECTOR2(SCREEN_WIDTH * 0.89f, SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.17f));
	g_bEnd = false;
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 0));//‚æ‚­•ª‚©‚ç‚ñ‚ªƒtƒF[ƒh‚ðÁ‚·‚½‚ß‚É“ü‚ê‚é

}

void Load_Finalize(void)
{
}

void Load_Update(void)
{
	FrameCount++;

	// ƒ¿’l‚ª‚O‚ÌŽž‚ÉŒ©‚¦‚é‚æ‚¤‚É‚·‚é
	if (!Alphaflg)
	{
		g_AbuttonAlpha += 3.4;
		if (g_AbuttonAlpha >= 255)
		{
			Alphaflg = true;
			FrameCount = 0;
		}
	}

	//ƒ¿’l‚ª‚Q‚T‚T‚ÌŽž‚ÉŒ©‚¦‚È‚¢‚æ‚¤‚É‚·‚é
	if (Alphaflg)
	{
		g_AbuttonAlpha -= 3.4;
		if (g_AbuttonAlpha <= 0)
		{
			Alphaflg = false;
			FrameCount = 0;
		}
	}

	if (!g_bEnd) 
	{
		if (keyboard.IsTrigger(DIK_SPACE)||joycon[0].IsTrigger(JOYCON_A)) {
			PlaySound(SOUND_LABEL_SE_OK);
			Fade_Start(true, 30, D3DCOLOR_RGBA(0, 0, 0, 0));
			g_bEnd = true;
		}
	}
	else {
		if (!Fade_IsFade()) {
			Scene_Change(SCENE_INDEX_OPERATION);
		}
	}
}

void Load_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_LOAD, 0.0f, 0.0f);
	//Sprite_Draw(TEXTURE_INDEX_A,0.0f, 0.0f);

	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, g_AbuttonAlpha));

	//‚`ƒ{ƒ^ƒ“‚Ì•`‰æ
	Sprite_Draw(TEXTURE_INDEX_A,Apos.x ,Apos.y,
		0, 0, 142, 142,
		1.0f, 1.0f, 1.5f, 1.5f, 0.0f);

	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

}
