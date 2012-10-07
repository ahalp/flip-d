#include "Utilities.h"

#include "Display.h"
#include "Timing.h"
#include "Character.h"
#include "Input.h"
#include "CollisionGrid.h"
#include "GameData.h"

#include <iostream>
#include <string>

// Data File Names
const std::string BG_MENUMAIN =	"data//bg_menuMain.png";
const std::string BG_MENUPAUSE = "data//bg_menuPause.png";
const std::string BG_MENUEXIT =	"data//bg_menuExit.png";
const std::string BG_GAMEPLAY_LEVEL1 = "data//bg_gamePlay_level1.png";


void MainMenu(GameToolsStruct &gameTools)
{

	// Shorter names for pointers to gameTools objects
	Timing* timer = gameTools.Timer;
	Input* input = gameTools.Input;
	Display* display = gameTools.Display;

	// Start the FPS cap counter.
	timer->FPS_Start();

	// -- 1. Input --
	input->handleMainMenuInput(gameTools);

	// -- 2. Logic --

	// -- 3. Rendering --
	display->beginScene();
	drawBackground (gameTools);
	display->endScene();

	// Delay any extra time in a frame to maintain FPS cap
	if ( timer->FPS_TimeElapsedForFrame() < 1000 / FPS_CAP ) {
		SDL_Delay( ( 1000/ FPS_CAP ) - timer->FPS_TimeElapsedForFrame());
	}

}
void Gameplay(GameToolsStruct &gameTools)
{

	// Shorter names for pointers to gameTools objects
	Timing* timer = gameTools.Timer;
	Input* input = gameTools.Input;
	Display* display = gameTools.Display;
	Character* player = gameTools.Player;
	CollisionGrid* grid = gameTools.CollisionGrid;

	// Start the FPS cap counter.
	timer->FPS_Start();

	// Time between frames in seconds
	float deltaTime = timer->timeSinceLastFrame() / 1000.0f;


	// -- 1. Input --
	input->handleGameplayInput(gameTools);

	// -- 2. Logic --
	player->update(deltaTime, gameTools);
		
	// -- 3. Rendering --
	display->beginScene();
	drawBackground (gameTools);
	grid->drawGrid(gameTools);
	player->animate();
	display->endScene( player );

	// Delay any extra time in a frame to maintain FPS cap
	if ( timer->FPS_TimeElapsedForFrame() < 1000 / FPS_CAP ) {
		SDL_Delay( ( 1000/ FPS_CAP ) - timer->FPS_TimeElapsedForFrame());
	}

}

void PauseMenu(GameToolsStruct &gameTools)
{

	// Shorter names for pointers to gameTools objects
	Timing* timer = gameTools.Timer;
	Input* input = gameTools.Input;
	Display* display = gameTools.Display;

	// Start the FPS cap counter.
	timer->FPS_Start();

	// -- 1. Input --
	input->handleMainMenuInput(gameTools);

	// -- 2. Logic --

	// -- 3. Rendering --
	display->beginScene();
	drawBackground (gameTools);
	display->endScene();

	// Delay any extra time in a frame to maintain FPS cap
	if ( timer->FPS_TimeElapsedForFrame() < 1000 / FPS_CAP ) {
		SDL_Delay( ( 1000/ FPS_CAP ) - timer->FPS_TimeElapsedForFrame());
	}
}

void ExitMenu(GameToolsStruct &gameTools)
{

	// Shorter names for pointers to gameTools objects
	Timing* timer = gameTools.Timer;
	Input* input = gameTools.Input;
	Display* display = gameTools.Display;

	// Start the FPS cap counter.
	timer->FPS_Start();

	// -- 1. Input --
	input->handleMainMenuInput(gameTools);

	// -- 2. Logic --

	// -- 3. Rendering --
	display->beginScene();
	drawBackground (gameTools);
	display->endScene();

	// Delay any extra time in a frame to maintain FPS cap
	if ( timer->FPS_TimeElapsedForFrame() < 1000 / FPS_CAP ) {
		SDL_Delay( ( 1000/ FPS_CAP ) - timer->FPS_TimeElapsedForFrame());
	}
}



void drawBackground (GameToolsStruct &gameTools) {

	// Each State as a different background
	switch ( gameTools.State ) {
		case MAIN_MENU:
			gameTools.Display->drawImage(gameTools.Data->getSurface(BG_MENUMAIN, gameTools), 0, 0, 0, 0, 0, 0);
			break;
		case PAUSE:
			gameTools.Display->drawImage(gameTools.Data->getSurface(BG_MENUPAUSE, gameTools), 0, 0, 0, 0, 0, 0);
			break;
		case EXIT:
			gameTools.Display->drawImage(gameTools.Data->getSurface(BG_MENUEXIT, gameTools), 0, 0, 0, 0, 0, 0);
			break;

	}

}
