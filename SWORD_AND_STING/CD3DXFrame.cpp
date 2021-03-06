#include "myDirect3D.h"
#include "CD3DXFrame.h"
//#include "DXUtil.h"

CD3DXFrame::CD3DXFrame()
{
	// D3DXFRAME 由来の変数を初期化
	Name = NULL;
	D3DXMatrixIdentity( &TransformationMatrix );
	pMeshContainer = NULL;
	pFrameSibling = NULL;
	pFrameFirstChild = NULL;

	// メンバ変数初期化
	D3DXMatrixIdentity( &CombinedTransformationMatrix );
}

CD3DXFrame::~CD3DXFrame()
{
	SAFE_DELETE( Name );
}

void CD3DXFrame::SetName( LPCTSTR Name )
{
	if( Name ){
		UINT Length;
		Length = lstrlen( Name ) + 1;
		SAFE_DELETE( this->Name );
		this->Name = new TCHAR[Length];
		memcpy( this->Name, Name, Length * sizeof( TCHAR ) );
	}
}
