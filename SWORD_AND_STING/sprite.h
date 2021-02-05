#pragma once

#include <d3d9.h>
#include "texture.h"

void Sprite_SetColor(D3DCOLOR color);

void Sprite_Draw(TextureIndex texture_index, float dx, float dy);

void Sprite_Draw(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th);

void Sprite_Draw(TextureIndex texture_index, float dx, float dy, int tx, int ty, int tw, int th, float cx, float cy, float sx, float sy, float rotation);
