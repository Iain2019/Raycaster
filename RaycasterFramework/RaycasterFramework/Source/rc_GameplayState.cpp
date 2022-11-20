#include <stdafx.h>
#include "rc_GameplayState.h"
#include "rc_GameStateManager.h"
#include "rc_ImageLoader.h"
#include "Renderer.h"
#include "rc_Player.h"
#include "rc_Level.h"

#include <ctime>

GamePlayState::GamePlayState() : GameState(), m_pLevel(nullptr), m_pPlayer(nullptr)
{
	if (m_pLevel)
	{
		delete m_pLevel;
		delete m_pPlayer;
	}
}

GamePlayState::~GamePlayState()
{

}

void GamePlayState::Initialise(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);
	//Intitilise first level
	m_pLevel = new Level();
	if (m_pLevel->LoadLevel("../Resources/Levels/Level1.map"))
	{
		//Intitilise first player
		m_pPlayer = new Player();

		m_isActive = true;
		Process = &GameState::Update;
	}
}

void GamePlayState::Update(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);
	//process player movement
	m_pPlayer->Update(a_fDT);

	//Get the players position
	float m_posY; float m_posX;
	m_pPlayer->GetPosition(m_posX, m_posY);
	//check if in new map area
	if (m_posY < 3.0f || m_posX < 3.0f || m_posX >= 5.0f)
	{
		//delte level and player
		delete m_pLevel;
		delete m_pPlayer;

		//make new level
		m_pLevel = new Level();

		//create random integer to choose new (random) level
		std::srand((int)(std::time(nullptr)));
		int randNum = std::rand() % 7 + 1;

		//check random number for random level
		if (randNum == 7)
		{
			//load level
			if (m_pLevel->LoadLevel("../Resources/Levels/Level7.map"))
			{
				//if level loaded create new player
				m_pPlayer = new Player();

				m_isActive = true;
			}
		}
		else if (randNum == 6)
		{
			if (m_pLevel->LoadLevel("../Resources/Levels/Level6.map"))
			{
				m_pPlayer = new Player();

				m_isActive = true;
			}
		}
		else if (randNum == 5)
		{
			if (m_pLevel->LoadLevel("../Resources/Levels/Level5.map"))
			{
				m_pPlayer = new Player();

				m_isActive = true;
			}
		}
		else if (randNum == 4)
		{
			if (m_pLevel->LoadLevel("../Resources/Levels/Level4.map"))
			{
				m_pPlayer = new Player();

				m_isActive = true;
			}
		}
		else if (randNum == 3)
		{
			if (m_pLevel->LoadLevel("../Resources/Levels/Level3.map"))
			{
				m_pPlayer = new Player();

				m_isActive = true;
			}
		}
		else if (randNum == 2)
		{
			if (m_pLevel->LoadLevel("../Resources/Levels/Level2.map"))
			{
				m_pPlayer = new Player();

				m_isActive = true;
			}
		}
		else if (randNum == 1)
		{
			if (m_pLevel->LoadLevel("../Resources/Levels/Level1.map"))
			{
				m_pPlayer = new Player();

				m_isActive = true;
			}
		}
	}
}

void GamePlayState::Draw()
{
	//get renderer pointer
	Renderer* renderer = Renderer::GetInstance();

	//draw background
	unsigned int ceilingColour = 0x00505050;
	unsigned int floorColour = 0x00B2B2B2;
	renderer->FillRenderBuffer(0, 0, 640, 240, ceilingColour);
	renderer->FillRenderBuffer(0, 240, 640, 240, floorColour);

	m_pLevel->draw(m_pPlayer);
}

void GamePlayState::Leave(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);
	//if we are leaving the state inform the gamestatemanger to remove us
	GetManager()->RemoveState(this);
}
