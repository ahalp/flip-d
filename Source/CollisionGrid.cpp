#include "CollisionGrid.h"

#include <iostream>
#include <fstream>
#include <string>
#include "math.h"
#include "time.h"

#include "Display.h"
#include "Character.h"


CollisionGrid::CollisionGrid(Display* display, int rows, int cols, int _pixelsPerTile):
							_rows(rows), _cols(cols), _pixelsPerTile(_pixelsPerTile)
{
	
	// Create a 2D array bool elements
	_grid = (bool**) operator new ( sizeof(bool*) * rows );

	int i,j;
	for ( i = 0 ; i < rows; i++ ) {
		_grid[i] = (bool*) operator new ( sizeof(bool) * cols );
	}

	_coins = NULL;

	_numAddedTiles = 0;
	_coinsLeft  = MAX_COINS;
	_reversed = false;

	// Initiallize all to false except boundary
	for ( i = 0 ; i < rows ; i++ ) {
		for ( j = 0 ; j < cols ; j++ ) {
			if (i == (_rows-1) || i == 0 || j == (_cols-1) || j == 0) {
				_grid[i][j] = true;
			} else {
				_grid[i][j] = false;
			}
		}
	}

	// Create coin bool array and initialize
	_coins = (bool**) operator new (sizeof(bool*) * _rows);
	for (i = 0; i < _rows; i++){
		_coins[i] = (bool*) operator new (sizeof(bool) * _cols);
	}

	_coinsCopy = (bool**) operator new (sizeof(bool*) * _rows);
	for (i = 0; i < _rows; i++){
		_coinsCopy[i] = (bool*) operator new (sizeof(bool) * _cols);
	}

	for ( i = 0 ; i < rows ; i++ ) {
		for ( j = 0 ; j < cols ; j++ ) {
			_coins[i][j] = false;
			_coinsCopy[i][j] = false;
		}
	}


	_gridBG = new SDL_Rect();

	// Tile images
	_tileImage = display->loadImage("data//tile.png");
	_tileImage2 = display->loadImage("data//tile2.png");
	_coinImage = display->loadImage("data//coin.png");
	_coinImage2 = display->loadImage("data//coin_rev.png");
	_infoImage = display->loadImage("data//map_info.png");

	(*_gridBG).x = 0;
	(*_gridBG).y = 0;
	(*_gridBG).w = _cols * _pixelsPerTile;
	(*_gridBG).h = _rows * _pixelsPerTile;

	#ifndef CONSOLE
	std::cout << "New Level created.\n";
	#endif

}

// Free the bool array
CollisionGrid::~CollisionGrid(void)
{
	int i,j;

	for ( i = 0 ; i < _rows ; i++ ) {
		delete _grid[i];
	}

	delete _grid;
	_grid = NULL;

	for ( i = 0 ; i < _rows ; i++ ) {
		delete _coins[i];
	}

	delete _coins;
	_coins = NULL;

	for ( i = 0 ; i < _rows ; i++ ) {
		delete _coinsCopy[i];
	}

	delete _coinsCopy;
	_coinsCopy = NULL;

	SDL_FreeSurface(_tileImage);
	SDL_FreeSurface(_tileImage2);
	SDL_FreeSurface(_coinImage);
	SDL_FreeSurface(_coinImage2);	
	SDL_FreeSurface(_infoImage);
	delete _gridBG;

}

void CollisionGrid::addCollisionTile( int row, int col ) {
	
	if ( row < _rows && col < _cols && row >= 0 && col >= 0 ) {
		_grid[row][col] = true;
	} 
	else {
		//#ifndef CONSOLE
		//std::cerr << "Error: Attempted to create collision tile out of grid boundaries." << std::endl;
		//#endif
	}
}


void CollisionGrid::addCollisionTile ( int xPos, int yPos, GameToolsStruct &gameTools) {
	
	if ( !_reversed ) {

		int col  = xPos / _pixelsPerTile;
		int row  = yPos / _pixelsPerTile;

		if ( row < _rows-1 && col < _cols-1 && row > 0 && col > 0 ) {
		
			// if there is no tile at selected place	
			if ( !_grid[row][col] ) {
			
				_grid[row][col] = true;
				_numAddedTiles++;

				// Make sure the inserted tile does not overlap player.
				if ( getCollisionTile(gameTools.Player->getXPos(), gameTools.Player->getYPos(), 
									  gameTools.Player->getWidth(), gameTools.Player->getHeight() ) ) {
					_grid[row][col] = false;

				}
			}
		} 
		else {
			//#ifndef CONSOLE
			//std::cerr << "Error: Attempted to create collision tile out of grid boundaries." << std::endl;
			//#endif
		}
	}

}

void CollisionGrid::removeCollisionTile ( int xPos, int yPos, GameToolsStruct &gameTools) {
	
	if ( !_reversed ) {
		int col  = xPos / _pixelsPerTile;
		int row  = yPos / _pixelsPerTile;

		if ( row < _rows-1 && col < _cols-1 && row > 0 && col > 0 ) {
		
			// if there is a tile at selected place	
			if ( _grid[row][col] ) {
				_grid[row][col] = false;
				_numAddedTiles--;
			}
		} 
		else {
			//#ifndef CONSOLE
			//std::cerr << "Error: Attempted to delete collision tile out of grid boundaries." << std::endl;
			//#endif
		}
	}
}



bool CollisionGrid::getCollisionTile( int xPos, int yPos, int width, int height ) {


	int row_tl = yPos / _pixelsPerTile;
	int col_tl = xPos / _pixelsPerTile;
	int row_tr = yPos / _pixelsPerTile;
	int col_tr = (xPos +  width) / _pixelsPerTile;
	int row_bl = (yPos + height) / _pixelsPerTile;
	int col_bl = xPos / _pixelsPerTile;
	int row_br = (yPos + height) / _pixelsPerTile;
	int col_br = (xPos + width) / _pixelsPerTile;



	if ( row_bl < _rows && col_br < _cols && row_tl >= 0 && col_tl >= 0 ) {
		return ( _grid[row_tl][col_tl] || _grid[row_tr][col_tr] || _grid[row_bl][col_bl] || _grid[row_br][col_br] );
	}
	else {
		//#ifndef CONSOLE
		//std::cerr << "Warning: Attempted to get collision tile out of grid boundaries." << std::endl;
		//#endif
		return true;
	}

}


void CollisionGrid::collectCoins( int xPos, int yPos, int width, int height ) {


	int row_tl = yPos / _pixelsPerTile;
	int col_tl = xPos / _pixelsPerTile;
	int row_tr = yPos / _pixelsPerTile;
	int col_tr = (xPos +  width) / _pixelsPerTile;
	int row_bl = (yPos + height) / _pixelsPerTile;
	int col_bl = xPos / _pixelsPerTile;
	int row_br = (yPos + height) / _pixelsPerTile;
	int col_br = (xPos + width) / _pixelsPerTile;


	if ( _reversed ) {
		if ( row_bl < _rows && col_br < _cols && row_tl >= 0 && col_tl >= 0 ) {
			if ( _coins[row_tl][col_tl] || _coins[row_tr][col_tr] || _coins[row_bl][col_bl] || _coins[row_br][col_br] ){
				_coins[row_tl][col_tl] = false;
				_coins[row_tr][col_tr] = false;
				_coins[row_bl][col_bl] = false;
				_coins[row_br][col_br] = false;
			}
		}
		else {
			//#ifndef CONSOLE
			//std::cerr << "Warning: Attempted to get coins out of grid boundaries." << std::endl;
			//#endif
		}
	}

}

bool CollisionGrid::onGround( int xPos, int yPos, int width, int height ) {

	int threshold = 5;

	int row = yPos / _pixelsPerTile;
	int col = xPos / _pixelsPerTile;
	int row_bl = (yPos + height + threshold) / _pixelsPerTile;
	int col_bl = xPos / _pixelsPerTile;
	int row_br = (yPos + height + threshold) / _pixelsPerTile;
	int col_br = (xPos + width) / _pixelsPerTile;
	
	if ( row < _rows && col < _cols && row >= 0 && col >= 0 ) {
		return _grid[row_bl][col_bl] || _grid[row_br][col_br];
	}
	else {
		//#ifndef CONSOLE
		//std::cerr << "Warning: Attempted to get collision grond out of grid boundaries." << std::endl;
		//#endif
		return true;
	}

}

bool CollisionGrid::rightCollision( int xPos, int yPos, int width, int height ) {

	int threshold = 5;

	int row = yPos / _pixelsPerTile;
	int col = xPos / _pixelsPerTile;
	int row_tr = yPos / _pixelsPerTile;
	int col_tr = (xPos +  width + threshold) / _pixelsPerTile;
	int row_br = (yPos + height) / _pixelsPerTile;
	int col_br = (xPos + width  + threshold) / _pixelsPerTile;
	
	if ( row < _rows && col < _cols && row >= 0 && col >= 0 ) {
		return  _grid[row_br][col_br] || _grid[row_tr][col_tr];
	}
	else {
		//#ifndef CONSOLE
		//std::cerr << "Warning: Attempted to get collision grond out of grid boundaries." << std::endl;
		//#endif
		return true;
	}

}

bool CollisionGrid::leftCollision( int xPos, int yPos, int width, int height ) {

	int threshold = 5;

	int row = yPos / _pixelsPerTile;
	int col = (xPos - threshold) / _pixelsPerTile;
	int row_bl = (yPos + height) / _pixelsPerTile;
	int col_bl = (xPos + height - threshold) / _pixelsPerTile;
	
	if ( row < _rows && col < _cols && row >= 0 && col >= 0 ) {
		return  _grid[row_bl][col_bl] || _grid[row][col];
	}
	else {
		//#ifndef CONSOLE
		//std::cerr << "Warning: Attempted to get collision grond out of grid boundaries." << std::endl;
		//#endif
		return true;
	}

}





void CollisionGrid::drawGrid( GameToolsStruct &gameTools ){
	
	

	gameTools.Display->fillRect(_gridBG, 210, 201, 192);

	int i,j;

	for (i = 0; i < _rows; i++) {
		for (j = 0; j <_cols; j++) {
			if (_grid[i][j]) {

				// BOUNDARY TILES

				// top left
				if ( i == 0 && j == 0) {
					gameTools.Display->drawImage(_tileImage2, 0, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				} 
				// bottom left
				else if ( i == _rows-1 && j == 0 ) {
					gameTools.Display->drawImage(_tileImage2, _pixelsPerTile*1, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// top right
				else if ( i == 0 && j == _cols-1) {
					gameTools.Display->drawImage(_tileImage2, _pixelsPerTile*2, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// bottom right
				else if ( i == _rows-1 && j == _cols-1) {
					gameTools.Display->drawImage(_tileImage2, _pixelsPerTile*3, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// top
				else if ( i == 0 ) {
					if ( !_grid[i+1][j] ) {
						gameTools.Display->drawImage(_tileImage2, _pixelsPerTile*4,  _pixelsPerTile, 
						j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
					} else {
						gameTools.Display->drawImage(_tileImage2, _pixelsPerTile*4, 0, 
						j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
					}
				}
				// bottom
				else if ( i == _rows-1 ) {
					if ( !_grid[i-1][j] ) {
						gameTools.Display->drawImage(_tileImage2, _pixelsPerTile*5,  _pixelsPerTile, 
						j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
					} else {
						gameTools.Display->drawImage(_tileImage2, _pixelsPerTile*5, 0, 
						j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
					}
				}
				// left
				else if ( j == 0 ) {
					if ( !_grid[i][j+1] ) {
						gameTools.Display->drawImage(_tileImage2, _pixelsPerTile*6,  _pixelsPerTile, 
						j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
					} else {
						gameTools.Display->drawImage(_tileImage2, _pixelsPerTile*6, 0, 
						j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
					}
				}
				// right
				else if ( j == _cols-1 ) {
					if ( !_grid[i][j-1] ) {
						gameTools.Display->drawImage(_tileImage2, _pixelsPerTile*7,  _pixelsPerTile, 
						j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
					} else {
						gameTools.Display->drawImage(_tileImage2, _pixelsPerTile*7, 0, 
						j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
					}
				}

				// GAME TILES

				// left right top bottom
				else if (_grid[i][j] && _grid[i-1][j] && _grid[i+1][j] && _grid[i][j-1] && _grid[i][j+1]){
					gameTools.Display->drawImage(_tileImage, _pixelsPerTile*15, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// [none]
				else if (_grid[i][j] && !_grid[i-1][j] && !_grid[i+1][j] && !_grid[i][j-1] && !_grid[i][j+1]){
					gameTools.Display->drawImage(_tileImage, 0, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// top
				else if (_grid[i][j] && _grid[i-1][j] && !_grid[i+1][j] && !_grid[i][j-1] && !_grid[i][j+1]){
					gameTools.Display->drawImage(_tileImage, _pixelsPerTile*1, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// right
				else if (_grid[i][j] && !_grid[i-1][j] && !_grid[i+1][j] && !_grid[i][j-1] && _grid[i][j+1]){
					gameTools.Display->drawImage(_tileImage, _pixelsPerTile*2, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// left
				else if (_grid[i][j] && !_grid[i-1][j] && !_grid[i+1][j] && _grid[i][j-1] && !_grid[i][j+1]){
					gameTools.Display->drawImage(_tileImage, _pixelsPerTile*4, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// bottom
				else if (_grid[i][j] && !_grid[i-1][j] && _grid[i+1][j] && !_grid[i][j-1] && !_grid[i][j+1]){
					gameTools.Display->drawImage(_tileImage, _pixelsPerTile*3, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// top left
				else if (_grid[i][j] && _grid[i-1][j] && !_grid[i+1][j] && _grid[i][j-1] && !_grid[i][j+1]){
					gameTools.Display->drawImage(_tileImage, _pixelsPerTile*6, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// top right
				else if (_grid[i][j] && _grid[i-1][j] && !_grid[i+1][j] && !_grid[i][j-1] && _grid[i][j+1]){
					gameTools.Display->drawImage(_tileImage, _pixelsPerTile*5, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// bottom right
				else if (_grid[i][j] && !_grid[i-1][j] && _grid[i+1][j] && !_grid[i][j-1] && _grid[i][j+1]){
					gameTools.Display->drawImage(_tileImage, _pixelsPerTile*7, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// bottom left
				else if (_grid[i][j] && !_grid[i-1][j] && _grid[i+1][j] && _grid[i][j-1] && !_grid[i][j+1]){
					gameTools.Display->drawImage(_tileImage, _pixelsPerTile*8, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// left right
				else if (_grid[i][j] && _grid[i-1][j] && _grid[i+1][j] && !_grid[i][j-1] && !_grid[i][j+1]){
					gameTools.Display->drawImage(_tileImage, _pixelsPerTile*10, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// top bottom
				else if (_grid[i][j] && !_grid[i-1][j] && !_grid[i+1][j] && _grid[i][j-1] && _grid[i][j+1]){
					gameTools.Display->drawImage(_tileImage, _pixelsPerTile*9, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// not left
				else if (_grid[i][j] && _grid[i-1][j] && _grid[i+1][j] && !_grid[i][j-1] && _grid[i][j+1]){
					gameTools.Display->drawImage(_tileImage, _pixelsPerTile*12, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// not right
				else if (_grid[i][j] && _grid[i-1][j] && _grid[i+1][j] && _grid[i][j-1] && !_grid[i][j+1]){
					gameTools.Display->drawImage(_tileImage, _pixelsPerTile*14, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// not top
				else if (_grid[i][j] && !_grid[i-1][j] && _grid[i+1][j] && _grid[i][j-1] && _grid[i][j+1]){
					gameTools.Display->drawImage(_tileImage, _pixelsPerTile*11, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
				// not bottom
				else if (_grid[i][j] && _grid[i-1][j] && !_grid[i+1][j] && _grid[i][j-1] && _grid[i][j+1]){
					gameTools.Display->drawImage(_tileImage, _pixelsPerTile*13, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}

			}
		}
	}

	// draw coins
	for (i = 0; i < _rows; i++) {
		for (j = 0; j <_cols; j++) {
			if (_coins[i][j]) {
				if (_reversed) {
					gameTools.Display->drawImage(_coinImage, 0, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				} else {
					gameTools.Display->drawImage(_coinImage2, 0, 0, 
					j * _pixelsPerTile , i * _pixelsPerTile, _pixelsPerTile, _pixelsPerTile );
				}
			}
		}
	}

	// draw reversed
	if ( !_reversed ){
		gameTools.Display->drawImageStatic(_infoImage, 0, 0, 
			MAP_REVERSED_DESTX	, MAP_INFO_Y, MAP_REVERSED_W, MAP_REVERSED_H);
	} else {
		gameTools.Display->drawImageStatic(_infoImage,  MAP_REVERSED_X, 0, 
			MAP_REVERSED_DESTX, MAP_INFO_Y, MAP_REVERSED_W,  MAP_REVERSED_H); 
	}

	countCoins();

	if (_coinsLeft>0) {
		gameTools.Display->drawImageStatic(_infoImage,  (_coinsLeft - 1) * MAP_COINS_W, MAP_COINS_H, 
				MAP_COINS_DESTX, MAP_INFO_Y, MAP_COINS_W,  MAP_COINS_H); 
	}

}

int CollisionGrid::countCoins () {
	int i,j;
	int coins=0;
	for (i=0; i < _rows; i++) {
		for (j=0; j <_cols; j++) {
			if (_coins[i][j]){
				coins++;
			}
		}
	}
	_coinsLeft = coins;
	return _coinsLeft ;
}

void CollisionGrid::flipCollisionGrid() {

	// Create a temp 2D array bool elements
	bool** temp = (bool**) operator new ( sizeof(bool*) * _rows );

	int i,j;
	for ( i = 0 ; i < _rows; i++ ) {
		temp[i] = (bool*) operator new ( sizeof(bool) * _cols );
	}

	// make the temp populated
	for (i = 0 ; i < _rows; i++ ) {
		for (j = 0; j < _cols; j++ ) {
			temp[i][j] = _grid[_rows-i-1][j];
		}
	}

	// copy back to _grid
	for (i = 0 ; i < _rows; i++ ) {
		for (j = 0; j < _cols; j++ ) {
			_grid[i][j] = temp[i][j];
		}
	}

	for (i=0; i <_rows; i++){
		delete temp[i];
	}
	delete temp;

	_reversed = !_reversed;

}

void CollisionGrid::flipCoins() {

	// Create a temp 2D array bool elements
	bool** temp = (bool**) operator new ( sizeof(bool*) * _rows );

	int i,j;
	for ( i = 0 ; i < _rows; i++ ) {
		temp[i] = (bool*) operator new ( sizeof(bool) * _cols );
	}

	// make the temp populated
	for (i = 0 ; i < _rows; i++ ) {
		for (j = 0; j < _cols; j++ ) {
			temp[i][j] = _coins[_rows-i-1][j];
		}
	}

	// copy back to _grid
	for (i = 0 ; i < _rows; i++ ) {
		for (j = 0; j < _cols; j++ ) {
			_coins[i][j] = temp[i][j];
		}
	}

	for (i=0; i <_rows; i++){
		delete temp[i];
	}
	delete temp;


}

void CollisionGrid::populateCoins(){
	
	int i,j, random;
	int coinCount = 0;

	srand( time(NULL) );

	// clear any old coisn if they exist
	for (i = 0 ; i < _rows; i++ ) {
		for (j = 0; j < _cols; j++ ) {
			_coins[i][j] = false;
			_coinsCopy[i][j] = false;
		}
	}
	// populate
	for ( i = 2 ; coinCount <=9 ; i++ ) {
		random = rand()%(_cols-2) + 1;
		if (random % 2) {
			_coins[i][ random ] = true;
			_coinsCopy[i][ random ] = true;
			coinCount++;
		}
		if (i == (_rows - 2)){
			i = 2;
		}
	}

}


void CollisionGrid::resetCoins() {
	int i, j;
	for (i = 0 ; i < _rows; i++ ) {
		for (j = 0; j < _cols; j++ ) {
			_coins[i][j] = _coinsCopy[i][j];
		}
	}
}

bool CollisionGrid::getReversed(){
	return _reversed;
}

void CollisionGrid::populateCoinsFile() {

	std::ifstream gridFile;
	gridFile.open(MAP_COIN_FILE);
	std::string lineRead;


	#ifndef CONSOLE
	std::cout << "Reading Map File:  " << MAP_COIN_FILE << "." << std::endl;
	#endif
	
	// NEED TO FIX TO DO ERROR CHECKING AND CONFIRM NAMES
	if (gridFile.is_open()){

		
		// Create a charArray with the tile types
		_coins = (bool**) operator new (sizeof(bool*) * _rows);
		int i,j;
		for (i = 0; i < _rows; i++){
			_coins[i] = (bool*) operator new (sizeof(bool) * _cols);
		}

		
		for (i = 0; i < _rows; i++){
			if(gridFile.good()) {
				std::getline(gridFile, lineRead);
			}
			for (j = 0; j < _cols; j++){
				_coins[i][j] = (lineRead[j] == '1') ? true : false;
			}
		}


		// Print out the grid on the console
		#ifndef CONSOLE
		std::cout << "Coin Grid extracted from '" << MAP_COIN_FILE << "'\n\n";
		for (i = 0; i < _rows; i++){
			std::cout << " ";
			for (j = 0; j < _cols; j++){
				if (_coins[i][j]) std::cout << (char)178;
				else std::cout << ' ';
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		#endif
		
	}

	gridFile.close();

}

//void parseCollisionMapFile(CollisionGrid* &grid, const char* mapFileName){
//
//	std::ifstream gridFile;
//	gridFile.open(mapFileName);
//	std::string lineRead;
//
//	int rows;
//	int cols;
//	int pixelsPerTile;
//
//	#ifndef CONSOLE
//	std::cout << "Reading Map File:  " << mapFileName << "." << std::endl;
//	#endif
//	
//	// NEED TO FIX TO DO ERROR CHECKING AND CONFIRM NAMES
//	if (gridFile.is_open()){
//
//		// TileX
//		if(gridFile.good()){
//			std::getline(gridFile, lineRead);
//			size_t found = lineRead.find_first_of(":");
//			rows = atoi((lineRead.substr(found+1)).c_str());
//			#ifndef CONSOLE
//			std::cout << "Rows: " << rows << std::endl;
//			#endif
//		}
//		// TileY
//		if(gridFile.good()){
//			std::getline(gridFile, lineRead);
//			size_t found = lineRead.find_first_of(":");
//			cols = atoi((lineRead.substr(found+1)).c_str());
//			#ifndef CONSOLE
//			std::cout << "Cols: " << cols << std::endl;
//			#endif
//		}
//		// Pixels per Tile
//		if(gridFile.good()){
//			std::getline(gridFile, lineRead);
//			size_t found = lineRead.find_first_of(":");
//			pixelsPerTile = atoi((lineRead.substr(found+1)).c_str());
//			#ifndef CONSOLE
//			std::cout  << "Pixels Per Tile: " << pixelsPerTile << std::endl;
//			#endif
//		}
//		
//		// Create a charArray with the tile types
//		bool ** collisionArray;
//		collisionArray = (bool**) operator new (sizeof(bool*) * rows);
//		int i,j;
//		for (i = 0; i < rows; i++){
//			collisionArray[i] = (bool*) operator new (sizeof(bool) * cols);
//		}
//
//		
//		for (i = 0; i < rows; i++){
//			if(gridFile.good()) {
//				std::getline(gridFile, lineRead);
//			}
//			for (j = 0; j < cols; j++){
//				collisionArray[i][j] = (lineRead[j] == '1') ? true : false;
//			}
//		}
//
//
//		// Print out the grid on the console
//		#ifndef CONSOLE
//		std::cout << "Collision Grid extracted from '" << mapFileName << "'\n\n";
//		for (i = 0; i < rows; i++){
//			std::cout << " ";
//			for (j = 0; j < cols; j++){
//				if (collisionArray[i][j]) std::cout << (char)178;
//				else std::cout << ' ';
//			}
//			std::cout << std::endl;
//		}
//		std::cout << std::endl;
//		#endif
//		
//		// Create and populate the CollisionGrid
//		grid = new CollisionGrid(rows, cols, pixelsPerTile);
//
//		for (i = 0; i < rows; i++){
//			for (j = 0; j < cols; j++){
//				 if( collisionArray[i][j] ) {
//					 grid->addCollisionTile(i,j);
//				 }
//			}
//		}
//
//	}
//
//	gridFile.close();
//}

