
#pragma once

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif
// �Q�[���̏������֐�
// �߂�l:�������Ɏ��s�����Ƃ�false
bool D3D_Initialize(HWND hWnd);

// �Q�[���̏I������
void D3D_Finalize(void);

//Direct3DDevice�̃Q�b�^�[
LPDIRECT3DDEVICE9 GetD3DDevice();
