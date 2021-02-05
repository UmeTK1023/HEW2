#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <chrono>
#include <cmath>
#include <iostream>
#define SCREEN_WIDTH  (1920)              // スクリーン(クライアント領域)の幅
#define SCREEN_HEIGHT  (1080)              // スクリーン(クライアント領域)の高さ

#define	NUM_VERTEX		(24)		// 頂点数
#define	NUM_POLYGON		(22)		// ポリゴン数

// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 座標変換用係数(1.0fで固定)
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_2D;

// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 頂点カラー / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 上記３Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_3D;

// ３Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線 / 頂点カラー / テクスチャ座標 )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//ウィンドウハンドルのGetter
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