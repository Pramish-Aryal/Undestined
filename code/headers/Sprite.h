#pragma once

#include "types.h"
#include <string>
//#include <SDL2/SDL.h>

class Graphics;
struct SDL_Texture;

using namespace types;

class Sprite
{
	
	public:
	Sprite() {}
	Sprite(Graphics& graphics, std::string path);
	virtual ~Sprite() {}
	
	virtual void draw(Graphics& graphics, i32 dest_x, i32 dest_y, r32 scale);
	virtual void update(r32 dt) {}
	
	//static void load_sprite(char* path); //should I??
	
	protected:
	SDL_Texture* m_texture;
	types::i32 m_width, m_height;
	types::u8* m_pixels;
};