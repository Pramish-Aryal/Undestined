#pragma once

#pragma once

#include "Sprite.h"
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include "utils.h"

class Camera;

class Background{
	Sprite tile_set;
	public:
	Background(Graphics& graphics, std::string path = "data\\background.png");
	void draw(Graphics& graphics, types::r32 scale = 1.f);
};