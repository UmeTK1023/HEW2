#include "myDirect3D.h"
#include "CAllocateHierarchy.h"
#include "CD3DXObject.h"
#include "CD3DXFrame.h"
#include "CD3DXMesh.h"
//#include "DXUtil.h"

CAllocateHierarchy::CAllocateHierarchy( CD3DXObject *pCD3DXObject )
{
	this->pCD3DXObject = pCD3DXObject;
}

HRESULT CAllocateHierarchy::CreateFrame( LPCTSTR Name, LPD3DXFRAME *ppNewFrame )
{
	*ppNewFrame = NULL;

	CD3DXFrame *pFrame = new CD3DXFrame;
	if( !pFrame ) return E_OUTOFMEMORY;

	pFrame->SetName( Name );
	*ppNewFrame = pFrame;

	return S_OK;
}

/*
HRESULT CAllocateHierarchy::CreateMeshContainer( LPCTSTR Name, LPD3DXMESHDATA pMeshData,
                            LPD3DXMATERIAL pMaterials, LPD3DXEFFECTINSTANCE pEffectInstances, DWORD NumMaterials, 
                            DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo, 
                            LPD3DXMESHCONTAINER *ppNewMeshContainer ) 
*/
HRESULT CAllocateHierarchy::CreateMeshContainer( THIS_ 
        LPCSTR Name, 
        CONST D3DXMESHDATA *pMeshData, 
        CONST D3DXMATERIAL *pMaterials, 
        CONST D3DXEFFECTINSTANCE *pEffectInstances, 
        DWORD NumMaterials, 
        CONST DWORD *pAdjacency, 
        LPD3DXSKININFO pSkinInfo, 
        LPD3DXMESHCONTAINER *ppNewMeshContainer )
{
	HRESULT hr = S_OK;

	*ppNewMeshContainer = NULL;

	// patch meshes ���������͂ł��Ȃ�
	if( pMeshData->Type != D3DXMESHTYPE_MESH ) return E_FAIL;

	LPD3DXMESH pMesh = pMeshData->pMesh;

	// FVF �ŋL�q���ꂽ���b�V���ȊO�͓ǂ߂܂���
	if( pMesh->GetFVF() == 0 ) return E_FAIL;
	UINT NumFaces = pMesh->GetNumFaces();

	// ���b�V���쐬
	CD3DXMesh *pCD3DXMesh = new CD3DXMesh;
	if( !pCD3DXMesh ) return E_OUTOFMEMORY;

	pCD3DXMesh->SetName( Name );
	pCD3DXMesh->MeshData.Type = D3DXMESHTYPE_MESH;

	LPDIRECT3DDEVICE9 pD3DDevice = NULL;
	if( FAILED( pMesh->GetDevice( &pD3DDevice ) ) ){
		hr = E_FAIL;
		goto e_Exit;
	}

	// X�t�@�C���ɖ@��������������v�Z�ŋ��߂�
	if( !( pMesh->GetFVF() & D3DFVF_NORMAL ) ){

		// FVF�ɖ@����ǉ������V�������b�V���ɂ���
        hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), 
                                  pMesh->GetFVF() | D3DFVF_NORMAL, 
                                  pD3DDevice, &pCD3DXMesh->MeshData.pMesh );
        if( FAILED( hr ) ) goto e_Exit;

		// �����œn���ꂽ���b�V���ւ̃|�C���^�ɐV�������b�V���ւ̃|�C���^���Z�b�g
		// pMesh�ւ̎Q�Ƃ͂��̎��_�ő��݂��Ȃ��̂ŁA�����ł�release�������Ȃ�
		pMesh = pCD3DXMesh->MeshData.pMesh;

		D3DXComputeNormals( pMesh, NULL );
	}
	else{
		// ���t�@�����X�𑝂₷����
		pCD3DXMesh->MeshData.pMesh = pMesh;
		pMesh->AddRef();
	}

	// �}�e���A���p�̃��������m��
	pCD3DXMesh->NumMaterials = max( 1, NumMaterials );
	pCD3DXMesh->pMaterials = new D3DXMATERIAL[pCD3DXMesh->NumMaterials];
	pCD3DXMesh->ppTextures = new LPDIRECT3DTEXTURE9[pCD3DXMesh->NumMaterials];
	pCD3DXMesh->pAdjacency = new DWORD[NumFaces * 3];
	if( !pCD3DXMesh->pAdjacency || !pCD3DXMesh->pMaterials ){
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	memcpy( pCD3DXMesh->pAdjacency, pAdjacency, sizeof( DWORD ) * NumFaces * 3 );
	memset( pCD3DXMesh->ppTextures, 0, sizeof( LPDIRECT3DTEXTURE9 ) * pCD3DXMesh->NumMaterials );

	// �}�e���A���̃R�s�[
	if( NumMaterials > 0 ){
		memcpy( pCD3DXMesh->pMaterials, pMaterials, sizeof( D3DXMATERIAL ) * NumMaterials );

		for( UINT i = 0; i < NumMaterials; i++ ){
			if( pCD3DXMesh->pMaterials[i].pTextureFilename ){
				if( FAILED( D3DXCreateTextureFromFile(
								pD3DDevice, pCD3DXMesh->pMaterials[i].pTextureFilename,
								&pCD3DXMesh->ppTextures[i] ) ) ) pCD3DXMesh->ppTextures[i] = NULL;

				// ���S�̂��߂�NULL�N���A
				pCD3DXMesh->pMaterials[i].pTextureFilename = NULL;
			}
		}
	}
	else{ // �}�e���A���������Ƃ��͓K���ɃZ�b�g
		pCD3DXMesh->pMaterials[0].pTextureFilename = NULL;
		memset( &pCD3DXMesh->pMaterials[0].MatD3D, 0, sizeof( D3DMATERIAL9 ) );
		pCD3DXMesh->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pCD3DXMesh->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pCD3DXMesh->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pCD3DXMesh->pMaterials[0].MatD3D.Specular = pCD3DXMesh->pMaterials[0].MatD3D.Diffuse;
	}

	if( pSkinInfo ){ // �����X�L���`����X�t�@�C��
		// pSkinInfo �� pMesh ���Z�b�g
		pCD3DXMesh->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pCD3DXMesh->pOrigMesh = pMesh;
		pMesh->AddRef();

		// Bone Offset Matrices �ۑ��p�̃��������m��
		UINT NumBones = pSkinInfo->GetNumBones();
		pCD3DXMesh->pBoneOffsetMatrices = new D3DXMATRIX[NumBones];
		if( !pCD3DXMesh->pBoneOffsetMatrices ){
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		// Bone Offset Matrices �ǂݍ���
		for( UINT i = 0; i < NumBones; i++ ){
			pCD3DXMesh->pBoneOffsetMatrices[i] = *( pCD3DXMesh->pSkinInfo->GetBoneOffsetMatrix( i ) );
		}

		// �X�L�����b�V���쐬
        hr = pCD3DXObject->GenerateSkinnedMesh( pCD3DXMesh );
        if( FAILED( hr ) ) goto e_Exit;
	}

	*ppNewMeshContainer = pCD3DXMesh;
	pCD3DXMesh = NULL;

e_Exit:
	SAFE_RELEASE( pD3DDevice );
	if( pCD3DXMesh ) DestroyMeshContainer( pCD3DXMesh );

	return hr;
}

HRESULT CAllocateHierarchy::DestroyFrame( LPD3DXFRAME pFrameToFree )
{
	CD3DXFrame *pCD3DXFrame = (CD3DXFrame*)pFrameToFree;
	SAFE_DELETE( pCD3DXFrame );

	return S_OK; 
}

HRESULT CAllocateHierarchy::DestroyMeshContainer( LPD3DXMESHCONTAINER pCD3DXMeshBase )
{
	CD3DXMesh *pCD3DXMesh = (CD3DXMesh*)pCD3DXMeshBase;
	SAFE_DELETE( pCD3DXMesh );

	return S_OK;
}
