#include "camera.h"
#include "myDirect3D.h"
#include "input.h"
#include "player.h"
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CAM_POS_V_X		(0.0f)					// �J�����̎��_�����ʒu(X���W)
#define	CAM_POS_V_Y		(120.0f)				// �J�����̎��_�����ʒu(Y���W)
#define	CAM_POS_V_Z		(0.0f)					// �J�����̎��_�����ʒu(Z���W)
#define	CAM_POS_R_X		(0.0f)					// �J�����̒����_�����ʒu(X���W)
#define	CAM_POS_R_Y		(10.0f)					// �J�����̒����_�����ʒu(Y���W)
#define	CAM_POS_R_Z		(-10.0f)				// �J�����̒����_�����ʒu(Z���W)
#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��
#define	VIEW_NEAR_Z		(10.0f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z		(3000.0f)										// �r���[���ʂ�FarZ�l
#define	VALUE_MOVE		(5.0f)											// �ړ���

//*****************************************************************************
// �J�����̍\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 ePosR;			// �G�̍��W�擾�p�ϐ�
	D3DXVECTOR3 pPosV;			// �v���C���[�̍��W�擾�p�ϐ�
	D3DXVECTOR3 posV;			// ���_
	D3DXVECTOR3 posR;			// �����_
	D3DXVECTOR3 vecU;			// ������x�N�g�� �V����w�����߂Ɏg���قڌŒ�
	D3DXMATRIX mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;			// �r���[�}�g���b�N�X
} CAMERA;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
CAMERA g_camera;		// �J�������

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************

//=============================================================================
// �J�����̏���������
//=============================================================================
void Camera_Initialize(void)
{
	// �v���C���[�ƓG�̍��W���擾
	g_camera.ePosR = GetPosEnemy();
	g_camera.pPosV = GetPosPlayer();

	//�J���������ʒu�̏�����
	//g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
	g_camera.posV = D3DXVECTOR3(g_camera.pPosV.x, g_camera.pPosV.y+PLAYER_HEIGHT+10, g_camera.pPosV.z);
	
	//�J���������_�̏�����
	//g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);
	g_camera.posR = D3DXVECTOR3(g_camera.ePosR.x, g_camera.ePosR.y+20, g_camera.ePosR.z+10.0f);

	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void Camera_Finalize(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void Camera_Update(void)
{
	// �v���C���[�ƓG�̍��W���擾
	g_camera.ePosR = GetPosEnemy();
	//g_camera.posR = D3DXVECTOR3(g_camera.ePosR.x, g_camera.ePosR.y, g_camera.ePosR.z);

	g_camera.pPosV = GetPosPlayer();
	g_camera.posV = D3DXVECTOR3(g_camera.pPosV.x, g_camera.pPosV.y + PLAYER_HEIGHT, g_camera.pPosV.z + 10.0f);

	if (keyboard.IsPress(DIK_W) || (joycon[0].GetStick_Y() > 0.0f))
		{// ���ړ�
			//�J�����ړ�
			//g_camera.posV.x += sinf(D3DX_PI * 1.0f) * VALUE_MOVE;
			//g_camera.posV.z -= cosf(D3DX_PI * 1.0f) * VALUE_MOVE;
	
			g_camera.posR.x += sinf(D3DX_PI * 1.0f) * VALUE_MOVE;
			g_camera.posR.z -= cosf(D3DX_PI * 1.0f) * VALUE_MOVE;
		}
		//else if (Keyboard_IsPress(DIK_S))
		//{// ��O�ړ�
		//	//�J�����ړ�
		//	//g_camera.posV.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE;
		//	//g_camera.posV.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE;
		//	g_camera.posR.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE;
		//	g_camera.posR.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE;
		//}
}
 
//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void Camera_SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice(); 

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
								VIEW_ANGLE,				// �r���[���ʂ̎���p
								VIEW_ASPECT,			// �r���[���ʂ̃A�X�y�N�g��
								VIEW_NEAR_Z,			// �r���[���ʂ�NearZ�l
								VIEW_FAR_Z);			// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView, 
						&g_camera.posV,		// �J�����̎��_
						&g_camera.posR,		// �J�����̒����_
						&g_camera.vecU);	// �J�����̏�����x�N�g��

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

