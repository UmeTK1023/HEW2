#include "field.h"
#include "myDirect3D.h"
#include "input.h"
#include "texture.h"
#include "debugproc.h"
#include "player.h"
#include "camera.h"
#include "ui.h"
#include "enemy.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_MOVE		(5.0f)							// �ړ���
#define	VALUE_PLAYER_MOVE		(1.5f)							// �ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)				// ��]��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

D3DXMATRIX				g_mtxWorldPlayer;		// ���[���h�}�g���b�N�X
D3DXVECTOR3				g_posPlayer;			// �v���C���[�̈ʒu
D3DXVECTOR3				g_rotPlayer;			// �v���C���[�̌���(��])
D3DXVECTOR3				g_sclPlayer;			// �v���C���[�̑傫��(�X�P�[��)
D3DXVECTOR3				Pdistance;
int UiHealthTable;
int FlameCount;									// �t���[���J�E���g
float PlayerMove;

//=============================================================================
// ����������
//=============================================================================
HRESULT Player_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexPlayer(pDevice);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_posPlayer = D3DXVECTOR3(0.0f, 0.0f, -100.0f);
	g_rotPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclPlayer = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	FlameCount = 0;
	Pdistance = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	PlayerMove = VALUE_PLAYER_MOVE;
	UiHealthTable = GetTable();
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Player_Finalize(void)
{
	if (g_pVtxBuffPlayer != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Player_Update(void)
{
	UiHealthTable = GetTable();
	D3DXVECTOR3 EnemyPos = GetPosEnemy();

	Pdistance = g_posPlayer - EnemyPos;

	//JoyCon�̏ꍇ�A�i���O�X�e�B�b�N�����
	if (keyboard.IsPress(DIK_W)||(joycon[0].GetStick_Y() > 0.0f))
	{// ���ړ�
		if (D3DXVec3Length(&Pdistance) > 100)
		{
			g_posPlayer.x += sinf(D3DX_PI * 1.0f) * VALUE_MOVE;
			g_posPlayer.z -= cosf(D3DX_PI * 1.0f) * VALUE_MOVE;
		}

	}
	//JoyCon�̏ꍇ�A�i���O�X�e�B�b�N������
	else if (keyboard.IsPress(DIK_S) || (joycon[0].GetStick_Y() < 0.0f))
	{// ���ړ�
		if (g_posPlayer.z > -300)
		{
			g_posPlayer.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE;
			g_posPlayer.z -= cosf(D3DX_PI * 0.0f) * VALUE_MOVE;
		}
	}
	if (UiHealthTable < 4)//�̗͂��Ȃ��Ƃ��͉���s�����o���Ȃ�
	{
		//JoyCon�̏ꍇ�a�{�^���\��
		if ((keyboard.IsTrigger(DIK_Z)) || (joycon[0].IsTrigger(JOYCON_ZR)))
		{//����s��
			g_posPlayer.x += sinf(D3DX_PI * 0.0f) * VALUE_MOVE;
			g_posPlayer.z -= cosf(D3DX_PI * 0.0f) * (VALUE_MOVE + 10.0f);
		}
	}
	FlameCount++;
}

//=============================================================================
// �`�揈��
//=============================================================================
void Player_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	//���[���h�}�g���b�N�X�̏������A�P�ʍs����쐬
	D3DXMatrixIdentity(&g_mtxWorldPlayer);//���l�c�l

	//�X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, g_sclPlayer.x, g_sclPlayer.y, g_sclPlayer.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxScl);//�s��̊|���Z�����Ă����֐��@����2.3���|���Z���A����1�ɓ���������

	//��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotPlayer.y, g_rotPlayer.x, g_rotPlayer.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxRot);

	//�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_posPlayer.x, g_posPlayer.y, g_posPlayer.z);
	D3DXMatrixMultiply(&g_mtxWorldPlayer, &g_mtxWorldPlayer, &mtxTranslate);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldPlayer);

	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_3D));

	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTexture(0, Texture_GetTexture(TEXTURE_INDEX_PLAYER));

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPlayer(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffPlayer, NULL)))
	{
		return E_FAIL;
	}
	//CreateVertexBuffer=VRAM�ɒ��_�f�[�^�����
	{
		VERTEX_3D *pVtx;//���_�f�[�^

		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);//�������ɒ��_�f�[�^�������Ă��āA���_�f�[�^���������ĕԂ��B���̃��b�N�ő�������������ƌy���Ȃ�B

		//////////////////////////////��ʂ̎l�p�`/////////////////////////////////
		pVtx[0].pos = D3DXVECTOR3(-PLAYER_WIDTH, 0, PLAYER_DEPTH);
		pVtx[1].pos = D3DXVECTOR3(PLAYER_WIDTH, 0, PLAYER_DEPTH);
		pVtx[2].pos = D3DXVECTOR3(-PLAYER_WIDTH, 0, -PLAYER_DEPTH);
		pVtx[3].pos = D3DXVECTOR3(PLAYER_WIDTH, 0, -PLAYER_DEPTH);


		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		////////////////////////////////////////////////////////////////////////////

		//�E���ʂ̎l�p
		pVtx[4].pos = D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT, -PLAYER_DEPTH);
		pVtx[5].pos = D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_DEPTH);
		pVtx[6].pos = D3DXVECTOR3(PLAYER_WIDTH, 0, -PLAYER_DEPTH);
		pVtx[7].pos = D3DXVECTOR3(PLAYER_WIDTH, 0, PLAYER_DEPTH);

		pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[4].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[5].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[6].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		pVtx[7].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

		pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);
		//////////////////////////////////////////////////////////////////////////

		//���̎l�p
		pVtx[8].pos = pVtx[5].pos;
		pVtx[9].pos = D3DXVECTOR3(-PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_DEPTH);
		pVtx[10].pos = D3DXVECTOR3(PLAYER_WIDTH, 0, PLAYER_DEPTH);
		pVtx[11].pos = D3DXVECTOR3(-PLAYER_WIDTH, 0, PLAYER_DEPTH);

		pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[9].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[10].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[11].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[8].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[9].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[10].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		pVtx[11].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

		pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);
		////////////////////////////////////////////////////////////////////////

		//�����ʂ̎l�p
		pVtx[12].pos = pVtx[9].pos;
		pVtx[13].pos = D3DXVECTOR3(-PLAYER_WIDTH, PLAYER_HEIGHT, -PLAYER_DEPTH);
		pVtx[14].pos = D3DXVECTOR3(-PLAYER_WIDTH, 0, PLAYER_DEPTH);
		pVtx[15].pos = D3DXVECTOR3(-PLAYER_WIDTH, 0, -PLAYER_DEPTH);

		pVtx[12].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[13].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[14].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[15].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[12].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[13].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[14].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[15].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

		pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);
		////////////////////////////////////////////////////////////////////////

		//��O�̎l�p
		pVtx[16].pos = pVtx[13].pos;
		pVtx[17].pos = D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT, -PLAYER_DEPTH);
		pVtx[18].pos = D3DXVECTOR3(-PLAYER_WIDTH, 0, -PLAYER_DEPTH);
		pVtx[19].pos = D3DXVECTOR3(PLAYER_WIDTH, 0, -PLAYER_DEPTH);

		pVtx[16].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[17].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[18].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[19].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[16].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[17].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[18].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[19].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[16].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[17].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[18].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[19].tex = D3DXVECTOR2(1.0f, 1.0f);
		///////////////////////////////////////////////////////////////////////

		//��ʂ̎l�p
		pVtx[20].pos = D3DXVECTOR3(-PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_DEPTH);
		pVtx[21].pos = D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_DEPTH);
		pVtx[22].pos = D3DXVECTOR3(-PLAYER_WIDTH, PLAYER_HEIGHT, -PLAYER_DEPTH);
		pVtx[23].pos = D3DXVECTOR3(PLAYER_WIDTH, PLAYER_HEIGHT, -PLAYER_DEPTH);


		pVtx[20].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[21].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[22].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[23].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


		pVtx[20].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[21].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[22].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[23].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

		pVtx[20].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[21].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[22].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[23].tex = D3DXVECTOR2(1.0f, 1.0f);
		g_pVtxBuffPlayer->Unlock();
	}

	return S_OK;
}


D3DXVECTOR3 GetPosPlayer()
{
	return g_posPlayer;
}

D3DXMATRIX GetMatrixPlayer()
{
	return g_mtxWorldPlayer;
}

D3DXVECTOR3 GetPdistance()
{
	return Pdistance;
}