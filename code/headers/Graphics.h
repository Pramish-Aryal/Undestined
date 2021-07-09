#pragma once

#include "types.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Rect;
struct SDL_Point;

using namespace types;

class Graphics
{
public:
	Graphics();
	~Graphics();

	void clear_screen(u8 r, u8 g, u8 b);
	void Graphics::blit_surface(SDL_Texture *texture, SDL_Rect src_rect, SDL_Rect dest_rect, SDL_Point center, bool left = false);

	SDL_Renderer *get_renderer();
	void display();

private:
	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
};
