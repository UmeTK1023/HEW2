
#include "player.h"
#include "weapon.h"
#include "enemy.h"
#include "enemyweapon.h"
#include "fade.h"
#include "scene.h"
#include "camera.h"
#include "ui.h"
#include "bg.h"
#include "sound.h"
#include "field.h"

typedef enum PHASE_INDEX
{
	PHASE_INDEX_FADE,
	PHASE_INDEX_GAME,
	PHASE_INDEX_STAGE_CLEAR,
	PHASE_INDEX_END,

	PHASE_INDEX_MAX
};

static PHASE_INDEX g_Phase;
static int g_GameFrameCount;

static bool Game_EndCheck(void);



void Game_Initialize(void)
{
	PlaySound(SOUND_LABEL_BATTLE);
	Bg_Initialize(D3DXVECTOR3(0.0f, -15.0f, 80.0f), D3DXVECTOR3(0.0f, 15.7f, 0.0f));
	Player_Initialize();
	Weapon_Initialize(D3DXVECTOR3(20.0f, 15.0f, 0.0f), D3DXVECTOR3(0.0f, 15.7f, 0.0f));
	Enemy_Initialize(D3DXVECTOR3(0.0f,35.0f,30.0f), D3DXVECTOR3(0.0f, 0.0f,0.0f));
	EnemyWeapon_Initialize(D3DXVECTOR3(-16.0f, 20.0f, 35.0f), D3DXVECTOR3(0.0f, -0.1f, 0.0f));
	// 地面の初期化処理
	Field_Initialize();
	Ui_Initialize();
	// カメラの初期化処理
	Camera_Initialize();
	g_Phase = PHASE_INDEX_FADE;
	g_GameFrameCount = 0;
	Fade_Start(false, 30, D3DCOLOR_RGBA(0, 0, 0, 0));
}

void Game_Finalize(void)
{

	// カメラの終了処理
	Camera_Finalize();
	Bg_Finalize();
	Field_Finalize();
	Player_Finalize();
	Weapon_Finalize();
	Enemy_Finalize();
	EnemyWeapon_Finalize();
	Ui_Finalize();
	StopSound();
}

void Game_Update(void)
{
	switch (g_Phase)
	{
	case PHASE_INDEX_FADE:
		if (!Fade_IsFade()) {
			g_Phase = PHASE_INDEX_GAME;
			// PlaySound(SOUND_LABEL_BGM000);
		}
		break;
	case PHASE_INDEX_GAME:
		Bg_Update();
		Player_Update();
		Weapon_Update();
		Enemy_Update();
		EnemyWeapon_Update();
		Ui_Update();

		// フィールドの更新処理
		Field_Update();

		// カメラの更新処理
		Camera_Update();

		//ゲームの終了チェック
		if (Game_EndCheck())
		{
			Fade_Start(true, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
			g_Phase = PHASE_INDEX_STAGE_CLEAR;
		}
		break;
	case PHASE_INDEX_STAGE_CLEAR:
		if (!Fade_IsFade()) {
			Scene_Change(SCENE_INDEX_RESULT);
			g_Phase = PHASE_INDEX_END;
		}
		break;
	case PHASE_INDEX_END:
		break;
	}

}

void Game_Draw(void)
{
	// カメラの設定
	Camera_SetCamera();
	Bg_Draw();
	// 地面の描画処理
	Field_Draw();
	Player_Draw();
	Weapon_Draw();
	Enemy_Draw();
	EnemyWeapon_Draw();
	Ui_Draw();
	DrawTime();
	DrawPlayerScore();
	DrawEnemyScore();

}

bool Game_EndCheck(void)
{
	/*
	return true;終了
	return false;終了しない
	*/
	return false;
}
