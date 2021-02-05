#include "camera.h"
#include "myDirect3D.h"
#include "input.h"
#include "player.h"
#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	CAM_POS_V_X		(0.0f)					// カメラの視点初期位置(X座標)
#define	CAM_POS_V_Y		(120.0f)				// カメラの視点初期位置(Y座標)
#define	CAM_POS_V_Z		(0.0f)					// カメラの視点初期位置(Z座標)
#define	CAM_POS_R_X		(0.0f)					// カメラの注視点初期位置(X座標)
#define	CAM_POS_R_Y		(10.0f)					// カメラの注視点初期位置(Y座標)
#define	CAM_POS_R_Z		(-10.0f)				// カメラの注視点初期位置(Z座標)
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比
#define	VIEW_NEAR_Z		(10.0f)											// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(3000.0f)										// ビュー平面のFarZ値
#define	VALUE_MOVE		(5.0f)											// 移動量

//*****************************************************************************
// カメラの構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 ePosR;			// 敵の座標取得用変数
	D3DXVECTOR3 pPosV;			// プレイヤーの座標取得用変数
	D3DXVECTOR3 posV;			// 視点
	D3DXVECTOR3 posR;			// 注視点
	D3DXVECTOR3 vecU;			// 上方向ベクトル 天井を指すために使うほぼ固定
	D3DXMATRIX mtxProjection;	// プロジェクションマトリックス
	D3DXMATRIX mtxView;			// ビューマトリックス
} CAMERA;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
CAMERA g_camera;		// カメラ情報

//*****************************************************************************
// グローバル変数
//*****************************************************************************

//=============================================================================
// カメラの初期化処理
//=============================================================================
void Camera_Initialize(void)
{
	// プレイヤーと敵の座標を取得
	g_camera.ePosR = GetPosEnemy();
	g_camera.pPosV = GetPosPlayer();

	//カメラ初期位置の初期化
	//g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
	g_camera.posV = D3DXVECTOR3(g_camera.pPosV.x, g_camera.pPosV.y+PLAYER_HEIGHT+10, g_camera.pPosV.z);
	
	//カメラ注視点の初期化
	//g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);
	g_camera.posR = D3DXVECTOR3(g_camera.ePosR.x, g_camera.ePosR.y+20, g_camera.ePosR.z+10.0f);

	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void Camera_Finalize(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void Camera_Update(void)
{
	// プレイヤーと敵の座標を取得
	g_camera.ePosR = GetPosEnemy();
	//g_camera.posR = D3DXVECTOR3(g_camera.ePosR.x, g_camera.ePosR.y, g_camera.ePosR.z);

	g_camera.pPosV = GetPosPlayer();
	g_camera.posV = D3DXVECTOR3(g_camera.pPosV.x, g_camera.pPosV.y + PLAYER_HEIGHT, g_camera.pPosV.z + 10.0f);

	if (keyboard.IsPress(DIK_W) || (joycon[0].GetStick_Y() > 0.0f))
		{// 奥移動
			//カメラ移動
			//g_camera.posV.x += sinf(D3DX_PI * 1.0f) * VALUE_MOVE;
			//g_camera.posV.z -= cosf(D3DX_PI * 1.0f) * VALUE_MOVE;
	
			g_camera.posR.x += sinf(D3DX_PI * 1.0f) * VALUE_MOVE;
			g_camera.posR.z -= cosf(D3DX_PI * 1.0f) * VALUE_MOVE;
		}
		//else if (Keyboard_IsPress(DIK_S))
		//{// 手前移動
		//	//カメラ移動
		//	//g_camera.posV.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE;
		//	//g_camera.posV.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE;
		//	g_camera.posR.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE;
		//	g_camera.posR.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE;
		//}
}
 
//=============================================================================
// カメラの設定処理
//=============================================================================
void Camera_SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice(); 

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								VIEW_ANGLE,				// ビュー平面の視野角
								VIEW_ASPECT,			// ビュー平面のアスペクト比
								VIEW_NEAR_Z,			// ビュー平面のNearZ値
								VIEW_FAR_Z);			// ビュー平面のFarZ値

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView, 
						&g_camera.posV,		// カメラの視点
						&g_camera.posR,		// カメラの注視点
						&g_camera.vecU);	// カメラの上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

