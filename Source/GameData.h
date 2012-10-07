// Class GameData: contains all the data (images & audio) used in the game. 
// Data is loaded when needed, and kept for the remainder of the level / state.

#pragma once

#include "SDL.h"
#include "Utilities.h"
#include <map>
#include <string>



class GameData
{

private:
	std::map <std::string, SDL_Surface*> _surfaces;
	
public:
	GameData(void);
	~GameData(void);

	SDL_Surface* getSurface(const std::string &surfaceName, GameToolsStruct &gameTools);
};

