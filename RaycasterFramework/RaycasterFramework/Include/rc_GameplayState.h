#include <stdafx.h>
#include "rc_GameState.h"
#pragma once

#ifndef __GAMEPLAY_STATE_H__
#define __GAMEPLAY_STATE_H__

class Player;
class Level;

class GamePlayState : public GameState
{
public:
	GamePlayState();
	virtual ~GamePlayState();

	//define the virtual parent methods and impliment them in source file
	virtual void Initialise(float a_fDT);
	virtual void Update(float a_fDT);
	virtual void Leave(float a_fDT);

	virtual void Draw();

private:
	Level* m_pLevel;
	Player* m_pPlayer;
};

#endif // !__GAMEPLAY_STATE_H__