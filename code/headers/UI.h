#pragma once

#include "utils.h"
#include "Graphics.h"
#include <SDL2/SDL.h>
using namespace types;

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
	void render_char(Graphics& graphics, char c, Vec2f pos, float scale);
	void render_text_sized(Graphics& graphics, const char* text, size_t text_size, Vec2f pos, float scale);
	public:
	Font(Graphics& graphics);
	void render_text(Graphics& graphics, const char* text, Vec2f pos, float scale);
};
#include <iostream>
class Menu
{
	public:
	void draw_menu(Graphics& graphics, Font& font)
	{
		font.render_text(graphics, "Undestined", Vec2f(1280, 720) / 2 - Vec2f(200, 0), 5);
		font.render_text(graphics, "Press Enter to begin", Vec2f(1280, 720) / 2  + Vec2f( -225, 150), 3);
	}
	
	void draw_score(Graphics& graphics, Font& font, i32 score)
	{
		std::stringstream ss;
		ss << "Kill Count: " << score;
		font.render_text(graphics, ss.str().c_str(), Vec2f(1280 - 250, 5), 2);
	}
	
	void draw_pause(Graphics& graphics, Font& font)
	{
		font.render_text(graphics, "Paused", Vec2f(1280, 720) / 2 - Vec2f(125, 0), 5);
		font.render_text(graphics, "Press Enter to resume", Vec2f(1280, 720) / 2 + Vec2f( -225, 150), 3);
		font.render_text(graphics, "Press Escape to Quit", Vec2f(1280, 720) / 2 + Vec2f( -225, 200), 3);
	}
};
