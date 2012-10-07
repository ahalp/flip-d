#pragma once

#include "SDL.h"
#include "Utilities.h"

class Display;

class CollisionGrid
{

private:

	bool ** _grid;
	bool ** _coins;
	bool ** _coinsCopy;

	int _rows, _cols;
	int _pixelsPerTile;

	int _numAddedTiles;

	SDL_Surface * _tileImage;
	SDL_Surface * _tileImage2;
	SDL_Surface * _coinImage;
	SDL_Surface * _coinImage2;
	SDL_Surface * _infoImage;
	SDL_Rect * _gridBG;
		
	bool _reversed;
	int _coinsLeft;

public:

	// Create the 2D bool array and initialize it
	CollisionGrid(Display* display, int rows, int cols, int _pixelsPerTile);
	// Free the bool array
	~CollisionGrid(void);


	void addCollisionTile(int row, int col);
	
	void addCollisionTile ( int xPos, int yPos, GameToolsStruct &gameTools);
	void removeCollisionTile ( int xPos, int yPos, GameToolsStruct &gameTools);

	bool getCollisionTile(int xPos, int yPos, int width, int height);

	bool onGround(int xPos, int yPos,  int width, int height);
	bool rightCollision(int xPos, int yPos,  int width, int height);
	bool leftCollision(int xPos, int yPos,  int width, int height);

	void drawGrid( GameToolsStruct &gameTools );

	void flipCollisionGrid();
	void flipCoins();

	void collectCoins( int xPos, int yPos,  int width, int height );
	int countCoins();

	void populateCoins();
	void resetCoins();
	void populateCoinsFile();

	bool getReversed();

};

//void parseCollisionMapFile(CollisionGrid* &grid, const char* mapFileName);