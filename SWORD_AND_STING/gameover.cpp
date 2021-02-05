#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include "sound.h"

void GameOver_Initialize(void)
{
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
}

void GameOver_Finalize(void)
{
	StopSound();
}

void GameOver_Update(void)
{
	if (keyboard.IsTrigger(DIK_SPACE) || joycon[0].IsTrigger(JOYCON_A)) {
		PlaySound(SOUND_LABEL_SE_OK);
		Scene_Change(SCENE_INDEX_TITLE);
	}
}

void GameOver_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_OVER, 0.0f, 0.0f);
}
