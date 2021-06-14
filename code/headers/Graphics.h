#pragma once

#include "types.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Rect;

class Graphics
{

public:
	Graphics();
	~Graphics();
	
	void clear_screen(types::u8 r, types::u8 g, types::u8 b);
	void blit_surface(SDL_Texture* texture, SDL_Rect src_rect, SDL_Rect dest_rect);
	
	SDL_Renderer* get_renderer();
	void display();
	
private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
};

