#include <stdafx.h>
#pragma once

#ifndef __TIMERS_H__
#define __TIMERS_H__

class Timer
{
public:
	// utilities for timing; Get() updates timers and returns the time since last Get call
	static void resetTimer();
	static float tickTimer();
	static float getDeltaTime();
	static float getTotalTime();
};

#endif // !__TIMERS_H__

