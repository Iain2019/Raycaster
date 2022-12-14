#include <stdafx.h>
#include "rc_Timers.h"

static std::chrono::time_point<std::chrono::system_clock> s_prevTime;
static float s_totalTime;
static float s_deltaTime;

void Timer::resetTimer()
{
	//delta time variale
	s_prevTime = std::chrono::system_clock::now();
	s_totalTime = 0.0f;
	s_deltaTime = 0.0f;
}

float Timer::tickTimer()
{
	auto currentTime = std::chrono::system_clock::now();
	std::chrono::duration<float> tstep = currentTime - s_prevTime;
	s_deltaTime = tstep.count();
	s_totalTime += s_deltaTime;
	s_prevTime = currentTime;
	return s_deltaTime;
}

float Timer::getDeltaTime()
{
	return s_deltaTime;
}

float Timer::getTotalTime()
{
	return s_totalTime;
}