#include "myDirect3D.h"
#include "CD3DXFrame.h"
//#include "DXUtil.h"

CD3DXFrame::CD3DXFrame()
{
	// D3DXFRAME �R���̕ϐ���������
	Name = NULL;
	D3DXMatrixIdentity( &TransformationMatrix );
	pMeshContainer = NULL;
	pFrameSibling = NULL;
	pFrameFirstChild = NULL;

	// �����o�ϐ�������
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
