#pragma once

#include "types.h"
struct SDL_Texture;

class Sprite
{
	public:
	Sprite();
	~Sprite();
	
	void draw();
	
	private:
	SDL_Texture* m_texture;
};