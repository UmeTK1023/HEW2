#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <chrono>
#include <cmath>
#include <iostream>
#define SCREEN_WIDTH  (1920)              // �X�N���[��(�N���C�A���g�̈�)�̕�
#define SCREEN_HEIGHT  (1080)              // �X�N���[��(�N���C�A���g�̈�)�̍���

#define	NUM_VERTEX		(24)		// ���_��
#define	NUM_POLYGON		(22)		// �|���S����

// ��L�Q�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	float rhw;			// ���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
} VERTEX_2D;

// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���_�J���[ / �e�N�X�`�����W )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// ��L�R�c�|���S�����_�t�H�[�}�b�g�ɍ��킹���\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
} VERTEX_3D;

// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@�� / ���_�J���[ / �e�N�X�`�����W )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//�E�B���h�E�n���h����Getter
HWND GetHWND();
double frand();
//D3DXVECTOR2 GetAnimTbl(int set, int muki, int anim);

//class iTime {
//public:
//	///Time it took to render previous frame
//	float DeltaTime;
//
//	iTime() {
//		DeltaTime = 1;
//	}
//
//	void Start() {
//		t1 = std::chrono::high_resolution_clock::now();
//	}
//	void End() {
//		t2 = std::chrono::high_resolution_clock::now();
//		DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
//		DeltaTime /= 1000000;
//	}
//private:
//	std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;
//
//};
//
//extern iTime Time;