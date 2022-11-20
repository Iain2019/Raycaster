#include <stdafx.h>
#pragma once
#ifndef __RC_LEVEL_H__
#define __RC_LEVEL_H__

class Player;
class TextureManager;
class Texture;

class Level
{
public:
	Level();
	~Level();

	//Get level info
	void GetSize(u32& a_w, u32& a_h);
	u8 GetGridValue(u32 a_x, u32 a_y);

	//load and draw level functions
	bool LoadLevel(const char* a_filename);

	void draw(const Player* a_player);

private:
	u32 m_width;
	u32 m_height;

	u8* m_map;

	Texture* WallTexture;
};

#endif //!__RC_LEVEL_H__