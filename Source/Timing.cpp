#include "Timing.h"

	

	Timing::Timing(): _timeOfLastCall(0){
		SDL_Init(SDL_INIT_TIMER);
	}

	Timing::~Timing(){

	}

	int Timing::timeSinceCreation()
	{
		// return in seconds
		return SDL_GetTicks();
	}
	int Timing::timeSinceLastFrame()
	{
		int thisTime = timeSinceCreation();
		int deltaTime = thisTime - _timeOfLastCall;
		_timeOfLastCall = thisTime;

		return deltaTime;
	}

	void Timing::FPS_Start(){
		_FPSframeStartTime = SDL_GetTicks();
	}

	int Timing::FPS_TimeElapsedForFrame(){
		return ( SDL_GetTicks() - _FPSframeStartTime);
	}