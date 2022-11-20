#include <stdafx.h>
#pragma once
#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

//forward delaration for game state manager class so that states can inform the manager to move them
class GameStateManager;
//this is a virtual state class that base classes will inherit from
//this is an abstract class, not a concrete class
class GameState
{
public:
	GameState();
	virtual ~GameState();
	//all functions for states below
	virtual void Initialise(float a_fDT) = 0;
	virtual void Update(float a_fDT) = 0;
	virtual void Leave(float a_fDT) = 0;

	virtual void Draw();

	void SetStateName(const char* a_name);
	const char* GetStateName();

	void SetManager(GameStateManager* a_pManager);
	GameStateManager* GetManager() { return m_pManager; }
	bool isActive() { return m_isActive; }
	//this is a function pointer to a member function that represents the immediate process of this state
	void (GameState::* Process)(float);

private:
	char* m_Name;
	GameStateManager* m_pManager;

protected:
	//protected variables are able to be modified by child classes that publically inherit from gamestate
	float m_fTimeInState;
	bool m_isActive;
};

#endif // !__GAME_STATE_H__
