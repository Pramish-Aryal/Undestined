#pragma once

#include "Sprite.h"
#include <vector>
#include <SDL2/SDL.h>
#include "Collision.h"
#include "utils.h"

class Camera;

class Backdrop {
	Sprite tile_set;
	std::vector<SDL_Rect> draw_tiles;
	std::vector<Rect> positions;
	public:
	Backdrop(Graphics& graphics);
	void load_backdrop_menu(types::r32 scale = 1.f);
	void load_backdrop_game(types::r32 scale = 1.f);
	void draw(Graphics& graphics, types::r32 scale = 1.f);
};