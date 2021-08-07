#include "Background.h"

#include "Graphics.h"
#include "Camera.h"
#include <SDL2/SDL.h>

using namespace types;

#define COUNT(array) (sizeof((array)) / sizeof(array[0]))

Background::Background(Graphics &graphics, std::string path) : tile_set(Sprite(graphics, path)) {}

void Background::draw(Graphics &graphics, r32 scale)
{
	SDL_Rect rect = {0, 0, 288, 180};
	SDL_Rect dest_rect = { - 1280, -720, 1280 * 4, 720 * 4};
	tile_set.draw(graphics, rect, dest_rect, scale);
}
