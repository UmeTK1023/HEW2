#include "field.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"
#include "enemy.h"
#include "player.h"
#include "ui.h"
#include "scene.h"
#include "enemyweapon.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_WEAPON   "asset/MODEL/BUKI1.x"				// 読み込むモデル名
#define	VALUE_MOVE_WEAPON		(5.0f)						// 移動量
#define	VALUE_ROTATE_WEAPON	(D3DX_PI * 0.02f)				// 回転量

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureWeapon; // テクスチャへのポインタ
LPD3DXMESH   g_pMeshWeapon;  // メッシュ情報へのポインタ
LPD3DXBUFFER  g_pBuffMatWeapon; // マテリアル情報へのポインタ
DWORD    g_nNumMatWeapon;  // マテリアル情報の総数

struct WEAPON
{
	D3DXMATRIX   mtxWorldModel;  // ワールドマトリックス
	D3DXVECTOR3   posModel;   // 現在の位置
	D3DXVECTOR3   rotModel;   // 現在の向き
	float rotspeed;
	D3DXVECTOR3   rotDestModel;  // 目的の向き
	D3DXVECTOR3   moveModel;   // 移動量
};

WEAPON weapon;
static int	FlameCount;		// フレームカウント
static int	GuardFrame;		// ガードフレームカウント
static int  AvoidFrame;		// 回避フレームカウント
static bool	atkU;			// 上方向攻撃
static bool	atkR;			// 右方向攻撃
static bool	atkL;			// 左方向攻撃
static bool	guardU;			// 上方向ガード
static bool	guardR;			// 右方向ガード
static bool	guardL;			// 左方向ガード
static int  UiHealthTable;	// 体力テーブル（？）
static int  UiGuardTable;	// ガードテーブル（？）
static int  g_LightTable;	// ライトテーブル（？）
static bool hit;			// 当たり判定
static bool Avoid;			// 回避
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Weapon_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();


	g_pTextureWeapon = NULL;
	g_pMeshWeapon = NULL;
	g_pBuffMatWeapon = NULL;
	//位置・向き・移動量の初期設定
	weapon.posModel = pos;
	weapon.rotModel = rot;
	weapon.rotDestModel = rot;
	weapon.moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_WEAPON,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatWeapon,
		NULL,
		&g_nNumMatWeapon,
		&g_pMeshWeapon)))
	{
		return E_FAIL;
	}
	FlameCount = 0;
	GuardFrame = 0;
	AvoidFrame = 0;
	g_LightTable = 0;
	UiHealthTable = GetTable();
	UiGuardTable = GetGuardTable();
	atkU = false;
	atkR = false;
	atkL = false;
	guardU = false;
	guardR = false;
	guardL = false;
	hit = false;
	Avoid = false;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Weapon_Finalize(void)
{
	if (g_pTextureWeapon != NULL)
	{// テクスチャの開放
		g_pTextureWeapon->Release();
		g_pTextureWeapon = NULL;
	}

	if (g_pMeshWeapon != NULL)
	{// メッシュの開放
		g_pMeshWeapon->Release();
		g_pMeshWeapon = NULL;
	}

	if (g_pBuffMatWeapon != NULL)
	{// マテリアルの開放
		g_pBuffMatWeapon->Release();
		g_pBuffMatWeapon = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Weapon_Update(void)
{
	UiHealthTable = GetTable();
	UiGuardTable = GetGuardTable();
	D3DXVECTOR3 enemyPos = GetPosEnemy();
	D3DXVECTOR3 playerPos = GetPosPlayer();

	//基本ポジション
	if (!atkU && !atkR && !atkL && !guardU && !guardR && !guardL)
		weapon.posModel.z = playerPos.z+85;

	//// 奥移動
	//if (Keyboard_IsPress(DIK_W))
	//{
	//	weapon.posModel.x += sinf(D3DX_PI * 1.0f) * VALUE_MOVE_WEAPON;
	//	weapon.posModel.z -= cosf(D3DX_PI * 1.0f) * VALUE_MOVE_WEAPON;

	//}
	//else if (Keyboard_IsPress(DIK_S))
	//{// 手前移動
	//	weapon.posModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON;
	//	weapon.posModel.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON;
	//}
	/*else*/ if (keyboard.IsTrigger(DIK_R))
	{
		weapon.rotModel.y -= 0.1f;
	}

	//攻撃していなかったら攻撃可能
	if ((!Avoid) && (!atkU) && (!atkR) && (!atkL) && (UiHealthTable < 4))
	{
		//スペースキー&↑キーで上方向攻撃、少し前に出る
		if (keyboard.IsTrigger(DIK_SPACE) && keyboard.IsPress(DIK_UPARROW) || (!joycon[0].IsPress(JOYCON_R)) &&(joycon[0].GetGyro_Y() >= 0.9f))
		{
			weapon.rotModel.y = 21.7f;
			weapon.rotModel.x = 13.2f;
			weapon.posModel.y = 22.0f;
			weapon.posModel.x = -6.0f;
			weapon.posModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON*-12;
			weapon.posModel.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON*-12;
			atkU = true;
			PlaySound(SOUND_LABEL_SE_ATTACK);
			AddTable(1);
		}
		if ((atkU) && (!GetEguardU()) && (!GetEavoid()) && (D3DXVec3Length(&GetPdistance())<170))
		{
			hit = true;
		}
		if ((atkU) && (GetEguardU()))
		{
			PlaySound(SOUND_LABEL_SE_GUARD);
		}
	}
	//攻撃していなかったら攻撃可能
	if ((!Avoid) && (!atkU) && (!atkR) && (!atkL) && (UiHealthTable < 4))
	{
		//スペースキー&→キーで右方向攻撃、少し前に出る
		if (keyboard.IsTrigger(DIK_SPACE) && keyboard.IsPress(DIK_RIGHTARROW) || (!joycon[0].IsPress(JOYCON_R))&&(joycon[0].GetGyro_Z() >= 0.9f))
		{
			weapon.posModel.x = -17.0f;
			weapon.rotModel.y = 10.0f;
			weapon.posModel.y = 20.0f;
			weapon.posModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON*-12;
			weapon.posModel.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON*-12;
			atkR = true;
			PlaySound(SOUND_LABEL_SE_ATTACK);
			AddTable(1);
		}
		if ((atkR) && (!GetEguardR()) && (!GetEavoid()) && (D3DXVec3Length(&GetPdistance())<170))
		{
			hit = true;
		}
		if ((atkR) && (GetEguardR()))
		{
			PlaySound(SOUND_LABEL_SE_GUARD);
		}
	}
	//攻撃していなかったら攻撃可能
	if ((!Avoid) && (!atkU) && (!atkR) && (!atkL)&& (UiHealthTable < 4))
	{
		if (keyboard.IsTrigger(DIK_SPACE) && keyboard.IsPress(DIK_LEFTARROW) || (!joycon[0].IsPress(JOYCON_R))&&(joycon[0].GetGyro_Z() <= -0.9f))
		{
			//スペースキー&←キーで左方向攻撃、少し前に出る
			weapon.rotModel.y = 8.7f;
			weapon.posModel.y = 26.0f;
			weapon.posModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON*-12;
			weapon.posModel.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON*-12;
			atkL = true;
			PlaySound(SOUND_LABEL_SE_ATTACK);
			AddTable(1);
		}
		if ((atkL) && (!GetEguardL()) && (!GetEavoid()) && (D3DXVec3Length(&GetPdistance())<170))
		{
			hit = true;
		}
		if ((atkL) && (GetEguardL()))
		{
			PlaySound(SOUND_LABEL_SE_GUARD);
		}
	}

	//攻撃したら20フレーム間攻撃不能にする。
	if ((atkU) || (atkR) || (atkL) && (FlameCount <= 20))
	{
		//weapon.posModel.z -= 3.0f;
		FlameCount++;
		if (FlameCount >= 20)
		{
			weapon.posModel.x = 20.0f;
			weapon.posModel.y = 15.0f;
			weapon.rotModel.y = 15.7f;
			weapon.rotModel.z = 0.0f;
			weapon.rotModel.x = 0.0f;
			atkU = false;
			atkR = false;
			atkL = false;
			FlameCount = 0;//フレームの初期化
			g_LightTable = 0;
		}
	}

	//ガードゲージがないときはガード行動が出来ない
	if (UiGuardTable < 4)
	{
		//ガードしていなかったらガード可能
		if ((!guardU) && (!guardR) && (!guardL))
		{
			//スペースキー&Ｉキーで上方向ガード、少し前に出る
			if (keyboard.IsTrigger(DIK_SPACE) && keyboard.IsPress(DIK_I)||joycon[0].IsPress(JOYCON_R) && (joycon[0].GetGyro_Y() >= 0.9f))
			{
				weapon.rotModel.y = 20.7f;
				weapon.rotModel.x = 13.7f;
				weapon.posModel.y = 22.0f;
				weapon.posModel.x = -8.0f;
				//weapon.posModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON*-12;
				//weapon.posModel.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON*-12;
				guardU = true;
				SetPlayerDefTable(2);
				PlaySound(SOUND_LABEL_SE_ATTACK);
				AddGuardTable(1);
			}
		}
		//ガードしていなかったらガード可能
		if ((!guardU) && (!guardR) && (!guardL))
		{
			//スペースキー&Ｌキーで右方向ガード、少し前に出る
			if (keyboard.IsTrigger(DIK_SPACE) && keyboard.IsPress(DIK_L)|| joycon[0].IsPress(JOYCON_R)&&(joycon[0].GetGyro_Z() >= 0.9f))
			{
				weapon.posModel.x = -17.0f;
				weapon.rotModel.y = 10.0f;
				weapon.posModel.y = 20.0f;
				//weapon.posModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON*-12;
				//weapon.posModel.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON*-12;
				guardR = true;
				SetPlayerDefTable(3);
				PlaySound(SOUND_LABEL_SE_ATTACK);
				AddGuardTable(1);
			}
		}
		//ガードしていなかったらガード可能
		if ((!guardU) && (!guardR) && (!guardL))
		{
			//スペースキー&Ｊキーで左方向ガード、少し前に出る
			if (keyboard.IsTrigger(DIK_SPACE) && keyboard.IsPress(DIK_J) || joycon[0].IsPress(JOYCON_R) && (joycon[0].GetGyro_Z() <= -0.9f))
			{
				weapon.rotModel.y = 8.7f;
				weapon.posModel.y = 26.0f;
				//weapon.posModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON*-12;
				//weapon.posModel.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON*-12;
				guardL = true;
				SetPlayerDefTable(1);
				PlaySound(SOUND_LABEL_SE_ATTACK);
				AddGuardTable(1);
			}
		}
	}

	//ガードしたら20フレーム間ガード不能にする。
	if ((guardU) || (guardR) || (guardL) && (GuardFrame <= 20))
	{

		GuardFrame++;
		if (GuardFrame >= 20)
		{
			weapon.posModel.x = 20.0f;
			weapon.posModel.y = 15.0f;
			weapon.rotModel.y = 15.7f;
			weapon.rotModel.z = 0.0f; 
			weapon.rotModel.x = 0.0f;
			guardU = false;
			guardR = false;
			guardL = false;
			SetPlayerDefTable(0);
			GuardFrame = 0;//フレームの初期化
		}

	}

	//回避したら２０フレーム回避不能にする
	if ((Avoid) && (AvoidFrame <= 20))
	{
		AvoidFrame++;
		if (AvoidFrame >= 20)
		{
			Avoid = false;
			AvoidFrame = 0;//フレームの初期化
		}
	}

	//体力がないときは回避行動が出来ない
	if ((!Avoid)&&(UiHealthTable < 4))
	{
		if (keyboard.IsTrigger(DIK_Z)||joycon[0].IsTrigger(JOYCON_ZR))
		{//回避行動
			weapon.posModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON;
			weapon.posModel.z -= cosf(D3DX_PI * 0.0f) * (VALUE_MOVE_WEAPON + 10.0f);
			Avoid = true;
			PlaySound(SOUND_LABEL_SE_AVOID);
			AddTable(1);
		}
	}

	//プレイヤー攻撃がヒットしたときに得点
	if (hit)
	{
		DebugProc_Print((char *)"プレイヤーの攻撃命中確認 : 命中\n");
		g_LightTable = 3;
		AddPlayerScore(1);
		PlaySound(SOUND_LABEL_SE_HIT);
		hit = false;
		if (GetPlayerScore() >= 5)
		{
			Scene_Change(SCENE_INDEX_RESULT);

		}
	}
	
	//初期位置に戻す
	//if (Keyboard_IsPress(DIK_RETURN))
	//{// リセット
	//	weapon.posModel.x = PLAYER_WIDTH;
	//	weapon.posModel.y = PLAYER_HEIGHT / 2;
	//	weapon.posModel.z = -100.0f;
	//	weapon.rotModel.x = 0.0f;
	//	weapon.rotModel.y = -0.2f;
	//	weapon.rotModel.z = 0.0f;
	//}

}

//=============================================================================
// 描画処理
//=============================================================================
void Weapon_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&weapon.mtxWorldModel);

	//回転の反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, weapon.rotModel.y, weapon.rotModel.x, weapon.rotModel.z);
	D3DXMatrixMultiply(&weapon.mtxWorldModel, &weapon.mtxWorldModel, &mtxRot);

	//移動を反映
	D3DXMatrixTranslation(&mtxTranslate, weapon.posModel.x, weapon.posModel.y, weapon.posModel.z);
	D3DXMatrixMultiply(&weapon.mtxWorldModel, &weapon.mtxWorldModel, &mtxTranslate);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &weapon.mtxWorldModel);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)g_pBuffMatWeapon->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatWeapon; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureWeapon);

		//描画
		g_pMeshWeapon->DrawSubset(nCntMat);
	}
	//マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

int GetLightTable()
{
	return g_LightTable;
}
void SetLighTable(int n)
{
	g_LightTable = n;
}
bool GetguardU()
{
	return guardU;
}
bool GetguardR()
{
	return guardR;
}
bool GetguardL()
{
	return guardL;
}
bool Getavoid()
{
	return Avoid;
}