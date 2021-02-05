#include <d3dx9.h>
//#include "debugPrintf.h"
#include "mydirect3d.h"
#include "texture.h"

#define TEXTURE_FILENAME_MAX (64)


typedef struct TextureFile_tag
{
	char filename[TEXTURE_FILENAME_MAX];
	int width;
	int height;
} TextureFile;

static const TextureFile g_TextureFiles[] = {
	{ "asset/texture/field000.jpg", 160, 160 },
	{ "asset/texture/player.png",189,222},
	{ "asset/texture/title2.jpg",1920,1080},
	{ "asset/texture/result.png",1920,1080},
	{ "asset/texture/load.png",1920,1080},
	{ "asset/texture/operation.png",1920,1080},
	{ "asset/texture/number.tga", 512, 512 },
	{ "asset/texture/Light.png", 3000, 600 },
	{ "asset/texture/HP.png", 3000, 600 },
	{ "asset/texture/Guard.png", 3000, 600 },
	{ "asset/texture/ATK.png", 2400, 600 },
	{ "asset/texture/DEF.png", 2400, 600 },
	{ "asset/texture/gameclear.jpg",1920,1080 },
	{ "asset/texture/gameover.jpg",1920,1080 },
	{ "asset/texture/A.png",142,142 },
	{ "asset/texture/B.png",142,142 },
	{ "asset/texture/start.png",1920,1080 },
	{ "asset/texture/score.png",3402,567 },

};


static const int TEXTURE_FILE_COUNT = sizeof(g_TextureFiles) / sizeof(g_TextureFiles[0]);

static_assert(TEXTURE_INDEX_MAX == TEXTURE_FILE_COUNT, "TEXTURE_INDEX_MAX != TEXTURE_FILE_COUNT");

static LPDIRECT3DTEXTURE9 g_pTextures[TEXTURE_FILE_COUNT] = {};

int Texture_Load(void)
{   
    LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();
	if( !pDevice ) {
		return TEXTURE_FILE_COUNT;
	}

	int failed_count = 0;

	for( int i = 0; i < TEXTURE_FILE_COUNT; i++ ) {
		
		if( FAILED(D3DXCreateTextureFromFile(pDevice, g_TextureFiles[i].filename, &g_pTextures[i])) ) {
            // DebugPrintf("テクスチャの読み込みに失敗 ... %s\n", g_TextureFiles[i].filename);
			failed_count++;
		}
	}

	return failed_count;
}

void Texture_Release(void)
{
	for( int i = 0; i < TEXTURE_FILE_COUNT; i++ ) {
		
		if( g_pTextures[i] ) {
			g_pTextures[i]->Release();
			g_pTextures[i] = NULL;
		}
	}
}

LPDIRECT3DTEXTURE9 Texture_GetTexture(TextureIndex index)
{
    if( index < 0 || index >= TEXTURE_INDEX_MAX ) {
        return NULL;
    }

	return g_pTextures[index];
}

int Texture_GetWidth(TextureIndex index)
{
    if( index < 0 || index >= TEXTURE_INDEX_MAX ) {
        return NULL;
    }

	return g_TextureFiles[index].width;
}

int Texture_GetHeight(TextureIndex index)
{
    if( index < 0 || index >= TEXTURE_INDEX_MAX ) {
        return NULL;
    }

	return g_TextureFiles[index].height;
}
