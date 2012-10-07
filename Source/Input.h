#pragma once

#include "SDL.h"
#include "Utilities.h"
#include <utility>

class Input
{
private:

	// SDL_Event structure: contains information about events that are happening
	SDL_Event _events;
	
	// Store the state of all the keys, initially all false
	bool _keysHeld[323];

	// Mouse State
	bool _mouseDown;

	// Mouse Coordinates
	std::pair <int,int> _mouseCoordinates;


	// Set to true if window is closed. Used to break window loop.
	bool _windowClosed;

	int _rows, _cols;

public:
	
	// Sets inital states of member variables
	// (keysHeld = false, windowClosed = false)
	Input();

	~Input();

	// Fills events member with events occuring and appropriatly 
	// fills in keysHeld and windowClosed
	// Must be called before any other input methods
	void handleKeyStatesInput(GameToolsStruct &gameTools);

	void handleMainMenuInput(GameToolsStruct &gameTools);
	void handlePauseMenuInput(GameToolsStruct &gameTools);
	void handleExitMenuInput(GameToolsStruct &gameTools);
	void handleGameplayInput(GameToolsStruct &gameTools);

	
	void resetKeysHeld();

	// returns bool array of keysHeld
	bool* getInput();

	bool getWindowClosed();

	SDL_Event getEvents();
	
};



