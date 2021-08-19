#pragma once

#include "utils.h"
#include "Graphics.h"
#include "Collision.h"
#include "Sprite.h"
#include <SDL2/SDL.h>
#include <sstream>

#define FONT_COLS 18
#define FONT_ROWS 7
#define FONT_WIDTH 128
#define FONT_HEIGHT 64
#define FONT_CHAR_WIDTH (FONT_WIDTH / FONT_COLS)
#define FONT_CHAR_HEIGHT (FONT_HEIGHT / FONT_ROWS)

class Font
{
	SDL_Texture* atlas;
	i32 width, height;
	SDL_Rect glyph_table[128 - 32];
	void render_char(Graphics& graphics, char c, Vec2f pos, types::r32 scale);
	void render_text_sized(Graphics& graphics, const char* text, size_t text_size, Vec2f pos, types::r32 scale, types::u32 color = 0xffffffff);
	void set_font_color(SDL_Texture* texture, types::u32 color);
	public:
	Font(Graphics& graphics);
	void render_text(Graphics& graphics, const char* text, Vec2f pos, types::r32 scale, types::u32 color = 0xffffffff);
};

class Menu
{
	Sprite* tutorial_sprite;
	u32 start_color = 0xffffffff, tutorial_color = 0xffffffff, quit_color = 0xffffffff;
	Rect start_collider = { Vec2f(1280, 720) / 2  + Vec2f( -55, -75), {87, 28}}, 
	tutorial_collider = { Vec2f(1280, 720) / 2  + Vec2f( -90, -25), {168, 28}}, 
	quit_collider = { Vec2f(1280, 720) / 2  + Vec2f( -55, 25), {82, 28}};
	public:
	Menu(Graphics& graphics);
	~Menu();
	void update_menu(Vec2f mouse, void* game_state, bool clicked);
	void draw_tutorial(Graphics& graphics);
	void draw_menu(Graphics& graphics, Font& font);
	void draw_score(Graphics& graphics, Font& font, i32 score);
	void draw_pause(Graphics& graphics, Font& font);
	void draw_gameover(Graphics& graphics, Font& font);
};
