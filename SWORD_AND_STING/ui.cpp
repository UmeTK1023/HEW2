#include "input.h"
#include "myDirect3D.h"
#include "sprite.h"
#include "texture.h"
#include "main.h"
#include "ui.h"
#include "scene.h"
#include "weapon.h"
#include "enemyweapon.h"

#define TIME_UI_SIZE_X		(600)	//時間?UIのXサイズ
#define TIME_UI_SIZE_Y		(600)	//時間?UIのYサイズ
#define ATTACK_UI_SIZE_X	(600)	//攻撃指示器UIのXサイズ
#define ATTACK_UI_SIZE_Y	(600)	//攻撃指示器UIのYサイズ
#define DEFENSIVE_UI_SIZE_X	(600)	//ガードUIのXサイズ
#define DEFENSIVE_UI_SIZE_Y	(600)	//ガードUIのYサイズ
#define HEALTH_UI_SIZE_X	(600)		//体力UIのXサイズ
#define HEALTH_UI_SIZE_Y	(600)	//体力UIのYサイズ
#define TIME_FIGURE			(2)		//タイマーの桁数
#define SCORE_FIGURE		(1)		//スコアの桁数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
int g_Time;			//制限時間
int g_CountTime;	//時間カウント
int g_PlayerScore;	//プレイヤースコア
int g_EnemyScore;	//エネミースコア
bool g_timerflag;
// UIを分割して表示するための変数
int g_HealthTable;		//プレイヤー体力ゲージ増減変数
int g_GuardTable;		//プレイヤーガードゲージ増減変数
static int FlameCount;
static int GuardFrameCount;
int g_EnemyAtk;
int g_PlayerDef;

typedef struct
{
	D3DXVECTOR2 pos;
}UI;

UI g_ui[5];//UI構造体配列


void Ui_Initialize(void)
{
	g_Time = 90;			//タイマーの初期値
	g_CountTime = 0;		//タイムカウント
	g_PlayerScore = 0;		//プレイヤースコアの初期化
	g_EnemyScore = 0;		//エネミースコアの初期化
	g_timerflag = false;	//制限時間終了フラグ
	FlameCount = 0;			//フレームカウントの初期化
	GuardFrameCount = 0;	
	g_HealthTable = 0;		
	g_GuardTable = 0;		
	g_EnemyAtk = 0;
	g_PlayerDef = 0;

	//真ん中上
	g_ui[0].pos = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.05f);							//時間?UIの座標
																									//真ん中下
	g_ui[1].pos = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.70f);							//攻撃指示器UIの描画
	g_ui[2].pos = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.735f);							//左下二つ
	g_ui[3].pos = D3DXVECTOR2(SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.25f));		//ガードUIの表示
	g_ui[4].pos = D3DXVECTOR2(SCREEN_WIDTH * 0.15f, SCREEN_HEIGHT - (SCREEN_HEIGHT * 0.15f));		//体力UIの表示
}

void Ui_Finalize(void)
{

}

void Ui_Update(void)
{
	//時間、体力、ガード、攻撃を切り替える処理を入れること
	if (g_CountTime > 60)
	{
		g_Time -= 1;
		g_CountTime = 0;
	}
	g_CountTime++;

	//９０秒経ったらフラグをtrueにする
	if (g_Time == 0)
	{
		g_timerflag = true;
	}

	//９０秒経ったらリザルトシーンへ移動
	if (g_timerflag)
	{
		Scene_Change(SCENE_INDEX_RESULT);

	}

	//体力が0以上かつ5以下
	if ((FlameCount >= 90) && (g_HealthTable > 0) && (g_HealthTable<5))
	{
		g_HealthTable -= 1;
		FlameCount = 0;
	}
	else if (FlameCount >= 90)
	{
		FlameCount = 0;
	}
	FlameCount++;

	//ガードゲージが0以上かつ5以下
	if ((GuardFrameCount >= 120) && (g_GuardTable > 0) && (g_GuardTable<5))
	{
		g_GuardTable -= 1;
		GuardFrameCount = 0;
	}
	else if (GuardFrameCount >= 120)
	{
		GuardFrameCount = 0;
	}
	GuardFrameCount++;
}

void Ui_Draw(void)
{


	
	//時間？の描画
	Sprite_Draw(TEXTURE_INDEX_LIGHT, g_ui[0].pos.x, g_ui[0].pos.y, 
	GetLightTable()*600, 0,600,600,
	TIME_UI_SIZE_X * 0.5f, TIME_UI_SIZE_Y *0.5f, 1.0f, 1.0f, 0.0f);

	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, GetAtkAlpha()));

	//敵の攻撃方向の描画
	Sprite_Draw(TEXTURE_INDEX_ATK, g_ui[1].pos.x, g_ui[1].pos.y,
		g_EnemyAtk *600, 0, 600, 600,
		ATTACK_UI_SIZE_X*0.5f, ATTACK_UI_SIZE_Y*0.5f,0.7f, 0.7f, 0.0f);
	
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

	//敵のガード方向の描画
	Sprite_Draw(TEXTURE_INDEX_DEF, g_ui[2].pos.x, g_ui[2].pos.y,
		g_PlayerDef*600, 0, 600, 600,
		ATTACK_UI_SIZE_X*0.5f, ATTACK_UI_SIZE_Y*0.5f,0.7f, 0.7f, 0.0f);
	
	//ガードゲージの描画
	Sprite_Draw(TEXTURE_INDEX_GUARD, g_ui[3].pos.x, g_ui[3].pos.y,
		g_GuardTable*DEFENSIVE_UI_SIZE_X, 0, 600, 600,
		600*0.5, 600*0.5, 1.0f, 1.0f, 0.0f);
	

	//体力ゲージの描画
	Sprite_Draw(TEXTURE_INDEX_HP, g_ui[4].pos.x, g_ui[4].pos.y,
		g_HealthTable * HEALTH_UI_SIZE_X,0, 600, 600,
		600*0.5, 600 * 0.5, 1.0f, 1.0f, 0.0f);

}


//制限時間の描画
void DrawTime()
{

	int Time = g_Time;

	for (int i = 0; i<TIME_FIGURE; i++)
	{
		int n;

		float Tx, Ty;

		n = Time % 10;      //一番右の数の抜き出し
		Ty = (n / 5) * 0.2f * 512;
		Tx = (n % 5) * 0.2f * 512;

		Time /= 10;         //一番右の数を落とす


		float Sx, Sy;

		Sx = 870.0f + 80.0f * (TIME_FIGURE - 1 - i);

		Sy = 0.0f;

		Sprite_Draw(TEXTURE_INDEX_NUMBER, Sx, Sy,
			Tx, Ty, 0.2f * 512, 0.2f * 512,
			0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
	}
}

//プレイヤースコア描画
void DrawPlayerScore()
{

	int Score = g_PlayerScore;

	for (int i = 0; i<SCORE_FIGURE; i++)
	{
		int n;

		float Tx, Ty;

		n = Score % 10;      //一番右の数の抜き出し
		Ty = (n / 5) * 0.2f * 512;
		Tx = (n % 5) * 0.2f * 512;

		Score /= 10;         //一番右の数を落とす


		float Sx, Sy;

		Sx = 700.0f + 70.0f * (SCORE_FIGURE - 1 - i);

		Sy = 8.7f;

		Sprite_Draw(TEXTURE_INDEX_NUMBER, Sx, Sy,
			Tx, Ty, 0.2f * 512, 0.2f * 512,
			1.0f, 1.0f, 0.8f, 0.8f, 0.0f);
	}
}

//エネミースコア描画
void DrawEnemyScore()
{

	int Score = g_EnemyScore;

	for (int i = 0; i<SCORE_FIGURE; i++)
	{
		int n;

		float Tx, Ty;

		n = Score % 10;      //一番右の数の抜き出し
		Ty = (n / 5) * 0.2f * 512;
		Tx = (n % 5) * 0.2f * 512;

		Score /= 10;         //一番右の数を落とす


		float Sx, Sy;

		Sx = 1140.0f + 70.0f * (SCORE_FIGURE - 1 - i);

		Sy = 8.7f;

		Sprite_Draw(TEXTURE_INDEX_NUMBER, Sx, Sy,
			Tx, Ty, 0.2f * 512, 0.2f * 512,
			0.0f, 0.0f, 0.8f, 0.8f, 0.0f);
	}
}

//プレイヤースコアを加算
void AddPlayerScore(int Score)
{
	g_PlayerScore += Score;
}

//エネミースコアを加算
void AddEnemyScore(int Score)
{
	g_EnemyScore += Score;
}

//現在のプレイヤースコアを取得
int GetPlayerScore()
{
	return g_PlayerScore;
}

//現在のエネミースコアを取得
int GetEnemyScore()
{
	return g_EnemyScore;
}


// UI分割数値を変える
int AddTable(int n)
{
	if ((g_HealthTable >= 0) || (g_HealthTable<5))
	{
		g_HealthTable += n;
	}
	return g_HealthTable;
}

int GetTable()
{
	return g_HealthTable;
}

// UI分割数値を変える
int AddGuardTable(int n)
{
	if ((g_GuardTable >= 0) || (g_GuardTable<5))
	{
		g_GuardTable += n;
	}
	return g_GuardTable;
}

int GetGuardTable()
{
	return g_GuardTable;
}

void SetEnemyAtkTable(int n)
{
	g_EnemyAtk = n;
}

void SetPlayerDefTable(int n)
{
	g_PlayerDef = n;
}

