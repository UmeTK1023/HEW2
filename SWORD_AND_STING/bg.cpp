#include "bg.h"
#include "myDirect3D.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_BG   "asset/MODEL/bg.x"  // 読み込むモデル名

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBg; // テクスチャへのポインタ
LPD3DXMESH   g_pMeshBg;  // メッシュ情報へのポインタ
LPD3DXBUFFER  g_pBuffMatBg; // マテリアル情報へのポインタ
DWORD    g_nNumMatBg;  // マテリアル情報の総数

struct BG
{
	D3DXMATRIX   mtxWorldFModel;  // ワールドマトリックス
	D3DXVECTOR3   posFModel;   // 現在の位置
	D3DXVECTOR3   rotFModel;   // 現在の向き
	D3DXVECTOR3   rotDestFModel;  // 目的の向き
	D3DXVECTOR3   moveFModel;   // 移動量
	D3DXVECTOR3	  scaleFModel;//大きさ
};
BG Bg;

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Bg_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	g_pTextureBg = NULL;
	g_pMeshBg = NULL;
	g_pBuffMatBg = NULL;

	//位置・向き・移動量の初期設定
	Bg.posFModel = pos;
	Bg.rotFModel = rot;
	Bg.rotDestFModel = rot;
	Bg.moveFModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Bg.scaleFModel = D3DXVECTOR3(10.0f, 10.0f, 10.0f);

	//Xファイルの読み込み
	if (FAILED(D3DXLoadMeshFromX(MODEL_BG,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatBg,
		NULL,
		&g_nNumMatBg,
		&g_pMeshBg)))
	{
		return E_FAIL;
	}
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Bg_Finalize(void)
{
	if (g_pTextureBg != NULL)
	{// テクスチャの開放
		g_pTextureBg->Release();
		g_pTextureBg = NULL;
	}

	if (g_pMeshBg != NULL)
	{// メッシュの開放
		g_pMeshBg->Release();
		g_pMeshBg = NULL;
	}

	if (g_pBuffMatBg != NULL)
	{// マテリアルの開放
		g_pBuffMatBg->Release();
		g_pBuffMatBg = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Bg_Update(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void Bg_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxscl;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&Bg.mtxWorldFModel);

	//回転の反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, Bg.rotFModel.y, Bg.rotFModel.x, Bg.rotFModel.z);
	D3DXMatrixMultiply(&Bg.mtxWorldFModel, &Bg.mtxWorldFModel, &mtxRot);

	//移動を反映
	D3DXMatrixTranslation(&mtxTranslate, Bg.posFModel.x, Bg.posFModel.y, Bg.posFModel.z);
	D3DXMatrixMultiply(&Bg.mtxWorldFModel, &Bg.mtxWorldFModel, &mtxTranslate);

	//スケールを反映
	D3DXMatrixScaling(&mtxscl, Bg.scaleFModel.x, Bg.scaleFModel.y, Bg.scaleFModel.z);
	D3DXMatrixMultiply(&Bg.mtxWorldFModel, &Bg.mtxWorldFModel, &mtxscl);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &Bg.mtxWorldFModel);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアル情報に対するポインタを取得
	pD3DXMat = (D3DXMATERIAL*)g_pBuffMatBg->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatBg; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBg);

		//描画
		g_pMeshBg->DrawSubset(nCntMat);
	}
	//マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

