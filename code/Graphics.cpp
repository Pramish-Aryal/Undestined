#include "Graphics.h"
#include "Fatal.h"

#include <SDL2/SDL.h>

Graphics::Graphics()
{
	m_window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE);
	if (!m_window)
		Fatal::fatal_error("Couldn't create Window");
	
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (!m_renderer)
		Fatal::fatal_error("Couldn't create the renderer");
	
}

Graphics::~Graphics()
{
	if(m_window)
		SDL_DestroyWindow(m_window);
	if(m_renderer)
		SDL_DestroyRenderer(m_renderer);
}

void Graphics::clear_screen(u8 r, u8 g, u8  b)
{
	SDL_SetRenderDrawColor(m_renderer, r, g, b, 255);
	SDL_RenderClear(m_renderer);
}

void Graphics::display()
{
	SDL_RenderPresent(m_renderer);
}

SDL_Renderer* Graphics::get_renderer()
{
	return m_renderer;
}

void Graphics::blit_surface(SDL_Texture* texture, SDL_Rect src_rect, SDL_Rect dest_rect)
{
	SDL_RenderCopy(m_renderer, texture, &src_rect, &dest_rect);
}