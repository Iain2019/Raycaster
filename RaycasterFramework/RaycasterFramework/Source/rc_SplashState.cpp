#include <stdafx.h>
#include "rc_SplashState.h"
#include "rc_GameStateManager.h"
#include "rc_ImageLoader.h"
#include "Renderer.h"
#include "rc_GameplayState.h"

SplashState::SplashState() : m_backgroundImageData(nullptr), m_bgWidth(0), m_bgHeight(0)
{

}

SplashState::~SplashState()
{
	if (m_backgroundImageData)
	{
		delete[] m_backgroundImageData;
		m_backgroundImageData = nullptr;
	}
}

void SplashState::Initialise(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);
	void* paletteData;
	u8 bpp;
	//load the background image for this state
	m_backgroundImageData = ImageLoader::LoadFromFile("../Resources/Sprites/TITLEH.PCX",
		RC_ImageType::IM_PCX, m_bgWidth, m_bgHeight, bpp, paletteData);
	m_isActive = true;
	m_fTimeInState = 0.0f;
	Process = &GameState::Update;
}

void SplashState::Update(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);
	m_fTimeInState += a_fDT;
	//wait 5 seconds then leave
	if (m_fTimeInState > 5.0f)
	{
		m_fTimeInState = 0.0f;
		Process = &GameState::Leave;
	}
}

void SplashState::Draw()
{
	//draw background image
	Renderer* renderer = Renderer::GetInstance();
	if (m_backgroundImageData != nullptr)
	{
		renderer->FillRenderBuffer(0, 0, m_bgWidth, m_bgHeight, m_backgroundImageData);
	}
}

void SplashState::Leave(float a_fDT)
{
	UNREFERENCED_PARAMETER(a_fDT);
	//set up the gameplay state
	GamePlayState* gpState = new GamePlayState();
	gpState->SetStateName("GamePlayState");
	gpState->SetManager(GetManager());
	//Switch to gameplay state
	GetManager()->PushState(gpState);
	//if we are leaving the state inform the gameStateManager to remove us
	GetManager()->RemoveState(this);
}