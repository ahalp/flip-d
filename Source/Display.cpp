#include "Display.h"
#include "Character.h"
#include "Utilities.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include <iostream>


	Display::Display(	int windowWidth, int windowHeight, 
						int worldWidth, int worldHeight, 
						const char* windowTitle, 
						int bgR, int bgG, int bgB )
	{

		// Initialize SDL's Video Component
		SDL_Init(SDL_INIT_VIDEO);

		// Create the window and an area in memory ( screen buffer ) pointed to be displayScreen
		// SDL_HWSURFACE: indicates screen buffer to be created in video memory]
		// SDL_DOUBLEBUF: indicates the use of 2 buffers
		//		- FrontBuffer: buffer being displayed
		//		- BackBuffer: buffer being drawn too
		_displayScreen = SDL_SetVideoMode(windowWidth, windowHeight, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);

		// Allocate a SDL Surface for the renderScreen
		_renderScreen = SDL_CreateRGBSurface(SDL_SWSURFACE || SDL_ASYNCBLIT ,worldWidth,worldHeight,32,0,0,0,0);
		_staticScreen = loadImage(STATIC_SCREEN_FILE);

		// Window title and icon (0 = default)
		SDL_WM_SetCaption(windowTitle, 0);

		setBackgroundColor(bgR, bgG, bgB);

		// Initialize camera dimension
		_camera.h = windowHeight;
		_camera.w = windowWidth;
		_camera.x = 0;
		_camera.y = 0;

		_staticRectDest.x = STATIC_SCREEN_X;
		_staticRectDest.y = STATIC_SCREEN_Y;
		_staticRectDest.w = 0;
		_staticRectDest.h = 0;

		_staticRectSource.y = 0;
		_staticRectSource.x = 0;
		_staticRectSource.w = STATIC_SCREEN_WIDTH;
		_staticRectSource.h = STATIC_SCREEN_HEIGHT;


	#ifndef CONSOLE
		std::cout << "Display Created." << std::endl;
		#endif

	}


	Display::~Display(void)
	{
		SDL_FreeSurface(_renderScreen);

		// Quit SDL and free window screen buffer
		SDL_Quit();
	}


	SDL_Surface* Display::loadImage( const char* imageFileName)
	{
		SDL_Surface* imgLoad = NULL;
		SDL_Surface* imgOptimized = NULL;


		imgLoad = IMG_Load( imageFileName );
		if ( imgLoad != NULL ){
			imgOptimized = SDL_DisplayFormatAlpha( imgLoad );
			SDL_FreeSurface( imgLoad );
		} else {
			#ifndef CONSOLE
			std::cerr << "Error: Image file < " << imageFileName << " > could no open." << std::endl;
			#endif
		}

		return imgOptimized;

	}

	void Display::closeImage(SDL_Surface* image)
	{
		 SDL_FreeSurface(image);
	}

	void Display::beginScene()
	{
		SDL_FillRect(	_renderScreen, NULL, SDL_MapRGB( _renderScreen->format,
						_bgRed, _bgGreen, _bgBlue ) );
		SDL_FillRect(	_displayScreen, NULL, SDL_MapRGB( _renderScreen->format,
						_bgRed, _bgGreen, _bgBlue ) );
	}

	void Display::endScene(Character* player){
		_camera.x = player->getXPos() - WINDOW_WIDTH / 2;
		_camera.y = player->getYPos() - WINDOW_HEIGHT / 2;
		SDL_BlitSurface( _renderScreen, &_camera, _displayScreen, NULL );
		SDL_BlitSurface( _staticScreen, &_staticRectSource, _displayScreen, &_staticRectDest);
		SDL_Flip( _displayScreen );
	}

	void Display::endScene()
	{
		_camera.x = 0;
		_camera.y = 0;
		SDL_BlitSurface( _renderScreen, &_camera, _displayScreen, NULL );
		SDL_Flip( _displayScreen );
	}

	void Display::drawImage( SDL_Surface* imageSurface,
							 int sourceX, int sourceY,
							 int destinationX, int destinationY,
							 int width, int height	)
	{

		// Part of the bitmap that we want to draw
		SDL_Rect sourceRect = {sourceX, sourceY, width, height};
		// Part of the screen we want to draw the sprite to
		SDL_Rect destinationRect = {destinationX, destinationY, width, height};

		if ( width == 0 && height == 0 ){
			SDL_BlitSurface( imageSurface, NULL, _renderScreen, &destinationRect );
		}else{
			SDL_BlitSurface( imageSurface, &sourceRect, _renderScreen, &destinationRect );
		}

	}

	void Display::drawImageStatic (SDL_Surface* imageSurface,
								 int sourceX, int sourceY,
								 int destinationX, int destinationY,
								 int width, int height ) {

		// Part of the bitmap that we want to draw
		SDL_Rect sourceRect = {sourceX, sourceY, width, height};
		// Part of the screen we want to draw the sprite to
		SDL_Rect destinationRect = {destinationX, destinationY, width, height};

		if ( width == 0 && height == 0 ){
			SDL_BlitSurface( imageSurface, NULL, _staticScreen, &destinationRect );
		}else{
			SDL_BlitSurface( imageSurface, &sourceRect, _staticScreen, &destinationRect );
		}

	}

	void Display::fillRect (SDL_Rect* dimensions, int r, int g, int b){
		SDL_FillRect(	_renderScreen, dimensions, SDL_MapRGB( _renderScreen->format,
						r, g, b ) );
	}

	void Display::fillRect (int r, int g, int b){
		SDL_FillRect(	_displayScreen, NULL, SDL_MapRGB( _renderScreen->format,
						r, g, b ) );

	}

	void Display::setBackgroundColor(int r, int g, int b)
	{
		_bgRed = r;
		_bgGreen = g;
		_bgBlue = b;
	}

	void Display::changeRenderScreenSize(int width, int height){
		SDL_FreeSurface(_renderScreen);
		_renderScreen = SDL_CreateRGBSurface(SDL_HWSURFACE, width, height,32,0,0,0,0);
	}

	int Display::getCameraX() {
		return _camera.x;
	
	}

	int Display::getCameraY() {
		return _camera.y;
	}