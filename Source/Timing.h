#pragma once

#include "SDL.h"

class Timing
{
private:
	int _timeOfLastCall;
	int _FPSframeStartTime;
public:
	
	Timing();
	~Timing();

	int timeSinceCreation();
	int timeSinceLastFrame();
	
	void FPS_Start();
	int FPS_TimeElapsedForFrame();

};