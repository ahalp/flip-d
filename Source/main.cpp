// External Libraries linked at run-time
#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_TTF.lib")
#pragma comment(lib, "sdlgfx.lib")
#pragma comment(lib, "SDL_image.lib")

#include <iostream>

#include "Display.h"
#include "Timing.h"
#include "Character.h"
#include "Input.h"
#include "CollisionGrid.h"
#include "Utilities.h"
#include "GameData.h"


int main (int argc, char *argv[]){

	#ifndef CONSOLE
	std::cout << "Initiating Game.\n";
	std::cout << "_________________\n\n";
	#endif

	


	GameToolsStruct gameTools;

	gameTools.Display = new Display(WINDOW_WIDTH,WINDOW_HEIGHT,MAX_WORLD_WIDTH,MAX_WORLD_HEIGHT,WINDOW_TITLE,58,59,49);
	gameTools.Timer = new Timing();
	gameTools.Input = new Input();
	gameTools.Player = new Character(gameTools.Display, gameTools.Timer, 0, 0, PLAYER_WIDTH,PLAYER_HEIGHT, PLAYER_IMAGE, 100, 100, 
									PLAYER_MAXSPEED_X, PLAYER_MAXSPEED_Y, PLAYER_GRAVITY, PLAYER_JUMP_TIME, PLAYER_JUMP_DECREASE);	
	gameTools.CollisionGrid = new CollisionGrid (gameTools.Display, MAP_ROWS, MAP_COLS, PIX_PER_TILE);
	gameTools.CollisionGrid->populateCoins();
	gameTools.Data = new GameData();

	gameTools.State = MAIN_MENU;

 
	
	bool gameActive = true;

	while ( gameActive ) {

		switch ( gameTools.State ) {

		case MAIN_MENU:
			MainMenu(gameTools);
			break;
		case GAMEPLAY:
			Gameplay(gameTools);
			break;
		case PAUSE:
			PauseMenu(gameTools);
			break;
		case EXIT:
			ExitMenu(gameTools);
			break;
		case CLOSE_WINDOW:
			gameActive = false;
			break;

		}

	}

	return 0;


}
