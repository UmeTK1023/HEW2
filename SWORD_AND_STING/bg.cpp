#include "bg.h"
#include "myDirect3D.h"
#include "texture.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_BG   "asset/MODEL/bg.x"  // �ǂݍ��ރ��f����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBg; // �e�N�X�`���ւ̃|�C���^
LPD3DXMESH   g_pMeshBg;  // ���b�V�����ւ̃|�C���^
LPD3DXBUFFER  g_pBuffMatBg; // �}�e���A�����ւ̃|�C���^
DWORD    g_nNumMatBg;  // �}�e���A�����̑���

struct BG
{
	D3DXMATRIX   mtxWorldFModel;  // ���[���h�}�g���b�N�X
	D3DXVECTOR3   posFModel;   // ���݂̈ʒu
	D3DXVECTOR3   rotFModel;   // ���݂̌���
	D3DXVECTOR3   rotDestFModel;  // �ړI�̌���
	D3DXVECTOR3   moveFModel;   // �ړ���
	D3DXVECTOR3	  scaleFModel;//�傫��
};
BG Bg;

//=============================================================================
// ����������
//=============================================================================
HRESULT Bg_Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	g_pTextureBg = NULL;
	g_pMeshBg = NULL;
	g_pBuffMatBg = NULL;

	//�ʒu�E�����E�ړ��ʂ̏����ݒ�
	Bg.posFModel = pos;
	Bg.rotFModel = rot;
	Bg.rotDestFModel = rot;
	Bg.moveFModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	Bg.scaleFModel = D3DXVECTOR3(10.0f, 10.0f, 10.0f);

	//X�t�@�C���̓ǂݍ���
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
// �I������
//=============================================================================
void Bg_Finalize(void)
{
	if (g_pTextureBg != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureBg->Release();
		g_pTextureBg = NULL;
	}

	if (g_pMeshBg != NULL)
	{// ���b�V���̊J��
		g_pMeshBg->Release();
		g_pMeshBg = NULL;
	}

	if (g_pBuffMatBg != NULL)
	{// �}�e���A���̊J��
		g_pBuffMatBg->Release();
		g_pBuffMatBg = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void Bg_Update(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void Bg_Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxscl;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&Bg.mtxWorldFModel);

	//��]�̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, Bg.rotFModel.y, Bg.rotFModel.x, Bg.rotFModel.z);
	D3DXMatrixMultiply(&Bg.mtxWorldFModel, &Bg.mtxWorldFModel, &mtxRot);

	//�ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, Bg.posFModel.x, Bg.posFModel.y, Bg.posFModel.z);
	D3DXMatrixMultiply(&Bg.mtxWorldFModel, &Bg.mtxWorldFModel, &mtxTranslate);

	//�X�P�[���𔽉f
	D3DXMatrixScaling(&mtxscl, Bg.scaleFModel.x, Bg.scaleFModel.y, Bg.scaleFModel.z);
	D3DXMatrixMultiply(&Bg.mtxWorldFModel, &Bg.mtxWorldFModel, &mtxscl);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &Bg.mtxWorldFModel);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)g_pBuffMatBg->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_nNumMatBg; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBg);

		//�`��
		g_pMeshBg->DrawSubset(nCntMat);
	}
	//�}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

