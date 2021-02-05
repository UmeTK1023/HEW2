#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include "ui.h"
#include "sound.h"
#include "main.h"

#define PLAYER_SCORE_SIZE (567);
#define ENEMY_SCORE_SIZE  (567);

static int g_AbuttonAlpha;
static int FrameCount;
static bool Alphaflg;

D3DXVECTOR2 Apos3;
D3DXVECTOR2 PSpos;
D3DXVECTOR2 ESpos;


void Result_Initialize(void)
{
	FrameCount = 0;
	g_AbuttonAlpha = 0;
	Alphaflg = false;
	Apos3 = D3DXVECTOR2(SCREEN_WIDTH * 0.89f, SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.17f));
	PSpos = D3DXVECTOR2(SCREEN_WIDTH * 0.06f, SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.77f));
	ESpos = D3DXVECTOR2(SCREEN_WIDTH * 0.53f, SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.77f));

	if (GetPlayerScore() >= 5 || GetPlayerScore() >= GetEnemyScore())
	{
		PlaySound(SOUND_LABEL_VICTORY);
	}
	else if (GetEnemyScore() >= 5 || GetEnemyScore() >= GetPlayerScore())
	{
		PlaySound(SOUND_LABEL_DEFEAT);
	}
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
}

void Result_Finalize(void)
{

}

void Result_Update(void)
{

	FrameCount++;

	// α値が０の時に見えるようにする
	if (!Alphaflg)
	{
		g_AbuttonAlpha += 3.4;
		if (g_AbuttonAlpha >= 255)
		{
			Alphaflg = true;
			FrameCount = 0;
		}
	}

	//α値が２５５の時に見えないようにする
	if (Alphaflg)
	{
		g_AbuttonAlpha -= 3.4;
		if (g_AbuttonAlpha <= 0)
		{
			Alphaflg = false;
			FrameCount = 0;
		}
	}

	if(keyboard.IsTrigger(DIK_SPACE) || joycon[0].IsTrigger(JOYCON_A)) {
		if (GetPlayerScore() >= 5 || GetPlayerScore() >= GetEnemyScore())
		{
			PlaySound(SOUND_LABEL_SE_OK);
			Scene_Change(SCENE_INDEX_CLEAR);
		}
		else if (GetEnemyScore() >= 5 || GetEnemyScore() >= GetPlayerScore())
		{
			PlaySound(SOUND_LABEL_SE_OK);
			Scene_Change(SCENE_INDEX_OVER);
		}
	}
}

void Result_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_RESULT, 0.0f, 0.0f);

	//プレイヤースコアの描画
	Sprite_Draw(TEXTURE_INDEX_SCORE, PSpos.x, PSpos.y,
		GetPlayerScore()*567, 0, 567, 567,
		1.0f, 1.0f, 1.3f, 1.3f, 0.0f);

	//エネミースコアの描画
	Sprite_Draw(TEXTURE_INDEX_SCORE, ESpos.x, ESpos.y,
		GetEnemyScore() * 567, 0, 567, 567,
		1.0f, 1.0f, 1.3f, 1.3f, 0.0f);

	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, g_AbuttonAlpha));

	//Ａボタンの描画
	Sprite_Draw(TEXTURE_INDEX_A, Apos3.x, Apos3.y,
		0, 0, 142, 142,
		1.0f, 1.0f, 1.5f, 1.5f, 0.0f);

	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
}
