#include "Sprite.h"
#include "Graphics.h"
#include <SDL2/SDL.h>

#include "Fatal.h"
#include <string>


using namespace types;

Sprite::Sprite(Graphics &graphics, std::string path)
{
	m_texture = graphics.load_image(path, m_width, m_height);
}

void Sprite::draw(Graphics &graphics, i32 dest_x, i32 dest_y, r32 scale)
{
	graphics.blit_surface(m_texture, {dest_x, dest_y, m_width, m_height}, {dest_x, dest_y, m_width * (int)scale, m_height * (int)scale}, {0, 0});
}


void Sprite::draw(Graphics &graphics, SDL_Rect src, SDL_Rect dest, r32 scale)
{
	graphics.blit_surface(m_texture, src, {dest.x, dest.y, dest.w * (int)scale, dest.h * (int)scale}, {0, 0});
}

void Sprite::set_flip(bool flip)
{
	this->flip = flip;
}

Sprite::~Sprite() {}