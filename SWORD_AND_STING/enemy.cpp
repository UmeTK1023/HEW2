//=============================================================================
//
// �v���C���[���� [Enemy.cpp]
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
// �}�N����`
//*****************************************************************************
#define MODEL_CAR   "asset/MODEL/tiny.x" // �ǂݍ��ރ��f����
#define VALUE_MOVE_MODEL (1.5f)			 // �ړ����x
#define VALUE_MOVE  (5.0f)				 // �ړ���
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DXVECTOR3 Edistance;	//�G�ƃv���C���[�̋���������
float EnemyMove;		//�G�̓������x

struct ENEMY
{
	D3DXMATRIX			mtxWorldModel;	// ���[���h�}�g���b�N�X
	D3DXVECTOR3			posModel;		// ���݂̈ʒu
	D3DXVECTOR3			rotModel;		// ���݂̌���
	D3DXVECTOR3			sclModel;		// ���݂̃X�P�[��
	D3DXVECTOR3			rotDestModel;	// �ړI�̌���
	D3DXVECTOR3			moveModel;		// �ړ���

	CD3DXObject			meshObject;
	float				animFrame;
};
ENEMY enemy;

//=============================================================================
// ����������
//=============================================================================
HRESULT Enemy_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	EnemyMove = VALUE_MOVE_MODEL;
	Edistance = D3DXVECTOR3(0.0f,0.0f,0.0f);

	// �ʒu�E�����E�ړ��ʂ̏����ݒ�
	enemy.posModel = pos;
	enemy.rotModel = rot;
	enemy.sclModel = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
	enemy.rotDestModel = rot;
	enemy.moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�X�L�����b�V���̓ǂݍ���
	enemy.meshObject.Load((char*)"asset/MODEL/tiny.x");
	enemy.animFrame = 0.0f;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Enemy_Finalize(void)
{
	//enemy.meshObject.Release();
}

//=============================================================================
// �X�V����
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
	//// �͈̓`�F�b�N
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
	//�G�������s�������Ă��Ȃ��Ƃ��̂݃A�j���[�V��������
	if (!GetEatkU() && !GetEatkR() && !GetEatkL() && !GetEguardU() && !GetEguardR() && !GetEguardL() && !GetEavoid())
	enemy.animFrame += 0.01f;

}

//=============================================================================
// �`�揈��
//=============================================================================
void Enemy_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&enemy.mtxWorldModel);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, enemy.sclModel.x, enemy.sclModel.y, enemy.sclModel.z);
	D3DXMatrixMultiply(&enemy.mtxWorldModel, &enemy.mtxWorldModel, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy.rotModel.y, enemy.rotModel.x, enemy.rotModel.z);
	D3DXMatrixMultiply(&enemy.mtxWorldModel, &enemy.mtxWorldModel, &mtxRot);

	// �ړ��𔽉f
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