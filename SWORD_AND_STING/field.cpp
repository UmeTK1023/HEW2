#include "field.h"
#include "myDirect3D.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VALUE_MOVE		(5.0f)							// �ړ���
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)				// ��]��

#define	FIELD_WIDTH		(50.0f)							// �n�ʂ̕�(X����)
#define FIELD_DEPTH		(500.0f)						// �n�ʂ̉��s(Z����)
#define FIELD_HEIGHT	(0.0f)					

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

D3DXMATRIX				g_mtxWorldField;		// ���[���h�}�g���b�N�X
D3DXVECTOR3				g_posField;				// �n�ʂ̈ʒu
D3DXVECTOR3				g_rotField;				// �n�ʂ̌���(��])
D3DXVECTOR3				g_sclField;				// �n�ʂ̑傫��(�X�P�[��)

//=============================================================================
// ����������
//=============================================================================
HRESULT Field_Initialize(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	// ���_���̍쐬
	MakeVertexField(pDevice);

	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_posField = D3DXVECTOR3(0.0f, -50.0f, 0.0f);
	g_rotField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_sclField = D3DXVECTOR3(1.5f, 1.5f, 1.5f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Field_Finalize(void)
{
	if (g_pVtxBuffField != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Field_Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void Field_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	//���[���h�}�g���b�N�X�̏������A�P�ʍs����쐬
	D3DXMatrixIdentity(&g_mtxWorldField);//���l�c�l

										 //�X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, g_sclField.x, g_sclField.y, g_sclField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxScl);//�s��̊|���Z�����Ă����֐��@����2.3���|���Z���A����1�ɓ���������

																	//��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);

	//�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTexture(NULL, NULL/*Texture_GetTexture(TEXTURE_INDEX_FIELD01)*/);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffField, NULL)))
	{
		return E_FAIL;
	}
	//CreateVertexBuffer=VRAM�ɒ��_�f�[�^�����
	{
		VERTEX_3D *pVtx;//���_�f�[�^

		g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);//�������ɒ��_�f�[�^�������Ă��āA���_�f�[�^���������ĕԂ��B���̃��b�N�ő�������������ƌy���Ȃ�B

		pVtx[0].pos = D3DXVECTOR3(-FIELD_WIDTH, FIELD_HEIGHT, FIELD_DEPTH);
		pVtx[1].pos = D3DXVECTOR3(FIELD_WIDTH, FIELD_HEIGHT, FIELD_DEPTH);
		pVtx[2].pos = D3DXVECTOR3(-FIELD_WIDTH, FIELD_HEIGHT, -FIELD_DEPTH);
		pVtx[3].pos = D3DXVECTOR3(FIELD_WIDTH, FIELD_HEIGHT, -FIELD_DEPTH);


		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		g_pVtxBuffField->Unlock();
	}

	return S_OK;
}

