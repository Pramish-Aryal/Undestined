#pragma once

#include "Sprite.h"
#include <string>
#include <vector>
#include <SDL2/SDL.h>

class Map {
	Sprite tile_set;
	public:
	std::vector<SDL_Rect> tiles;
	std::vector<SDL_Rect> draw_tiles;
	Map(Graphics& graphics);
	void load_map(std::string path);
	void draw(Graphics& graphics);
	
};