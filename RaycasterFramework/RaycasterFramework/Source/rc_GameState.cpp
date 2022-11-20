#include <stdafx.h>
#include "rc_GameState.h"
#include "rc_GameStateManager.h"

GameState::GameState() : m_Name(nullptr), m_fTimeInState(0.0f), m_isActive(false), m_pManager(nullptr)
{
	//set the current process function pointer to point to initialise
	Process = &GameState::Initialise;
}

GameState::~GameState()
{
	if (m_Name != nullptr)
	{
		delete[] m_Name;
		m_Name = nullptr;
	}
}

//set the name of the state
void GameState::SetStateName(const char* a_pStateName)
{
	if (m_Name == nullptr)
	{
		u32 l = (u32)strlen(a_pStateName);
		//int l = (int)strlen(a_pStateName);
		//allocated l+1 as null terminator is required
		m_Name = new char[l + 1];
		strcpy(m_Name, a_pStateName);
	}
}

//get the name of the state
const char* GameState::GetStateName()
{
	return m_Name;
}

//Set manager
void GameState::SetManager(GameStateManager* a_pManager)
{
	m_pManager = a_pManager;
}

//empty draw function
void GameState::Draw()
{

}