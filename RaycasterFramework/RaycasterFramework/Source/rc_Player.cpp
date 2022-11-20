#include <stdafx.h>
#include "rc_InputManager.h"
#include "rc_Player.h"
#include "rc_Level.h"

Player::Player() : m_dirX(0.0f), m_dirY(-1.0f), m_posX(4.0f), m_posY(6.0f)
{
	float nearPlaneDistance = 1.0f;
	m_nearPlaneLength = tanf(HALF_FOV * (float)(PI / 180.0f)) * nearPlaneDistance;
}

Player::~Player()
{

}

void Player::SetPosition(float a_x, float a_y)
{
	m_posX = a_x; m_posY = a_y;
}
void Player::GetPosition(float& a_x, float& a_y) const
{
	a_x = m_posX; a_y = m_posY;
}

void Player::SetRotation(float a_degrees)
{
	float radians = a_degrees * (float)(PI / 180.0f);
	m_dirX = 1.0f * cosf(radians);
	m_dirY = 1.0f * sin(radians);
}
void Player::GetRotation(float& a_x, float& a_y) const
{
	a_x = m_dirX; a_y = m_dirY;
}

void Player::Update(float a_fDT)
{
	//process player movement
	if (InputManager::GetInstance()->GetKeyDown('W'))
	{
		m_posX += 2.0f * m_dirX * a_fDT;
		m_posY += 2.0f * m_dirY * a_fDT;
	}
	if (InputManager::GetInstance()->GetKeyDown('S'))
	{
		m_posX -= 2.0f * m_dirX * a_fDT;
		m_posY -= 2.0f * m_dirY * a_fDT;
	}

	//turn left
	if (InputManager::GetInstance()->GetKeyDown('A'))
	{
		float playerRotSpeed = -0.4f * a_fDT;
		float oldDirX = m_dirX;
		m_dirX = oldDirX * cosf(playerRotSpeed) - m_dirY * sin(playerRotSpeed);
		m_dirY = oldDirX * sin(playerRotSpeed) + m_dirY * cos(playerRotSpeed);
	}
	//trun right
	if (InputManager::GetInstance()->GetKeyDown('D'))
	{
		float playerRotSpeed = 0.4f * a_fDT;
		float oldDirX = m_dirX;
		m_dirX = oldDirX * cosf(playerRotSpeed) - m_dirY * sin(playerRotSpeed);
		m_dirY = oldDirX * sin(playerRotSpeed) + m_dirY * cos(playerRotSpeed);
	}
}