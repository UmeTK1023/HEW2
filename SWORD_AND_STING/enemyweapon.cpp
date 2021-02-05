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
// �}�N����`

//*****************************************************************************
#define MODEL_WEAPON   "asset/MODEL/BUKI1.x"	// �ǂݍ��ރ��f����
#define MOVE_MODEL_WEAPON (1.5f)				// �ړ����x
#define VALUE_MOVE  (5.0f)						// �ړ���
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void Enemy_StateInit();			//������
void Enemy_StateWait();			//�s���ҋ@
void Enemy_Statedirection();	//�U�������I��
void Enemy_StateLightwait();	//�U���������҂�
void Enemy_StateAttack();		//�U���s��
void Enemy_StateGuard();		//�K�[�h�s��
void Enemy_StateAvoid();		//����s��
void Enemy_StateCooldown();		//�s���s�\

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureEWeapon;	// �e�N�X�`���ւ̃|�C���^
LPD3DXMESH   g_pMeshEWeapon;			// ���b�V�����ւ̃|�C���^
LPD3DXBUFFER  g_pBuffMatEWeapon;		// �}�e���A�����ւ̃|�C���^
DWORD    g_nNumMatEWeapon;				// �}�e���A�����̑���

struct EWEAPON//�G�\����
{
	D3DXMATRIX   mtxWorldEModel;  // ���[���h�}�g���b�N�X
	D3DXVECTOR3   posEModel;   // ���݂̈ʒu
	D3DXVECTOR3   rotEModel;   // ���݂̌���
	D3DXVECTOR3   rotDestEModel;  // �ړI�̌���
	D3DXVECTOR3   moveEModel;   // �ړ���
	int state;
};
EWEAPON enemy;

static int    FlameCount;	// �t���[���J�E���g
static int    GuardCount;	// �K�[�h�̃t���[���J�E���g
static bool		ElightU;	// �G�̃��C�g�����
static bool		ElightR;	// �G�̃��C�g�E����
static bool		ElightL;	// �G�̃��C�g������
static bool    EatkU;		// ������U��
static bool    EatkR;		// �E�����U��
static bool    EatkL;		// �������U��
static bool    EguardU;		// ������K�[�h
static bool    EguardR;		// �E�����K�[�h
static bool    EguardL;		// �������K�[�h
static bool    Eavoid;		//���
static bool    hitE;		//�����蔻��
static int  g_ELightTable;	
static int randAtk;			// �U�����������_���ϐ�
static int randguard;       // �K�[�h���������_���ϐ�
static int randAct;			// �s�������_���ϐ�
float EnemyWmove;			// �G�̈ړ����x
int g_AtkAlpha;

typedef enum 
{
	ENEMY_STATE_INIT,		// ������
	ENEMY_STATE_WAIT,		// �ҋ@
	ENEMY_STATE_DIRECTION,	// �U�����������߂�
	ENEMY_STATE_LIGHTWAIT,	// �w�������܂ő҂�
	ENEMY_STATE_ATTACK,		// �U��
	ENEMY_STATE_GUARD,		// �K�[�h
	ENEMY_STATE_AVOID,		// ���
	ENEMY_STATE_COOLDOWN,	// �s���I��

	ENEMY_STATE_MAX

}ENEMY_STATE;

//=============================================================================
// ����������
//=============================================================================
HRESULT EnemyWeapon_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	g_pTextureEWeapon = NULL;
	g_pMeshEWeapon = NULL;
	g_pBuffMatEWeapon = NULL;

	//�ʒu�E�����E�ړ��ʂ̏����ݒ�
	enemy.posEModel = pos;
	enemy.rotEModel = rot;
	enemy.rotDestEModel = rot;
	enemy.moveEModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//X�t�@�C���̓ǂݍ���
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

	//�V�[�h�̏�����
	srand((unsigned int)time(NULL));

	enemy.state = ENEMY_STATE_INIT;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void EnemyWeapon_Finalize(void)
{
	if (g_pTextureEWeapon != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureEWeapon->Release();
		g_pTextureEWeapon = NULL;
	}

	if (g_pMeshEWeapon != NULL)
	{// ���b�V���̊J��
		g_pMeshEWeapon->Release();
		g_pMeshEWeapon = NULL;
	}

	if (g_pBuffMatEWeapon != NULL)
	{// �}�e���A���̊J��
		g_pBuffMatEWeapon->Release();
		g_pBuffMatEWeapon = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void EnemyWeapon_Update(void)
{
	D3DXVECTOR3 enemyPos = GetPosEnemy();
	D3DXVECTOR3 playerPos = GetPosPlayer();

	//��{�|�W�V����
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

	//�v���C���[�U�����q�b�g�����Ƃ��ɓ��_
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
		case 0://�U��
			FlameCount = 0;
			enemy.state = ENEMY_STATE_DIRECTION;
			break;
		case 1://�K�[�h
			FlameCount = 0;
			enemy.state = ENEMY_STATE_GUARD;
			break;
		case 2://���
			FlameCount = 0;
			enemy.state = ENEMY_STATE_AVOID;
			break;
		}
	}
}

void Enemy_Statedirection()
{
	//�U�����Ă��Ȃ�������U���\
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

	//�����蔻��p
	//������U�����A�v���C���[���K�[�h����������Ă��Ȃ���΃q�b�g
	if ((EatkU) && (!GetguardU())&&(!Getavoid())&& (D3DXVec3Length(&GetEdistance())<170))
	{
		hitE = true;
	}
	//�E�����U�����A�v���C���[���K�[�h����������Ă��Ȃ���΃q�b�g
	if ((EatkR) && (!GetguardR()) && (!Getavoid()) && (D3DXVec3Length(&GetEdistance())<170))
	{
		hitE = true;
	}
	//�������U�����A�v���C���[���K�[�h����������Ă��Ȃ���΃q�b�g
	if ((EatkL) && (!GetguardL()) && (!Getavoid()) && (D3DXVec3Length(&GetEdistance())<170))
	{
		hitE = true;
	}

	//������U�����A�v���C���[���K�[�h���Ă���Βe��������炷
	if ((EatkU) && (GetguardU()))
	{
		PlaySound(SOUND_LABEL_SE_GUARD);
	}
	//�E�����U�����A�v���C���[���K�[�h���Ă���Βe��������炷
	if ((EatkR) && (GetguardR()))
	{
		PlaySound(SOUND_LABEL_SE_GUARD);
	}
	//�������U�����A�v���C���[���K�[�h���Ă���Βe��������炷
	if ((EatkL) && (GetguardL()))
	{
		PlaySound(SOUND_LABEL_SE_GUARD);
	}

}

void Enemy_StateGuard()
{
	//�K�[�h���Ă��Ȃ�������K�[�h�\
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
	//����s��
	Eavoid = true;
	PlaySound(SOUND_LABEL_SE_AVOID);
	enemy.state = ENEMY_STATE_COOLDOWN;
}

void Enemy_StateCooldown()
{
	FlameCount++;

	//�U��������20�t���[���̊ԍU���s�\�ɂ���B
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
			FlameCount = 0;//�t���[���̏�����
			SetLighTable(0);
			SetEnemyAtkTable(0);
			enemy.state = ENEMY_STATE_WAIT;
		}
	}
	//�K�[�h������20�t���[���̊ԃK�[�h�s�\�ɂ���B
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
			FlameCount = 0;//�t���[���̏�����
			enemy.state = ENEMY_STATE_WAIT;
		}
	}
	//���������20�t���[���̊ԉ��s�\�ɂ���B
	if ((Eavoid) && (FlameCount <= 20))
	{
		if (FlameCount >= 20)
		{
			Eavoid = false;
			FlameCount = 0;//�t���[���̏�����
			enemy.state = ENEMY_STATE_WAIT;
		}
	}

}


//=============================================================================
// �`�揈��
//=============================================================================
void EnemyWeapon_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&enemy.mtxWorldEModel);

	//��]�̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, enemy.rotEModel.y, enemy.rotEModel.x, enemy.rotEModel.z);
	D3DXMatrixMultiply(&enemy.mtxWorldEModel, &enemy.mtxWorldEModel, &mtxRot);

	//�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, enemy.posEModel.x, enemy.posEModel.y, enemy.posEModel.z);
	D3DXMatrixMultiply(&enemy.mtxWorldEModel, &enemy.mtxWorldEModel, &mtxTranslate);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &enemy.mtxWorldEModel);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)g_pBuffMatEWeapon->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatEWeapon; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureEWeapon);

		//�`��
		g_pMeshEWeapon->DrawSubset(nCntMat);
	}
	//�}�e���A�����f�t�H���g�ɖ߂�
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