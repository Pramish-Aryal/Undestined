#include "Graphics.h"
#include "Fatal.h"

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <stb_image.h>

Graphics::Graphics()
{
	m_screen_width = 1280;
	m_screen_height = 720;
	m_window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screen_width, m_screen_height,  SDL_WINDOW_RESIZABLE /*| SDL_WINDOW_FULLSCREEN*/);
	if (!m_window)
		Fatal::fatal_error("Couldn't create Window");
	//SDL_SetWindowBordered(m_window, SDL_FALSE);
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (!m_renderer)
		Fatal::fatal_error("Couldn't create the renderer");
	SDL_RenderSetLogicalSize(m_renderer, m_screen_width, m_screen_height);
}

Graphics::~Graphics()
{
	for(SpriteSheet::iterator sprite_sheet = m_sprite_sheets.begin(); sprite_sheet != m_sprite_sheets.end(); sprite_sheet++)
	{
		if(sprite_sheet->second) SDL_DestroyTexture(sprite_sheet->second);
	}
	
	if (m_renderer) SDL_DestroyRenderer(m_renderer);
	if (m_window) SDL_DestroyWindow(m_window);
}

SDL_Texture* Graphics::load_image(std::string path, i32& width, i32& height)
{
	if(m_sprite_sheets.count(path) == 0)
	{
		int n;
		u8* pixels = NULL;
		pixels = stbi_load(path.c_str(), &width, &height, &n, 4);
		if (!pixels)
			Fatal::fatal_error("Can't load image");
		SDL_Texture* texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, width, height);
		SDL_UpdateTexture(texture, 0, pixels, width * 4);
		stbi_image_free(pixels);
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		m_sprite_sheets[path] = texture;
		m_sprite_sheet_sizes[path] = {width, height};
	}
	width = m_sprite_sheet_sizes[path].width;
	height = m_sprite_sheet_sizes[path].height;
	return m_sprite_sheets[path];
}

SDL_Texture* Graphics::load_image_surface(std::string path, i32& width, i32& height, bool key)
{
	if(m_sprite_sheets.count(path) == 0)
	{
		u8* pixels = stbi_load(path.c_str(), &width, &height, 0, 4);
		std::string error = std::string("Can't load image: ") + path;
		if (!pixels) Fatal::fatal_error(error.c_str());
		SDL_Surface* surface =  SDL_CreateRGBSurfaceFrom(pixels, width, height, 32, 4 * width, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
		if(key) SDL_SetColorKey(surface, SDL_TRUE, 0xff000000);
		m_sprite_sheets[path] = SDL_CreateTextureFromSurface(m_renderer, surface);
	}
	return m_sprite_sheets[path];
}

void Graphics::clear_screen(u8 r, u8 g, u8 b)
{
	SDL_SetRenderDrawColor(m_renderer, r, g, b, 255);
	SDL_RenderClear(m_renderer);
}

void Graphics::display()
{
	SDL_RenderPresent(m_renderer);
}

SDL_Renderer *Graphics::get_renderer()
{
	return m_renderer;
}

void Graphics::blit_surface(SDL_Texture *texture, SDL_Rect src_rect, SDL_Rect dest_rect, SDL_Point center, bool flip)
{
	SDL_RenderCopyEx(m_renderer, texture, &src_rect, &dest_rect, 0, &center, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

Vec2f Graphics::get_display_resolution()
{
	SDL_GetWindowSize(m_window, &m_screen_width, &m_screen_height);
	return Vec2f(m_screen_width, m_screen_height);
}