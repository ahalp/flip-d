#pragma once

#include "SDL.h"

class Character;

class Display
{

private:

	// Hidden screen upon which all rendering is done.
	SDL_Surface* _renderScreen;

	// Screen displayed on window
	SDL_Surface* _displayScreen;

	// Screen containing static elements
	SDL_Surface* _staticScreen;

	// Camera dimensions, equal to window dimension
	SDL_Rect _camera;
	SDL_Rect _staticRectSource;
	SDL_Rect _staticRectDest;

	// Background Color [rgb]
	int _bgRed;
	int _bgGreen;
	int _bgBlue;

public:

	//	Initializes the SDL video component / TTF
	//	Creates a window with the indicated attributes
	Display(	int windowWidth, int windowHeight, 
				int worldWidth, int worldHeight, 
				const char* windowTitle, 
				int bgR, int bgG, int bgB );

	// Quits SDL and performs all free-ing of dynamic data.
	~Display(void);


	SDL_Surface* loadImage(	const char* imageFileName);

	void closeImage(SDL_Surface* image);

	// All rendering must be done between these two method calls
	// beginScene() clears the screen
	// endScene() blits the renderScreen to the displayScreen
	void beginScene();
	void endScene();
	// This version of endScene is required to update the camera in gameplay mode.
	void endScene( Character* player);

	// Draw an image surface onto the render screen
	// A width and height of 0 indicate drawing the entire source surface
	void drawImage(	SDL_Surface* imageSurface,
					int sourceX, int sourceY,
					int destinationX, int destinationY,
					int width, int height	);

	void drawImageStatic (SDL_Surface* imageSurface,
					int sourceX, int sourceY,
					int destinationX, int destinationY,
					int width, int height	);

	// fill rect:	1. fills renderscreen with rect dimentions
	//				2. fills camera display screen
	void fillRect (SDL_Rect* dimensions, int r, int g, int b);
	void fillRect (int r, int g, int b);
	
	void setBackgroundColor(int r, int g, int b);

	void changeRenderScreenSize(int x, int y);

	int getCameraX();
	int getCameraY();

};

