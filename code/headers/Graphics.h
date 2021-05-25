#pragma once

#include "types.h"

struct SDL_Window;
struct SDL_Renderer;

class Graphics
{
	public:
	Graphics();
	~Graphics();
	
	void clear_screen(types::u8 r, types::u8 g, types::u8 b);
	//void blit_to_screen();
	
	void display();
	
	private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
};

