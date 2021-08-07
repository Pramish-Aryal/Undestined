#pragma once

#include "Sprite.h"
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "Collision.h"
#include "utils.h"

class Camera;

class Map {
	Sprite tile_set;
	std::vector<SDL_Rect> tiles;
	public:
	std::vector<SDL_Rect> draw_tiles;
	std::vector<Rect> bounding_boxes;
	Map(Graphics& graphics);
	void load_map(std::string path);
	void draw(Graphics& graphics, types::r32 scale = 1.f);
	void debug_draw(Graphics& graphics, types::u8 scale = 10);
};