#include <stdafx.h>
#pragma once
#ifndef __RC_PLAYER_H__
#define __RC_PLAYER_H__

class Level;

class Player
{
public:
	Player();
	~Player();

	void Update(float a_fDT);

	//Setters and getters
	void SetPosition(float a_x, float a_y);
	void GetPosition(float& a_x, float& a_y) const;
	void SetRotation(float a_degrees);
	void GetRotation(float& a_x, float& a_y) const;
	float GetNearPlaneLength() const { return m_nearPlaneLength; }

private:
	float m_posX;
	float m_posY;
	float m_dirX;
	float m_dirY;

	float m_nearPlaneLength;
};

#endif //!__RC_PLAYER_H__