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
// �}�N����`
//*****************************************************************************
#define MODEL_WEAPON   "asset/MODEL/BUKI1.x"				// �ǂݍ��ރ��f����
#define	VALUE_MOVE_WEAPON		(5.0f)						// �ړ���
#define	VALUE_ROTATE_WEAPON	(D3DX_PI * 0.02f)				// ��]��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureWeapon; // �e�N�X�`���ւ̃|�C���^
LPD3DXMESH   g_pMeshWeapon;  // ���b�V�����ւ̃|�C���^
LPD3DXBUFFER  g_pBuffMatWeapon; // �}�e���A�����ւ̃|�C���^
DWORD    g_nNumMatWeapon;  // �}�e���A�����̑���

struct WEAPON
{
	D3DXMATRIX   mtxWorldModel;  // ���[���h�}�g���b�N�X
	D3DXVECTOR3   posModel;   // ���݂̈ʒu
	D3DXVECTOR3   rotModel;   // ���݂̌���
	float rotspeed;
	D3DXVECTOR3   rotDestModel;  // �ړI�̌���
	D3DXVECTOR3   moveModel;   // �ړ���
};

WEAPON weapon;
static int	FlameCount;		// �t���[���J�E���g
static int	GuardFrame;		// �K�[�h�t���[���J�E���g
static int  AvoidFrame;		// ����t���[���J�E���g
static bool	atkU;			// ������U��
static bool	atkR;			// �E�����U��
static bool	atkL;			// �������U��
static bool	guardU;			// ������K�[�h
static bool	guardR;			// �E�����K�[�h
static bool	guardL;			// �������K�[�h
static int  UiHealthTable;	// �̗̓e�[�u���i�H�j
static int  UiGuardTable;	// �K�[�h�e�[�u���i�H�j
static int  g_LightTable;	// ���C�g�e�[�u���i�H�j
static bool hit;			// �����蔻��
static bool Avoid;			// ���
//=============================================================================
// ����������
//=============================================================================
HRESULT Weapon_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();


	g_pTextureWeapon = NULL;
	g_pMeshWeapon = NULL;
	g_pBuffMatWeapon = NULL;
	//�ʒu�E�����E�ړ��ʂ̏����ݒ�
	weapon.posModel = pos;
	weapon.rotModel = rot;
	weapon.rotDestModel = rot;
	weapon.moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//X�t�@�C���̓ǂݍ���
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
// �I������
//=============================================================================
void Weapon_Finalize(void)
{
	if (g_pTextureWeapon != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureWeapon->Release();
		g_pTextureWeapon = NULL;
	}

	if (g_pMeshWeapon != NULL)
	{// ���b�V���̊J��
		g_pMeshWeapon->Release();
		g_pMeshWeapon = NULL;
	}

	if (g_pBuffMatWeapon != NULL)
	{// �}�e���A���̊J��
		g_pBuffMatWeapon->Release();
		g_pBuffMatWeapon = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Weapon_Update(void)
{
	UiHealthTable = GetTable();
	UiGuardTable = GetGuardTable();
	D3DXVECTOR3 enemyPos = GetPosEnemy();
	D3DXVECTOR3 playerPos = GetPosPlayer();

	//��{�|�W�V����
	if (!atkU && !atkR && !atkL && !guardU && !guardR && !guardL)
		weapon.posModel.z = playerPos.z+85;

	//// ���ړ�
	//if (Keyboard_IsPress(DIK_W))
	//{
	//	weapon.posModel.x += sinf(D3DX_PI * 1.0f) * VALUE_MOVE_WEAPON;
	//	weapon.posModel.z -= cosf(D3DX_PI * 1.0f) * VALUE_MOVE_WEAPON;

	//}
	//else if (Keyboard_IsPress(DIK_S))
	//{// ��O�ړ�
	//	weapon.posModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON;
	//	weapon.posModel.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON;
	//}
	/*else*/ if (keyboard.IsTrigger(DIK_R))
	{
		weapon.rotModel.y -= 0.1f;
	}

	//�U�����Ă��Ȃ�������U���\
	if ((!Avoid) && (!atkU) && (!atkR) && (!atkL) && (UiHealthTable < 4))
	{
		//�X�y�[�X�L�[&���L�[�ŏ�����U���A�����O�ɏo��
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
	//�U�����Ă��Ȃ�������U���\
	if ((!Avoid) && (!atkU) && (!atkR) && (!atkL) && (UiHealthTable < 4))
	{
		//�X�y�[�X�L�[&���L�[�ŉE�����U���A�����O�ɏo��
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
	//�U�����Ă��Ȃ�������U���\
	if ((!Avoid) && (!atkU) && (!atkR) && (!atkL)&& (UiHealthTable < 4))
	{
		if (keyboard.IsTrigger(DIK_SPACE) && keyboard.IsPress(DIK_LEFTARROW) || (!joycon[0].IsPress(JOYCON_R))&&(joycon[0].GetGyro_Z() <= -0.9f))
		{
			//�X�y�[�X�L�[&���L�[�ō������U���A�����O�ɏo��
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

	//�U��������20�t���[���ԍU���s�\�ɂ���B
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
			FlameCount = 0;//�t���[���̏�����
			g_LightTable = 0;
		}
	}

	//�K�[�h�Q�[�W���Ȃ��Ƃ��̓K�[�h�s�����o���Ȃ�
	if (UiGuardTable < 4)
	{
		//�K�[�h���Ă��Ȃ�������K�[�h�\
		if ((!guardU) && (!guardR) && (!guardL))
		{
			//�X�y�[�X�L�[&�h�L�[�ŏ�����K�[�h�A�����O�ɏo��
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
		//�K�[�h���Ă��Ȃ�������K�[�h�\
		if ((!guardU) && (!guardR) && (!guardL))
		{
			//�X�y�[�X�L�[&�k�L�[�ŉE�����K�[�h�A�����O�ɏo��
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
		//�K�[�h���Ă��Ȃ�������K�[�h�\
		if ((!guardU) && (!guardR) && (!guardL))
		{
			//�X�y�[�X�L�[&�i�L�[�ō������K�[�h�A�����O�ɏo��
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

	//�K�[�h������20�t���[���ԃK�[�h�s�\�ɂ���B
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
			GuardFrame = 0;//�t���[���̏�����
		}

	}

	//���������Q�O�t���[�����s�\�ɂ���
	if ((Avoid) && (AvoidFrame <= 20))
	{
		AvoidFrame++;
		if (AvoidFrame >= 20)
		{
			Avoid = false;
			AvoidFrame = 0;//�t���[���̏�����
		}
	}

	//�̗͂��Ȃ��Ƃ��͉���s�����o���Ȃ�
	if ((!Avoid)&&(UiHealthTable < 4))
	{
		if (keyboard.IsTrigger(DIK_Z)||joycon[0].IsTrigger(JOYCON_ZR))
		{//����s��
			weapon.posModel.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE_WEAPON;
			weapon.posModel.z -= cosf(D3DX_PI * 0.0f) * (VALUE_MOVE_WEAPON + 10.0f);
			Avoid = true;
			PlaySound(SOUND_LABEL_SE_AVOID);
			AddTable(1);
		}
	}

	//�v���C���[�U�����q�b�g�����Ƃ��ɓ��_
	if (hit)
	{
		DebugProc_Print((char *)"�v���C���[�̍U�������m�F : ����\n");
		g_LightTable = 3;
		AddPlayerScore(1);
		PlaySound(SOUND_LABEL_SE_HIT);
		hit = false;
		if (GetPlayerScore() >= 5)
		{
			Scene_Change(SCENE_INDEX_RESULT);

		}
	}
	
	//�����ʒu�ɖ߂�
	//if (Keyboard_IsPress(DIK_RETURN))
	//{// ���Z�b�g
	//	weapon.posModel.x = PLAYER_WIDTH;
	//	weapon.posModel.y = PLAYER_HEIGHT / 2;
	//	weapon.posModel.z = -100.0f;
	//	weapon.rotModel.x = 0.0f;
	//	weapon.rotModel.y = -0.2f;
	//	weapon.rotModel.z = 0.0f;
	//}

}

//=============================================================================
// �`�揈��
//=============================================================================
void Weapon_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&weapon.mtxWorldModel);

	//��]�̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, weapon.rotModel.y, weapon.rotModel.x, weapon.rotModel.z);
	D3DXMatrixMultiply(&weapon.mtxWorldModel, &weapon.mtxWorldModel, &mtxRot);

	//�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, weapon.posModel.x, weapon.posModel.y, weapon.posModel.z);
	D3DXMatrixMultiply(&weapon.mtxWorldModel, &weapon.mtxWorldModel, &mtxTranslate);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &weapon.mtxWorldModel);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)g_pBuffMatWeapon->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatWeapon; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureWeapon);

		//�`��
		g_pMeshWeapon->DrawSubset(nCntMat);
	}
	//�}�e���A�����f�t�H���g�ɖ߂�
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