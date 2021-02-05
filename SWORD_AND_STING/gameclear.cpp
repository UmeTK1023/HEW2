#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "texture.h"
#include "fade.h"
#include "sound.h"

void GameClear_Initialize(void)
{
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
}

void GameClear_Finalize(void)
{
	StopSound();
}

void GameClear_Update(void)
{
	if (keyboard.IsTrigger(DIK_SPACE) || joycon[0].IsTrigger(JOYCON_A)) {
		PlaySound(SOUND_LABEL_SE_OK);
		Scene_Change(SCENE_INDEX_TITLE);
	}
}

void GameClear_Draw(void)
{
	Sprite_Draw(TEXTURE_INDEX_CLEAR, 0.0f, 0.0f);
}
