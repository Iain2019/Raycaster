#include <stdafx.h>
#pragma once

#ifndef __GAME_STATE_MANAGER_H__
#define __GAME_STATE_MANAGER_H__

//forward delaration of state class
class GameState;

class GameStateManager
{
protected:

public:
	GameStateManager();
	~GameStateManager();

	void Update(float a_fDT);
	void Draw();

	//State removal and addition to state stack
	bool RemoveState(GameState* a_pState); //Function to remove the state passed in from the state stack
	bool PopToState(GameState* a_pState); // Function to pop all states from the stack until the state passed in is loacted
	void PushState(GameState* a_pState); //Function to push the state passed in onto the stack tack and make it the current state
	void PopState(); //Funtion to pop the current state from the state stack and make the next state the active state

	GameState* StateExitsts(const char* a_pStateName);

private:
	//a vector of states that are currently active/existing
	std::vector<GameState*> m_pStates;
	//when a state is popped from the stack it will get pushed onto the list of states
	//to free in the next update, as it may not be freed immediately as it will possibly
	//getting updates still as it is freed
	std::vector<GameState*> m_pStatesToFree;
};

#endif // !__GAME_STATE_MANAGER_H__

