#include "Input.h"
#include "Display.h"
#include "Timing.h"
#include "Character.h"
#include "CollisionGrid.h"

extern const int LEFT;
extern const int RIGHT;

#include <iostream>

	Input::Input()
	{
		
		// Set inital state of window to be open
		_windowClosed = false;

		// Set all keysHeld to be false initially
		int i;
		for (i = 0; i < 323; i++){
			_keysHeld[i] = false;
		}

		_mouseDown = false;
		_mouseCoordinates.first = 0;
		_mouseCoordinates.second = 0;

		_rows = MAP_ROWS;
		_cols = MAP_COLS;

		#ifndef CONSOLE
		std::cout << "Input created.\n";
		#endif

	}

	Input::~Input()
	{}

	void Input::handleKeyStatesInput(GameToolsStruct &gameTools)
	{
		// Collect Event Info: SDL_PollEvent(&events)
		// Fills parameter events wil info about the events that are happening. 
		// Returns 0 if no events
		if (SDL_PollEvent(&_events)){

			 // Check if X (close) on window has been pressed
			if(_events.type == SDL_QUIT){
				_windowClosed = true;
				gameTools.State = CLOSE_WINDOW;
			}	
			// Check if a keyboard Key has been pressed. Set the appropriate
			// element in the keysHeld array to true
			if(_events.type == SDL_KEYDOWN){
				_keysHeld[_events.key.keysym.sym] = true;

			}
			// Check if a keyboard Key has been released. set the appropriate
			// element in the keysHeld array to false
			if(_events.type == SDL_KEYUP){
				_keysHeld[_events.key.keysym.sym] = false;

			}

		}
	}

	void Input::resetKeysHeld(){
		int i;
		for (i = 0; i < 323; i++){
			_keysHeld[i] = false;
		}
	}

	bool* Input::getInput()
	{
		return _keysHeld;	
	}

	bool Input::getWindowClosed()
	{
		return _windowClosed;		
	}

	SDL_Event Input::getEvents()
	{
		return _events;
	}


	void Input::handleMainMenuInput(GameToolsStruct &gameTools) {
		if (SDL_PollEvent(&_events)) {
			// Check if X (close) on window has been pressed
			if(_events.type == SDL_QUIT){
				_windowClosed = true;
				gameTools.State = CLOSE_WINDOW;
			}
			if (_events.type == SDL_KEYDOWN) {
				switch ( _events.key.keysym.sym ) {
					case SDLK_SPACE: gameTools.State = GAMEPLAY;
				}
			}

		}
	}

		void Input::handlePauseMenuInput(GameToolsStruct &gameTools) {
		if (SDL_PollEvent(&_events)) {
			// Check if X (close) on window has been pressed
			if(_events.type == SDL_QUIT){
				_windowClosed = true;
				gameTools.State = CLOSE_WINDOW;
			}
			if (_events.type == SDL_KEYDOWN) {
				switch ( _events.key.keysym.sym ) {
					case SDLK_SPACE: gameTools.State = GAMEPLAY;
				}
			}

		}
	}

			void Input::handleExitMenuInput(GameToolsStruct &gameTools) {
		if (SDL_PollEvent(&_events)) {
			// Check if X (close) on window has been pressed
			if(_events.type == SDL_QUIT){
				_windowClosed = true;
				gameTools.State = CLOSE_WINDOW;
			}
			if (_events.type == SDL_KEYDOWN) {
				switch ( _events.key.keysym.sym ) {
					case SDLK_SPACE: gameTools.State = GAMEPLAY;
				}
			}

		}
	}
	
	void  Input::handleGameplayInput(GameToolsStruct &gameTools) {
	
	static bool remove;
	


		if (SDL_PollEvent(&_events)){

			// Check if X (close) on window has been pressed
			if(_events.type == SDL_QUIT){
				_windowClosed = true;
				gameTools.State = CLOSE_WINDOW;
			}	
			
			// Add new tiles with a mouse press
			if ( _events.type == SDL_MOUSEBUTTONDOWN ) {
				if (  _events.button.button == SDL_BUTTON_LEFT ) {
					remove = false;

				} else if (  _events.button.button == SDL_BUTTON_RIGHT ) {
					remove = true;
				}
				_mouseDown = true;
				
			} 
			else if ( _events.type == SDL_MOUSEBUTTONUP ) {
				_mouseDown = false;
			}

			if (_events.type == SDL_KEYDOWN) {
				
				_keysHeld[_events.key.keysym.sym] = true;

				switch ( _events.key.keysym.sym ) {
					
					case SDLK_a: 
						gameTools.Player->moveLeft();
						break;
					case SDLK_d:
						gameTools.Player->moveRight(); 
						break;
					// Pause
					case SDLK_ESCAPE: 
						gameTools.Player->stopMovement();
						resetKeysHeld();
						gameTools.State = PAUSE;
						break;
					case SDLK_SPACE: 
						//gameTools.Player->stopMovement();
						resetKeysHeld();
						gameTools.CollisionGrid->flipCollisionGrid();
						gameTools.CollisionGrid->flipCoins();
						if ( !gameTools.CollisionGrid->getReversed() ) {
							gameTools.CollisionGrid->resetCoins();
						}
						gameTools.Player->setPosition(gameTools.Player->getXPos(), 
													_rows*PIX_PER_TILE -
													gameTools.Player->getYPos() - 
													gameTools.Player->getHeight() );
						break;
				
				}
			}
			else if(_events.type == SDL_KEYUP) {

				_keysHeld[_events.key.keysym.sym] = false;

				switch ( _events.key.keysym.sym ) {
					
					// If the player lets go, the jump has ended.
					case SDLK_w: gameTools.Player->stopJump(); break;
					case SDLK_a: gameTools.Player->undoMoveLeft(); break;
					case SDLK_d: gameTools.Player->undoMoveRight(); break;

					//case SDLK_SPACE: 
					//	gameTools.Player->stopMovement();
				}

			}

		}

		// Jump Key Held
		if ( _keysHeld[SDLK_w] ) {
			gameTools.Player->jump(gameTools);
		}


		// Mouse button held
		if ( _mouseDown ) {
			SDL_GetMouseState(&_mouseCoordinates.first,&_mouseCoordinates.second);

			if (!remove) {
			gameTools.CollisionGrid->addCollisionTile(_mouseCoordinates.first + gameTools.Display->getCameraX(),  
													  _mouseCoordinates.second + gameTools.Display->getCameraY(), 
													  gameTools);
			}
			else if (remove) {
				gameTools.CollisionGrid->removeCollisionTile(_mouseCoordinates.first + gameTools.Display->getCameraX(),  
													  _mouseCoordinates.second + gameTools.Display->getCameraY(), 
													  gameTools);
			}
		}

		// Game over
		if (gameTools.CollisionGrid->countCoins() == 0 || _keysHeld[SDLK_EQUALS]) {
			_rows++;
			_cols++;
			if (_rows < MAP_ROWS_MAX && _cols < MAP_COLS_MAX ) {
				// delete gameTools.CollisionGrid;
				gameTools.CollisionGrid = new CollisionGrid (gameTools.Display, _rows,_cols, PIX_PER_TILE);
				gameTools.CollisionGrid->populateCoins();
			} else {
				gameTools.State = EXIT;
			}
		}

	}

	