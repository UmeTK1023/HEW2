#include "myDirect3D.h"
#include "CD3DXObject.h"
#include "CAllocateHierarchy.h"
#include "CD3DXFrame.h"
#include "CD3DXMesh.h"
//#include "DXUtil.h"


CD3DXObject::CD3DXObject()
{
	NumBoneMatricesMax	= 0;
	pBoneMatrices		= NULL;
	pFrameRoot			= NULL;
	pAnimController		= NULL;
	D3DXMatrixIdentity( &matWorld );

	Time = 0;
}

CD3DXObject::~CD3DXObject()
{
	Release(); // �����[�X�Y��h�~
}

bool CD3DXObject::Load( char *pFileName )
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();;

    CAllocateHierarchy Alloc( this );

    LPD3DXANIMATIONCONTROLLER pAnimControllerTmp;
    if( FAILED( D3DXLoadMeshHierarchyFromX(
					pFileName, D3DXMESH_MANAGED, pDevice,
					&Alloc, NULL, &pFrameRoot, &pAnimControllerTmp ) ) ) return false;

	// ���֐����l���āA�g���b�N�����A�j���[�V�����Z�b�g���ɑ��₷
	int TracksNum   = pAnimControllerTmp->GetMaxNumTracks();
	int AnimSetsNum = pAnimControllerTmp->GetMaxNumAnimationSets();
	if( TracksNum < AnimSetsNum ) TracksNum = AnimSetsNum;

	if( FAILED( pAnimControllerTmp->CloneAnimationController(
		pAnimControllerTmp->GetMaxNumAnimationOutputs(),
//		pAnimControllerTmp->GetMaxNumMatrices(),
		pAnimControllerTmp->GetMaxNumAnimationSets(),
		TracksNum,
		pAnimControllerTmp->GetMaxNumEvents(),
		&pAnimController ) ) ) return false;

	SAFE_RELEASE( pAnimControllerTmp );

	if( AnimSetsNum > 1 ){
		// �g���b�N�ɑS�ẴA�j���[�V�����Z�b�g��ǂݍ���
		for( int i = 1; i < AnimSetsNum; i++ ){
			LPD3DXANIMATIONSET pAnimSet = NULL;
			if( FAILED( pAnimController->GetAnimationSet( i, &pAnimSet ) ) ) return false;
			if( FAILED( pAnimController->SetTrackAnimationSet( i, pAnimSet ) ) ) return false;
			SAFE_RELEASE( pAnimSet );

			// �S�Ẵg���b�N�Ƀf�t�H���g��D3DXTRACK_DESC��ݒ肵�Ă���
//			D3DXTRACK_DESC Desc = { 0, 1, 1, FALSE, 0 };
			D3DXTRACK_DESC Desc = { D3DXPRIORITY_LOW, 1, 1, 0, FALSE };
			if( FAILED( pAnimController->SetTrackDesc( i, &Desc ) ) ) return false;
		}
	}

    if( FAILED( SetupBoneMatrixPointers( pFrameRoot ) ) ) return false;

	return true;
}

void CD3DXObject::Release()
{
	if( pFrameRoot ){
		CAllocateHierarchy Alloc( this );
		D3DXFrameDestroy( pFrameRoot, &Alloc );
		pFrameRoot = NULL;
	}

	SAFE_DELETE_ARRAY( pBoneMatrices );
	SAFE_RELEASE( pAnimController );
}

HRESULT CD3DXObject::GenerateSkinnedMesh( CD3DXMesh *pCD3DXMesh )
{
	HRESULT hr = S_OK;

	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	if( !pCD3DXMesh->pSkinInfo ) return hr;
	SAFE_RELEASE( pCD3DXMesh->MeshData.pMesh );
	SAFE_RELEASE( pCD3DXMesh->pBoneCombinationBuf );

	hr = pCD3DXMesh->pOrigMesh->CloneMeshFVF(
			D3DXMESH_MANAGED, pCD3DXMesh->pOrigMesh->GetFVF(),
			pDevice, &pCD3DXMesh->MeshData.pMesh );
	if( FAILED( hr ) ) goto e_Exit;

	hr = pCD3DXMesh->MeshData.pMesh->GetAttributeTable( NULL, &pCD3DXMesh->NumAttributeGroups );
	if( FAILED( hr ) ) goto e_Exit;

	delete[] pCD3DXMesh->pAttributeTable;
	pCD3DXMesh->pAttributeTable = new D3DXATTRIBUTERANGE[pCD3DXMesh->NumAttributeGroups];
	if( !pCD3DXMesh->pAttributeTable ){
		hr = E_OUTOFMEMORY;
		goto e_Exit;
	}

	hr = pCD3DXMesh->MeshData.pMesh->GetAttributeTable( pCD3DXMesh->pAttributeTable, NULL );
	if( FAILED( hr ) ) goto e_Exit;

	// ���̃��b�V���ɂ���ă{�[���s��p�̃��������m�ۂ�����Ă��Ȃ��ꍇ�͊m��
	if( NumBoneMatricesMax < pCD3DXMesh->pSkinInfo->GetNumBones() ){
		NumBoneMatricesMax = pCD3DXMesh->pSkinInfo->GetNumBones();

		delete []pBoneMatrices;
		pBoneMatrices = new D3DXMATRIXA16[NumBoneMatricesMax];
		if( !pBoneMatrices ){
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}
    }

e_Exit:
    return hr;
}

void CD3DXObject::DrawMeshContainer( LPD3DXMESHCONTAINER pD3DXMeshContainer, LPD3DXFRAME pD3DXFrame )
{
	CD3DXMesh *pCD3DXMesh = (CD3DXMesh*)pD3DXMeshContainer;
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	if( pCD3DXMesh->pSkinInfo ){ // �X�L�����b�V���̏ꍇ
		DWORD NumBones = pCD3DXMesh->pSkinInfo->GetNumBones();
		{
			for( DWORD i = 0; i < NumBones; i++ ){
				D3DXMatrixMultiply(
					&pBoneMatrices[i],
					&pCD3DXMesh->pBoneOffsetMatrices[i], 
					pCD3DXMesh->ppBoneMatrixPtrs[i]
				);
			}
		}

		// ���[���h�s����N���A
		D3DXMATRIX Identity;
		D3DXMatrixIdentity( &Identity );
		pDevice->SetTransform( D3DTS_WORLD, &Identity );

		// ���_�o�b�t�@�����b�N
		PBYTE pVerticesSrc;
		PBYTE pVerticesDest;
		pCD3DXMesh->pOrigMesh->LockVertexBuffer( D3DLOCK_READONLY, (LPVOID*)&pVerticesSrc );
		pCD3DXMesh->MeshData.pMesh->LockVertexBuffer( 0, (LPVOID*)&pVerticesDest );

		// �X�L�����b�V���쐬
		pCD3DXMesh->pSkinInfo->UpdateSkinnedMesh( pBoneMatrices, NULL, pVerticesSrc, pVerticesDest );

		// ���_�o�b�t�@�̃��b�N������
		pCD3DXMesh->pOrigMesh->UnlockVertexBuffer();
		pCD3DXMesh->MeshData.pMesh->UnlockVertexBuffer();

		// �X�L�����b�V���`��
		for( UINT i = 0; i < pCD3DXMesh->NumAttributeGroups; i++ ){
			pDevice->SetMaterial( &( pCD3DXMesh->pMaterials[pCD3DXMesh->pAttributeTable[i].AttribId].MatD3D ) );
			pDevice->SetTexture( 0, pCD3DXMesh->ppTextures[pCD3DXMesh->pAttributeTable[i].AttribId] );
			pCD3DXMesh->MeshData.pMesh->DrawSubset( pCD3DXMesh->pAttributeTable[i].AttribId );
		}
	}
	else{ // �ʏ�̃��b�V���̏ꍇ
		pDevice->SetTransform( D3DTS_WORLD, &( (CD3DXFrame *)pD3DXFrame )->CombinedTransformationMatrix );

		// ���b�V���`��
		for ( DWORD i = 0; i < pCD3DXMesh->NumMaterials; i++ ){
			pDevice->SetMaterial( &pCD3DXMesh->pMaterials[i].MatD3D );
			pDevice->SetTexture( 0, pCD3DXMesh->ppTextures[i] );
			pCD3DXMesh->MeshData.pMesh->DrawSubset( i );
		}
	}

}

void CD3DXObject::DrawFrame( LPD3DXFRAME pD3DXFrame )
{
	LPD3DXMESHCONTAINER pD3DXMeshContainer;

	pD3DXMeshContainer = pD3DXFrame->pMeshContainer;
	while( pD3DXMeshContainer ){
		DrawMeshContainer( pD3DXMeshContainer, pD3DXFrame );
		pD3DXMeshContainer = pD3DXMeshContainer->pNextMeshContainer;
	}

	if( pD3DXFrame->pFrameSibling ) DrawFrame( pD3DXFrame->pFrameSibling );
	if( pD3DXFrame->pFrameFirstChild ) DrawFrame( pD3DXFrame->pFrameFirstChild );
}

void CD3DXObject::Render()
{
	DrawFrame( pFrameRoot );
}

bool CD3DXObject::SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pD3DXMeshContainer )
{
	CD3DXMesh *pCD3DXMesh = (CD3DXMesh*)pD3DXMeshContainer;

	// �X�L�����b�V���̂Ƃ��̓{�[���s����Z�b�g
	if( pCD3DXMesh->pSkinInfo ){
		DWORD NumBones = pCD3DXMesh->pSkinInfo->GetNumBones();

		pCD3DXMesh->ppBoneMatrixPtrs = new D3DXMATRIX*[NumBones];
		if( !pCD3DXMesh->ppBoneMatrixPtrs ) return false;

		for( DWORD i = 0; i < NumBones; i++ ){
			CD3DXFrame *pCD3DXFrame = (CD3DXFrame*)D3DXFrameFind(
				pFrameRoot, pCD3DXMesh->pSkinInfo->GetBoneName( i ) );
			if( !pCD3DXFrame ) return false;

			pCD3DXMesh->ppBoneMatrixPtrs[i] = &pCD3DXFrame->CombinedTransformationMatrix;
		}
	}

	return true;
}


bool CD3DXObject::SetupBoneMatrixPointers( LPD3DXFRAME pD3DXFrame )
{
	if( pD3DXFrame->pMeshContainer ){
		if( FAILED( SetupBoneMatrixPointersOnMesh( pD3DXFrame->pMeshContainer ) ) ) return false;
	}

	if( pD3DXFrame->pFrameSibling ){
		if( FAILED( SetupBoneMatrixPointers( pD3DXFrame->pFrameSibling ) ) ) return false;
	}

	if( pD3DXFrame->pFrameFirstChild ){
		if( FAILED( SetupBoneMatrixPointers( pD3DXFrame->pFrameFirstChild ) ) ) return false;
	}

	return true;
}

void CD3DXObject::UpdateFrameMatrices( LPD3DXFRAME pD3DXMesh, LPD3DXMATRIX pParentMatrix )
{
	CD3DXFrame *pCD3DXFrame = (CD3DXFrame*)pD3DXMesh;

	if( pParentMatrix ){
		D3DXMatrixMultiply(
			&pCD3DXFrame->CombinedTransformationMatrix, &pCD3DXFrame->TransformationMatrix, pParentMatrix );
	}
	else pCD3DXFrame->CombinedTransformationMatrix = pCD3DXFrame->TransformationMatrix;

	if( pCD3DXFrame->pFrameSibling ) UpdateFrameMatrices( pCD3DXFrame->pFrameSibling, pParentMatrix );

	if( pCD3DXFrame->pFrameFirstChild ) UpdateFrameMatrices( pCD3DXFrame->pFrameFirstChild, &pCD3DXFrame->CombinedTransformationMatrix );
}

void CD3DXObject::Update( float Time )
{
	LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

	if( pAnimController ){
		pAnimController->AdvanceTime( Time - this->Time, NULL );
		this->Time = Time;
	}

	pDevice->SetTransform( D3DTS_WORLD, &matWorld );

	UpdateFrameMatrices( pFrameRoot, &matWorld );
}

// �ȉ����[�e�B���e�B�֐��Q
D3DXMATRIX& CD3DXObject::GetMatrix()
{
	return matWorld;
}

bool CD3DXObject::SetTrackEnable( DWORD Track, BOOL Enable )
{
	if( FAILED( pAnimController->SetTrackEnable( Track, Enable ) ) ){
		return false;
	}

	return true;
}

bool CD3DXObject::SetTrackSpeed( DWORD Track, FLOAT Speed )
{
	if( FAILED( pAnimController->SetTrackSpeed( Track, Speed ) ) ){
		return false;
	}

	return true;
}

bool CD3DXObject::SetTrackWeight( DWORD Track, FLOAT Weight )
{
	if( FAILED( pAnimController->SetTrackWeight( Track, Weight ) ) ){
		return false;
	}

	return true;
}
