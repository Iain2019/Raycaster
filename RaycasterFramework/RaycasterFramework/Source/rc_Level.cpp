#include <stdafx.h>
#include "rc_Level.h"
#include "Renderer.h"
#include "rc_Player.h"
#include "rc_TextureManager.h"
#include "rc_ImageLoader.h"

Level::Level() : m_width(0), m_height(0), m_map(nullptr), WallTexture(nullptr)
{
	TextureManager::CreateInstance();
}

Level::~Level()
{
	//TextureManager::DestroyInstance();
}

bool Level::LoadLevel(const char* a_filename)
{
	//Get instance of the texture manager and load the wall textures
	TextureManager::GetInstance()->LoadTexture("../Resources/Sprites/Wall1.pcx", RC_ImageType::IM_PCX);
	TextureManager::GetInstance()->LoadTexture("../Resources/Sprites/Wall2.pcx", RC_ImageType::IM_PCX);
	//we are going to load a level if the map is not null, then we need to delete it and free up memory
	//then open the file and load in level data
	if (m_map != nullptr)
	{
		delete[] m_map;
		m_map = nullptr;
	}
	//open a file stream to load map
	std::fstream file;
	file.open(a_filename, std::ios_base::in | std::ios_base::binary);
	//test to see if the file has opened correctly
	if (file.is_open())
	{
		//success file has opened, verify contents of file
		file.ignore(std::numeric_limits<std::streamsize>::max());
		std::streamsize filesize = file.gcount();
		file.clear();
		file.seekg(0, std::ios_base::beg);
		if (filesize == 0)
		{
			file.close();
			return false;
		}
		//read the width and height of the level map
		file >> m_width >> m_height;
		file.ignore(1);
		m_map = new u8[m_width * m_height];
		u32 index = 0;
		//read each line of the file to get one row worth of map data
		//use getline to read in each line of the file
		for (std::string currentMapLine; std::getline(file, currentMapLine);)
		{
			//convert string from getline into stringstream and use comma separator to break into chunks and store valaues in map
			std::istringstream ss{ currentMapLine };
			u32 val = 0;
			while (ss >> val) //whilst we still have number data to read
			{
				// when parsing store as int value temporarily then static cast to u8 otherwise we read values as char! char value of 0 is 48
				m_map[index] = (u8)(val);
				++index;
				if ((ss >> std::ws).peek() == ',') //std::ws ignores white space, if the next char is a comma ignore it too
				{
					ss.ignore();
				}
			}
		}
		//file close
		file.close();
		return true;
	}
	return false;
}

//the level knows how to draw itself
void Level::draw(const Player* a_player)
{
	//get the instance of the renderer to get the dimensions of the render window
	Renderer* renderer = Renderer::GetInstance();
	u32 windowWidth = 0, windowHeight = 0;
	renderer->GetWindowSize(windowWidth, windowHeight);

	//from the passed in pointer to the player get current position and the direction of the player
	float playerPosX = 0.0f, playerPosY = 0.0f;
	a_player->GetPosition(playerPosX, playerPosY);
	float playerDirX = 0.0f, playerDirY = 0.0f;
	a_player->GetRotation(playerDirX, playerDirY);
	float nearPlaneLength = a_player->GetNearPlaneLength();

	//camera plane is perpendicular to player direction
	//swap components and negate y
	//multiply by near plane length to get vector to right length
	float camPlaneX = -playerDirY * nearPlaneLength;
	float camPlaneY = playerDirX * nearPlaneLength;

	for (u32 c = 0; c < windowWidth; c++)
	{
		float cameraX = ((2.0f * c) / (float)windowWidth) - 1.0f;
		float rayDirX = playerDirX + (camPlaneX * cameraX);
		float rayDirY = playerDirY + (camPlaneY * cameraX);

		float deltaDistX = (rayDirX != 0.0f) ? std::abs(1.0f / rayDirX) : 0;
		float deltaDistY = (rayDirY != 0.0f) ? std::abs(1.0f / rayDirY) : 0;

		int mapX = (int)(playerPosX);
		int mapY = (int)(playerPosY);

		float sideDistX = 0.0f;
		float sideDistY = 0.0f;

		float prepWallDist = 0.0f;

		int stepX = 0;
		int stepY = 0;

		int collision = 0;
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (playerPosX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0f - playerPosX) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (playerPosY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0f - playerPosY) * deltaDistY;
		}

		int yIntersection = 0;
		while (collision == 0)
		{
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				yIntersection = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				yIntersection = 1;
			}
			collision = m_map[(mapX + (mapY * m_width))];
		}
		prepWallDist = (!yIntersection) ?
			(mapX - playerPosX + (1 - stepX) / 2.0f) / rayDirX :
			(mapY - playerPosY + (1 - stepY) / 2.0f) / rayDirY;

		s32 lineHeight = (int)(windowHeight / prepWallDist);

		int drawStart = -lineHeight / 2 + windowHeight / 2;
		if (drawStart < 0)drawStart = 0;
		int drawEnd = lineHeight / 2 + windowHeight / 2;
		if (drawEnd >= (s32)(windowHeight))drawEnd = windowHeight - 1;

		//Check what the walls value is (1 or 2) and use wall texture depending on that value
		switch (collision)
		{
			case 1: 
			{
				//assigns wall texture
				WallTexture = TextureManager::GetInstance()->GetTexture("../Resources/Sprites/Wall1.pcx");
				break;
			}
			case 2:
			{
				WallTexture = TextureManager::GetInstance()->GetTexture("../Resources/Sprites/Wall2.pcx");
				break;
			}
		}

		//Gets dimetions of the wall
		u32 textureWidth = 0; u32 textureHeight = 0;
		WallTexture->GetDimensions(textureWidth, textureHeight);

		//Find point on wall
		float WallX;
		if (yIntersection == 0) WallX = playerPosY + prepWallDist * rayDirY;
		else					WallX = playerPosX + prepWallDist * rayDirX;
		WallX -= floor((WallX));

		int textureX = int(WallX * double(textureWidth));
		if (yIntersection == 0 && rayDirX > 0) textureX = textureWidth - textureX - 1;
		if (yIntersection == 1 && rayDirY < 0) textureX = textureWidth - textureX - 1;

		double step = 1.0 * textureHeight / lineHeight;

		double texturePos = (drawStart - windowHeight / 2 + lineHeight / 2) * step;

		//assign the pixle data 
		u32* textureData = (u32*)WallTexture->GetPixelData();

		for (s32 yPos = drawStart; yPos < drawEnd; yPos++)
		{
			int textureY = (int)texturePos & (textureHeight - 1);
			texturePos += step;
			u32 pixelData = textureData[textureWidth * textureY + textureX];

			renderer->FillRenderBuffer(c, yPos, 1, 1, pixelData);
		}
	}
}

void Level::GetSize(u32& a_w, u32& a_h)
{
	a_w = m_width;
	a_h = m_height;
}

u8 Level::GetGridValue(u32 a_x, u32 a_y)
{
	u32 index = a_x + (a_y * m_width);
	if (index < (m_width * m_height))
	{
		return m_map[index];
	}
	return 255; //values of 255 will indicate a map out of bounds error
}