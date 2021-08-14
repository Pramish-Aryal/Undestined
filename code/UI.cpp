#include "UI.h"
#include <cstring>

Font::Font(Graphics& graphics)
{
	atlas = graphics.load_image_surface("data\\font_atlas.png", width, height, true);
	for(i32 i = 32; i < 128; ++i)
	{
		i32 index = i - 32;
		i32 col = index % FONT_COLS;
		i32 row = index / FONT_COLS;
		glyph_table[index] = {
			col * FONT_CHAR_WIDTH,
			row * FONT_CHAR_HEIGHT,
			FONT_CHAR_WIDTH,
			FONT_CHAR_HEIGHT
		};
	}
}

void Font::render_char(Graphics& graphics, char c, Vec2f pos, float scale)
{
	SDL_Rect dest = {
		(i32)pos.x, 
		(i32)pos.y,
		(i32)(FONT_CHAR_WIDTH * scale),
		(i32)(FONT_CHAR_HEIGHT * scale)
	};
	
	size_t index = '?' - 32;
	if(c >= 32 && c < 128) index = c - 32;
	SDL_RenderCopy(graphics.get_renderer(), this->atlas, &this->glyph_table[index], &dest);
}


void Font::render_text_sized(Graphics& graphics, const char* text, size_t text_size, Vec2f pos, float scale)
{
	Vec2f pen = pos;
	for(u32 i = 0; i < text_size; i++){
		render_char(graphics, text[i], pen, scale);
		pen.x += FONT_CHAR_WIDTH * scale;
	}
}

void Font::render_text(Graphics& graphics, const char* text, Vec2f pos, float scale)
{
	render_text_sized(graphics, text, strlen(text), pos, scale);
}

Font::~Font()
{
	SDL_DestroyTexture(atlas);
}