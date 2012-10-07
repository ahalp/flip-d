#include "GameData.h"
#include "Display.h"

using namespace std;




GameData::GameData(void)
{
	
}


GameData::~GameData(void)
{

}

// Traverse through the map to see if the file has been preloaded. if not load it.
// Return the cooresponding SDL_Surface.
//
// NOTE: This might be inefficient if used often. 
//	 Either find a way to cache the data so you don't need to traverse the map
//	 Or don't use this too much.
SDL_Surface* GameData::getSurface(const string & fileName , GameToolsStruct &gameTools){
	
	map<string, SDL_Surface*>::iterator find_itr = _surfaces.find( fileName );
	
	if (  find_itr == _surfaces.end() ){
		_surfaces[fileName] = gameTools.Display->loadImage(fileName.c_str());
		return _surfaces[fileName];
	} else {
		return _surfaces[fileName];
	}
}