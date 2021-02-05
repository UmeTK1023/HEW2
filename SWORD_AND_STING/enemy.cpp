//=============================================================================
//
// プレイヤー処理 [Enemy.cpp]
//
//=============================================================================
#include "myDirect3D.h"
#include "texture.h"
#include "debugproc.h"
#include "enemy.h"
#include "enemyweapon.h"
#include "player.h"
#include "CD3DXObject.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_CAR   "asset/MODEL/tiny.x" // 読み込むモデル名
#define VALUE_MOVE_MODEL (1.5f)			 // 移動速度
#define VALUE_MOVE  (5.0f)				 // 移動量
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DXVECTOR3 Edistance;	//敵とプレイヤーの距離を入れる
float EnemyMove;		//敵の動く速度

struct ENEMY
{
	D3DXMATRIX			mtxWorldModel;	// ワールドマトリックス
	D3DXVECTOR3			posModel;		// 現在の位置
	D3DXVECTOR3			rotModel;		// 現在の向き
	D3DXVECTOR3			sclModel;		// 現在のスケール
	D3DXVECTOR3			rotDestModel;	// 目的の向き
	D3DXVECTOR3			moveModel;		// 移動量

	CD3DXObject			meshObject;
	float				animFrame;
};
ENEMY enemy;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Enemy_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	EnemyMove = VALUE_MOVE_MODEL;
	Edistance = D3DXVECTOR3(0.0f,0.0f,0.0f);

	// 位置・向き・移動量の初期設定
	enemy.posModel = pos;
	enemy.rotModel = rot;
	enemy.sclModel = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
	enemy.rotDestModel = rot;
	enemy.moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//スキンメッシュの読み込み
	enemy.meshObject.Load((char*)"asset/MODEL/tiny.x");
	enemy.animFrame = 0.0f;
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Enemy_Finalize(void)
{
	//enemy.meshObject.Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void Enemy_Update(void)
{
	D3DXVECTOR3 playerPos = GetPosPlayer();

	Edistance = enemy.posModel - playerPos;
	if(D3DXVec3Length(&Edistance)<100)
	{
		EnemyMove *= -1.0f;
	}

	enemy.posModel.z += EnemyMove;

#if 1
	//// 範囲チェック
	//if (enemy.posModel.z < 0.0f)
	//{
	//	EnemyMove *= -1.0f;
	//}
	if (enemy.posModel.z > 300.0f)
	{
		EnemyMove *= -1.0f;
	}
#endif

	if ((GetEavoid()) && (enemy.posModel.z <140))
	{
		enemy.posModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE*-10;
		enemy.posModel.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE*-10;
	}

	enemy.meshObject.Update(enemy.animFrame);
	//敵が何も行動をしていないときのみアニメーションする
	if (!GetEatkU() && !GetEatkR() && !GetEatkL() && !GetEguardU() && !GetEguardR() && !GetEguardL() && !GetEavoid())
	enemy.animFrame += 0.01f;

}

//=============================================================================
// 描画処理
//=============================================================================
void Enemy_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&enemy.mtxWorldModel);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, enemy.sclModel.x, enemy.sclModel.y, enemy.sclModel.z);
	D3DXMatrixMultiply(&enemy.mtxWorldModel, &enemy.mtxWorldModel, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy.rotModel.y, enemy.rotModel.x, enemy.rotModel.z);
	D3DXMatrixMultiply(&enemy.mtxWorldModel, &enemy.mtxWorldModel, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, enemy.posModel.x, enemy.posModel.y, enemy.posModel.z);
	D3DXMatrixMultiply(&enemy.mtxWorldModel, &enemy.mtxWorldModel, &mtxTranslate);

	enemy.meshObject.GetMatrix() = enemy.mtxWorldModel;

	enemy.meshObject.Render();
}

D3DXVECTOR3 GetPosEnemy()
{
	return enemy.posModel;
}

D3DXVECTOR3 GetEdistance()
{
	return Edistance;
}