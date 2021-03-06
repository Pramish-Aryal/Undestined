#pragma once

#include "utils.h"
#include <string>
#include <SDL2/SDL.h>

class Graphics;
struct SDL_Texture;

class Sprite
{

public:
	Sprite() {}
	Sprite(Graphics &graphics, std::string path);
	virtual ~Sprite();

	virtual void draw(Graphics &graphics, types::i32 dest_x, types::i32 dest_y, types::r32 scale);
	void draw(Graphics &graphics, SDL_Rect src, SDL_Rect dest, types::r32 scale);
	void static_draw(Graphics &graphics, SDL_Rect src, SDL_Rect dest, types::r32 scale);
	virtual void update(types::r32 dt) {}

	void set_flip(bool flip);
	bool get_flip();
	//static void load_sprite(char* path); //should I??

protected:
	bool flip = false;
	SDL_Texture *m_texture;
	types::i32 m_width, m_height;
};