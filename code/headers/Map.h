#pragma once

#include "Sprite.h"
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "Collision.h"

class Map {
	Sprite tile_set;
	std::vector<SDL_Rect> tiles;
	public:
	std::vector<SDL_Rect> draw_tiles;
	std::vector<Rect> bounding_boxes;
	Map(Graphics& graphics);
	void load_map(std::string path);
	void draw(Graphics& graphics);
	
};