#ifndef __CD3DXOBJECT_H__
#define __CD3DXOBJECT_H__

#include <d3dx9.h>
class CD3DXMesh;

class CD3DXObject
{
public:
	CD3DXObject();
	~CD3DXObject();
	void Release();
	bool Load( char *pFileName );
	HRESULT GenerateSkinnedMesh( CD3DXMesh *pCD3DXMesh );
	void Render();
	void Update( float Time );

	D3DXMATRIX& GetMatrix();
	bool SetTrackEnable( DWORD Track, BOOL Enable );
	bool SetTrackSpeed( DWORD Track, FLOAT Speed );
	bool SetTrackWeight( DWORD Track, FLOAT Weight );

private:
	void DrawMeshContainer( LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pD3DXFrame );
	void DrawFrame( LPD3DXFRAME pD3DXFrame );
	bool SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pD3DXMeshContainer );
	bool SetupBoneMatrixPointers( LPD3DXFRAME pD3DXFrame );
	void UpdateFrameMatrices( LPD3DXFRAME pD3DXFrame, LPD3DXMATRIX pParentMatrix );

private:
    UINT						NumBoneMatricesMax;
    D3DXMATRIXA16*				pBoneMatrices;
    LPD3DXFRAME					pFrameRoot;
    LPD3DXANIMATIONCONTROLLER	pAnimController;
	D3DXMATRIX					matWorld;

	DOUBLE Time;
};

#endif // __CD3DXOBJECT_H__
