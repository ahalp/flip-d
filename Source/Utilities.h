#pragma once

class Display;
class Input;
class Character;
class Timing;
class CollisionGrid;
class GameData;

// Constants
#define FPS_CAP						60
#define WINDOW_WIDTH				600
#define WINDOW_HEIGHT				600
#define MAX_WORLD_WIDTH				2000
#define MAX_WORLD_HEIGHT			2000
#define STATIC_SCREEN_FILE			"data//static_screen.png"
#define STATIC_SCREEN_WIDTH			580
#define STATIC_SCREEN_HEIGHT		80
#define STATIC_SCREEN_X				( WINDOW_WIDTH - STATIC_SCREEN_WIDTH ) / 2
#define STATIC_SCREEN_Y				( WINDOW_HEIGHT - STATIC_SCREEN_HEIGHT - 10 )
#define WINDOW_TITLE				"f l i p ' d"

#define SURFACE_ENTRIES				3
#define AUDIO_ENTRIES				3

#define MAPFILE						"data//mapfile.txt"
#define MAP_COIN_FILE				"data//map_coin.txt"
#define MAP_ROWS					12
#define MAP_ROWS_MAX				MAP_ROWS + 12
#define MAP_COLS					15
#define MAP_COLS_MAX				MAP_COLS + 12
#define PIX_PER_TILE				50
#define MAP_REVERSED_W				22
#define MAP_REVERSED_H				21
#define MAP_REVERSED_X				25

#define MAP_REVERSED_DESTX			150
#define MAP_COINS_DESTX				500
#define MAP_COINS_H					25
#define MAP_COINS_W					19	
#define MAP_INFO_Y					30

#define MAX_COINS					9

#define PLAYER_IMAGE				"data//player.png"
#define PLAYER_WIDTH				45		// Should be atleast 4 less than TILE (unless using smaller tiles)
#define PLAYER_HEIGHT				45		// Should be atleast 4 less than TILE (unless using smaller tiles)
#define PLAYER_MAXSPEED_X			300
#define PLAYER_MAXSPEED_Y			400
#define PLAYER_GRAVITY				300
#define PLAYER_JUMP_TIME			350
#define PLAYER_JUMP_DECREASE		0.80

#define PLAYER_ANIMATION_DELAY		100
#define PLAYER_ANIMATION_FRAMES		4

#define PLAYER_THRESHOLD_JUMP		250

#define SW_HIDE 0




// GameState: All Possible GameStates
enum GameState {

	MAIN_MENU,
	GAMEPLAY,
	PAUSE,
	EXIT,
	CLOSE_WINDOW

};

enum GameLevel {
	TUTORIAL,
	LEVEL1,
	LEVEL2,
	LEVEL3,
	LEVEL4,
	LEVEL5
};

struct GameToolsStruct {
	Display* Display;
	Input* Input;
	Character* Player;
	Character* AutomatedPlayer;
	CollisionGrid* CollisionGrid;
	Timing* Timer;
	GameData* Data;

	GameState State;
	GameState LastState;
	GameLevel Level;
};



// GameState Function Prototypes
void MainMenu( GameToolsStruct &gameTools);
void Gameplay( GameToolsStruct &gameTools);
void PauseMenu ( GameToolsStruct &gameTools);
void ExitMenu( GameToolsStruct &gameTools);

void drawBackground (GameToolsStruct &gameTools);
