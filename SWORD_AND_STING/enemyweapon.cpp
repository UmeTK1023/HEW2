#include "field.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"
#include "enemy.h"
#include "enemyweapon.h"
#include "player.h"
#include "ui.h"
#include "scene.h"
#include "weapon.h"
#include <math.h>
#include "sound.h"

//*****************************************************************************
// マクロ定義

//*****************************************************************************
#define MODEL_WEAPON   "asset/MODEL/BUKI1.x"	// 読み込むモデル名
#define MOVE_MODEL_WEAPON (1.5f)				// 移動速度
#define VALUE_MOVE  (5.0f)						// 移動量
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void Enemy_StateInit();			//初期化
void Enemy_StateWait();			//行動待機
void Enemy_Statedirection();	//攻撃方向選択
void Enemy_StateLightwait();	//攻撃方向光待ち
void Enemy_StateAttack();		//攻撃行動
void Enemy_StateGuard();		//ガード行動
void Enemy_StateAvoid();		//回避行動
void Enemy_StateCooldown();		//行動不能

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureEWeapon;	// テクスチャへのポインタ
LPD3DXMESH   g_pMeshEWeapon;			// メッシュ情報へのポインタ
LPD3DXBUFFER  g_pBuffMatEWeapon;		// マテリアル情報へのポインタ
DWORD    g_nNumMatEWeapon;				// マテリアル情報の総数

struct EWEAPON//敵構造体
{
	D3DXMATRIX   mtxWorldEModel;  // ワールドマトリックス
	D3DXVECTOR3   posEModel;   // 現在の位置
	D3DXVECTOR3   rotEModel;   // 現在の向き
	D3DXVECTOR3   rotDestEModel;  // 目的の向き
	D3DXVECTOR3   moveEModel;   // 移動量
	int state;
};
EWEAPON enemy;

static int    FlameCount;	// フレームカウント
static int    GuardCount;	// ガードのフレームカウント
static bool		ElightU;	// 敵のライト上方向
static bool		ElightR;	// 敵のライト右方向
static bool		ElightL;	// 敵のライト左方向
static bool    EatkU;		// 上方向攻撃
static bool    EatkR;		// 右方向攻撃
static bool    EatkL;		// 左方向攻撃
static bool    EguardU;		// 上方向ガード
static bool    EguardR;		// 右方向ガード
static bool    EguardL;		// 左方向ガード
static bool    Eavoid;		//回避
static bool    hitE;		//当たり判定
static int  g_ELightTable;	
static int randAtk;			// 攻撃方向ランダム変数
static int randguard;       // ガード方向ランダム変数
static int randAct;			// 行動ランダム変数
float EnemyWmove;			// 敵の移動速度
int g_AtkAlpha;

typedef enum 
{
	ENEMY_STATE_INIT,		// 初期化
	ENEMY_STATE_WAIT,		// 待機
	ENEMY_STATE_DIRECTION,	// 攻撃方向を決める
	ENEMY_STATE_LIGHTWAIT,	// 指示器光るまで待つ
	ENEMY_STATE_ATTACK,		// 攻撃
	ENEMY_STATE_GUARD,		// ガード
	ENEMY_STATE_AVOID,		// 回避
	ENEMY_STATE_COOLDOWN,	// 行動終了

	ENEMY_STATE_MAX

}ENEMY_STATE;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT EnemyWeapon_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	g_pTextureEWeapon = NULL;
	g_pMeshEWeapon = NULL;
	g_pBuffMatEWeapon = NULL;

	//位置・向き・移動量の初期設定
	enemy.posEModel = pos;
	enemy.rotEModel = rot;
	enemy.rotDestEModel = rot;
	enemy.moveEModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_WEAPON,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatEWeapon,
		NULL,
		&g_nNumMatEWeapon,
		&g_pMeshEWeapon)))
	{
		return E_FAIL;
	}

	//シードの初期化
	srand((unsigned int)time(NULL));

	enemy.state = ENEMY_STATE_INIT;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void EnemyWeapon_Finalize(void)
{
	if (g_pTextureEWeapon != NULL)
	{// テクスチャの開放
		g_pTextureEWeapon->Release();
		g_pTextureEWeapon = NULL;
	}

	if (g_pMeshEWeapon != NULL)
	{// メッシュの開放
		g_pMeshEWeapon->Release();
		g_pMeshEWeapon = NULL;
	}

	if (g_pBuffMatEWeapon != NULL)
	{// マテリアルの開放
		g_pBuffMatEWeapon->Release();
		g_pBuffMatEWeapon = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void EnemyWeapon_Update(void)
{
	D3DXVECTOR3 enemyPos = GetPosEnemy();
	D3DXVECTOR3 playerPos = GetPosPlayer();

	//基本ポジション
	if (!EatkU && !EatkR && !EatkL && !EguardU && !EguardR && !EguardL)
		enemy.posEModel.z = (enemyPos.z - 20.0f);

	switch (enemy.state)
	{
	case ENEMY_STATE_INIT:
		Enemy_StateInit();
		break;
	case ENEMY_STATE_WAIT:
		Enemy_StateWait();
		break;
	case ENEMY_STATE_DIRECTION:
		Enemy_Statedirection();
		break;
	case ENEMY_STATE_LIGHTWAIT:
		Enemy_StateLightwait();
		break;
	case ENEMY_STATE_ATTACK:
		Enemy_StateAttack();
		break;
	case ENEMY_STATE_GUARD:
		Enemy_StateGuard();
		break;
	case ENEMY_STATE_AVOID:
		Enemy_StateAvoid();
		break;
	case ENEMY_STATE_COOLDOWN:
		Enemy_StateCooldown();
		break;
	default:
		break;
	}

	//プレイヤー攻撃がヒットしたときに得点
	if (hitE)
	{
		SetLighTable(2);
		AddEnemyScore(1);
		PlaySound(SOUND_LABEL_SE_HIT);
		hitE = false;
		if (GetEnemyScore() >= 5)
		{
			Scene_Change(SCENE_INDEX_RESULT);
		}
	}
}

void Enemy_StateInit()
{
	randAtk = 0;
	randguard = 0;
	randAct = 0;
	FlameCount = 0;
	GuardCount = 0;
	g_ELightTable = 0;
	g_AtkAlpha = 0;
	EnemyWmove = MOVE_MODEL_WEAPON;
	EatkU = false;
	EatkR = false;
	EatkL = false;
	EguardU = false;
	EguardR = false;
	EguardL = false;
	Eavoid = false;
	hitE = false;

	enemy.state = ENEMY_STATE_WAIT;
}

void Enemy_StateWait()
{
	FlameCount++;

	if (FlameCount>60)
	{
		randAct = rand() % 3;
		switch (randAct)
		{
		case 0://攻撃
			FlameCount = 0;
			enemy.state = ENEMY_STATE_DIRECTION;
			break;
		case 1://ガード
			FlameCount = 0;
			enemy.state = ENEMY_STATE_GUARD;
			break;
		case 2://回避
			FlameCount = 0;
			enemy.state = ENEMY_STATE_AVOID;
			break;
		}
	}
}

void Enemy_Statedirection()
{
	//攻撃していなかったら攻撃可能
	if ((!EatkU) && (!EatkR) && (!EatkL))
	{
		randAtk = rand() % 3;
		switch (randAtk)
		{
		case 0:
			ElightU = true;
			enemy.state = ENEMY_STATE_LIGHTWAIT;
			break;
		case 1:
			ElightR = true;
			enemy.state = ENEMY_STATE_LIGHTWAIT;
			break;
		case 2:
			ElightL = true;
			enemy.state = ENEMY_STATE_LIGHTWAIT;
			break;
		}
	}
}

void Enemy_StateLightwait()
{
	if (ElightU)
	{
		SetEnemyAtkTable(2);

		FlameCount++;
		if ((FlameCount > 60) ||(g_AtkAlpha >= 255))
		{
			EatkU = true;
			ElightU = false;
			FlameCount = 0;
			g_AtkAlpha = 0;
			enemy.state = ENEMY_STATE_ATTACK;
		}
		g_AtkAlpha += 4.25;
	}

	if (ElightR)
	{
		SetEnemyAtkTable(3);

		FlameCount++;
		if ((FlameCount > 60) || (g_AtkAlpha >= 255))
		{
			EatkR = true;
			ElightR = false;
			FlameCount = 0;
			g_AtkAlpha = 0;
			enemy.state = ENEMY_STATE_ATTACK;
		}
		g_AtkAlpha += 4.25;
	}

	if (ElightL)
	{
		SetEnemyAtkTable(1);

		FlameCount++;
		if ((FlameCount > 60) || (g_AtkAlpha >= 255))
		{
			EatkL = true;
			ElightL = false;
			FlameCount = 0;
			g_AtkAlpha = 0;
			enemy.state = ENEMY_STATE_ATTACK;
		}
		g_AtkAlpha += 4.25;
	}
}

void Enemy_StateAttack()
{
	if (EatkU)
	{
		enemy.rotEModel.y = -0.3f;
		enemy.rotEModel.x = 0.4f;
		enemy.posEModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE * 12;
		enemy.posEModel.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE * 12;
		PlaySound(SOUND_LABEL_SE_ATTACK);
		enemy.state = ENEMY_STATE_COOLDOWN;
	}

	if (EatkR)
	{
		enemy.posEModel.x = 17.0f;
		enemy.rotEModel.y = 0.3f;
		enemy.rotEModel.x = 0.1f;
		enemy.posEModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE * 12;
		enemy.posEModel.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE * 12;
		PlaySound(SOUND_LABEL_SE_ATTACK);
		enemy.state = ENEMY_STATE_COOLDOWN;
	}

	if (EatkL)
	{
		enemy.rotEModel.y = -0.3f;
		enemy.rotEModel.x = 0.1f;
		enemy.posEModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE * 12;
		enemy.posEModel.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE * 12;
		PlaySound(SOUND_LABEL_SE_ATTACK);
		enemy.state = ENEMY_STATE_COOLDOWN;
	}

	//当たり判定用
	//上方向攻撃時、プレイヤーがガードか回避をしていなければヒット
	if ((EatkU) && (!GetguardU())&&(!Getavoid())&& (D3DXVec3Length(&GetEdistance())<170))
	{
		hitE = true;
	}
	//右方向攻撃時、プレイヤーがガードか回避をしていなければヒット
	if ((EatkR) && (!GetguardR()) && (!Getavoid()) && (D3DXVec3Length(&GetEdistance())<170))
	{
		hitE = true;
	}
	//左方向攻撃時、プレイヤーがガードか回避をしていなければヒット
	if ((EatkL) && (!GetguardL()) && (!Getavoid()) && (D3DXVec3Length(&GetEdistance())<170))
	{
		hitE = true;
	}

	//上方向攻撃時、プレイヤーがガードしていれば弾いた音を鳴らす
	if ((EatkU) && (GetguardU()))
	{
		PlaySound(SOUND_LABEL_SE_GUARD);
	}
	//右方向攻撃時、プレイヤーがガードしていれば弾いた音を鳴らす
	if ((EatkR) && (GetguardR()))
	{
		PlaySound(SOUND_LABEL_SE_GUARD);
	}
	//左方向攻撃時、プレイヤーがガードしていれば弾いた音を鳴らす
	if ((EatkL) && (GetguardL()))
	{
		PlaySound(SOUND_LABEL_SE_GUARD);
	}

}

void Enemy_StateGuard()
{
	//ガードしていなかったらガード可能
	if ((!EguardU) && (!EguardR) && (!EguardL))
	{
		randguard = rand() % 3;
		switch (randguard)
		{
		case 0:
			//enemy.posEModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE * 12;
			//enemy.posEModel.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE * 12;
			enemy.rotEModel.y = -0.3f;
			enemy.rotEModel.x = 0.4f;
			EguardU = true;
			PlaySound(SOUND_LABEL_SE_ATTACK);
			enemy.state = ENEMY_STATE_COOLDOWN;
			break;
		case 1:
			//enemy.posEModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE * 12;
			//enemy.posEModel.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE * 12;
			enemy.posEModel.x = 17.0f;
			enemy.rotEModel.y = 0.3f;
			enemy.rotEModel.x = 0.1f;
			EguardR = true;
			PlaySound(SOUND_LABEL_SE_ATTACK);
			enemy.state = ENEMY_STATE_COOLDOWN;
			break;
		case 2:
			//enemy.posEModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE * 12;
			//enemy.posEModel.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE * 12;
			enemy.rotEModel.y = -0.3f;
			enemy.rotEModel.x = 0.1f;
			EguardL = true;
			PlaySound(SOUND_LABEL_SE_ATTACK);
			enemy.state = ENEMY_STATE_COOLDOWN;
			break;
		}
	}
}

void Enemy_StateAvoid()
{
	//回避行動
	Eavoid = true;
	PlaySound(SOUND_LABEL_SE_AVOID);
	enemy.state = ENEMY_STATE_COOLDOWN;
}

void Enemy_StateCooldown()
{
	FlameCount++;

	//攻撃したら20フレームの間攻撃不能にする。
	if ((EatkU) || (EatkR) || (EatkL) && (FlameCount <= 20))
	{
		enemy.posEModel.z += 3.0f;
		if (FlameCount >= 20)
		{
			enemy.posEModel.x = -17.0f;
			enemy.rotEModel.y = -0.1f;
			enemy.rotEModel.x = 0.0f;
			EatkR = false;
			EatkU = false;
			EatkL = false;
			FlameCount = 0;//フレームの初期化
			SetLighTable(0);
			SetEnemyAtkTable(0);
			enemy.state = ENEMY_STATE_WAIT;
		}
	}
	//ガードしたら20フレームの間ガード不能にする。
	if ((EguardU) || (EguardR) || (EguardL) && (FlameCount <= 20))
	{
		//enemy.posEModel.z += 3.0f;
		if (FlameCount >= 20)
		{
			enemy.posEModel.x = -17.0f;
			enemy.rotEModel.y = -0.1f;
			enemy.rotEModel.x = 0.0f;
			EguardR = false;
			EguardU = false;
			EguardL = false;
			FlameCount = 0;//フレームの初期化
			enemy.state = ENEMY_STATE_WAIT;
		}
	}
	//回避したら20フレームの間回避不能にする。
	if ((Eavoid) && (FlameCount <= 20))
	{
		if (FlameCount >= 20)
		{
			Eavoid = false;
			FlameCount = 0;//フレームの初期化
			enemy.state = ENEMY_STATE_WAIT;
		}
	}

}


//=============================================================================
// 描画処理
//=============================================================================
void EnemyWeapon_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&enemy.mtxWorldEModel);

	//回転の反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy.rotEModel.y, enemy.rotEModel.x, enemy.rotEModel.z);
	D3DXMatrixMultiply(&enemy.mtxWorldEModel, &enemy.mtxWorldEModel, &mtxRot);

	//移動を反映
	D3DXMatrixTranslation(&mtxTranslate, enemy.posEModel.x, enemy.posEModel.y, enemy.posEModel.z);
	D3DXMatrixMultiply(&enemy.mtxWorldEModel, &enemy.mtxWorldEModel, &mtxTranslate);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &enemy.mtxWorldEModel);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)g_pBuffMatEWeapon->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatEWeapon; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureEWeapon);

		//描画
		g_pMeshEWeapon->DrawSubset(nCntMat);
	}
	//マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

}

bool GetEatkU()
{
	return EatkU;
}

bool GetEatkR()
{
	return EatkR;
}

bool GetEatkL()
{
	return EatkL;
}

bool GetEguardU()
{
	return EguardU;
}

bool GetEguardR()
{
	return EguardR;
}

bool GetEguardL()
{
	return EguardL;
}

bool GetEavoid()
{
	return Eavoid;
}

int GetELightTable()
{
	return g_ELightTable;
}

int GetAtkAlpha()
{
	return g_AtkAlpha;
}