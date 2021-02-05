#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include <d3d9.h>
#include "sound.h"

static int g_StartAlpha;
static int FrameCount;
static bool Alphaflg;

static bool g_bEnd = false;


void Title_Initialize(void)
{
	FrameCount = 0;
	g_StartAlpha = 0;
	Alphaflg = false;
	PlaySound(SOUND_LABEL_TITLE);
	g_bEnd = false;
}

void Title_Finalize(void)
{
}

void Title_Update(void)
{
	FrameCount++;

	// ƒ¿’l‚ª‚O‚ÌŽž‚ÉŒ©‚¦‚é‚æ‚¤‚É‚·‚é
	if (!Alphaflg)
	{
		g_StartAlpha += 3.4;
		if (g_StartAlpha >= 255)
		{
			Alphaflg = true;
			FrameCount = 0;
		}
	}

	//ƒ¿’l‚ª‚Q‚T‚T‚ÌŽž‚ÉŒ©‚¦‚È‚¢‚æ‚¤‚É‚·‚é
	if (Alphaflg)
	{
		g_StartAlpha -= 3.4;
		if (g_StartAlpha <= 0)
		{
			Alphaflg = false;
			FrameCount = 0;
		}
	}

	if (!g_bEnd) {
		if (keyboard.IsTrigger(DIK_SPACE) || joycon[0].IsTrigger(JOYCON_A)) {
			PlaySound(SOUND_LABEL_SE_OK);
			Fade_Start(true, 30, D3DCOLOR_RGBA(0, 0, 0, 0));
			g_bEnd = true;
		}
	}
		else {
			if (!Fade_IsFade()) {
				Scene_Change(SCENE_INDEX_LOAD);
			}
		}
}

void Title_Draw(void)
{

	Sprite_Draw(TEXTURE_INDEX_TITLE, 0.0f, 0.0f);

	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, g_StartAlpha));

	//‚`ƒ{ƒ^ƒ“ƒXƒ^[ƒg‚Ì•`‰æ
	Sprite_Draw(TEXTURE_INDEX_START, 0, 0,
		0, 0, 1920, 1080,
		1.0f, 1.0f, 1.0f, 1.0f, 0.0f);

	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

}
