#pragma once

#include "types.h"
#include <string>
//#include <SDL2/SDL.h>

class Graphics;
struct SDL_Texture;

class Sprite
{
	public:
	Sprite() {}
	Sprite(Graphics& graphics, std::string path);
	virtual ~Sprite() {}
	
	void draw(Graphics& graphics, types::i32 dest_x, types::i32 dest_y, types::r32 scale);
	
	private:
	SDL_Texture* m_texture;
	types::i32 m_width, m_height;
	types::u8* m_pixels;
};