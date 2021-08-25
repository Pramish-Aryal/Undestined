#pragma once

#include "utils.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Rect;
struct SDL_Point;

using namespace types;

#include <string>
#include <map>

struct TextureSize
{
	i32 width, height;
};

class Graphics
{
	public:
	Graphics();
	~Graphics();
	
	void clear_screen(u8 r, u8 g, u8 b);
	void blit_surface(SDL_Texture *texture, SDL_Rect src_rect, SDL_Rect dest_rect, SDL_Point center, bool flip = false);
	
	SDL_Renderer *get_renderer();
	SDL_Window* get_window();
	SDL_Texture* load_image(std::string path, types::i32& width, types::i32& height);
	SDL_Texture* load_image_surface(std::string path, i32& width, i32& height, bool key = false);
	void get_mouse(types::i32& x, types::i32& y);
	void display();
	Vec2f get_display_resolution();
	private:
	int m_screen_width, m_screen_height;
	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	typedef std::map<std::string, SDL_Texture*> SpriteSheet;
	typedef std::map<std::string, TextureSize> SpriteSheetSize;
	SpriteSheetSize m_sprite_sheet_sizes;
	SpriteSheet m_sprite_sheets;
};
