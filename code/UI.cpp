#include "UI.h"

#include <cstring>
#include <iostream>
Font::Font(Graphics& graphics) {
	atlas = graphics.load_image_surface("data\\font_atlas.png", width, height, true);
	for (i32 i = 32; i < 128; ++i) {
		i32 index = i - 32;
		i32 col = index % FONT_COLS;
		i32 row = index / FONT_COLS;
		glyph_table[index] = {
			col * FONT_CHAR_WIDTH,
			row * FONT_CHAR_HEIGHT,
			FONT_CHAR_WIDTH,
			FONT_CHAR_HEIGHT};
	}
}

void Font::render_char(Graphics& graphics, char c, Vec2f pos, r32 scale) {
	SDL_Rect dest = {
		(i32)pos.x,
		(i32)pos.y,
		(i32)(FONT_CHAR_WIDTH * scale),
		(i32)(FONT_CHAR_HEIGHT * scale)};
	
	size_t index = '?' - 32;
	if (c >= 32 && c < 128) index = c - 32;
	SDL_RenderCopy(graphics.get_renderer(), this->atlas, &this->glyph_table[index], &dest);
}

void Font::render_text_sized(Graphics& graphics, const char* text, size_t text_size, Vec2f pos, r32 scale, u32 color) {
	set_font_color(atlas, color);
	for (u32 i = 0; i < text_size; i++) {
		render_char(graphics, text[i], pos, scale);
		pos.x += FONT_CHAR_WIDTH * scale;
	}
}

void Font::render_text(Graphics& graphics, const char* text, Vec2f pos, r32 scale, u32 color) {
	render_text_sized(graphics, text, strlen(text), pos, scale, color);
}

void Font::set_font_color(SDL_Texture* texture, u32 color) {
	SDL_SetTextureColorMod(texture, (color >> 24) & 0xff, (color >> 16) & 0xff, (color >> 8) & 0xff);
	SDL_SetTextureAlphaMod(texture, (color)&0xff);
}

//MENU

Menu::Menu(Graphics& graphics) {
	tutorial_sprite = new Sprite(graphics, "data\\controls.png");
}

Menu::~Menu() {
	delete tutorial_sprite;
}

void Menu::draw_menu(Graphics& graphics, Font& font) {
	font.render_text(graphics, "Undestined", Vec2f(1280, 720) / 2 - Vec2f(180, 225), 5);
	font.render_text(graphics, "Play", Vec2f(1280, 720) / 2 + Vec2f(-55, -75), 3, start_color);
	font.render_text(graphics, "Tutorial", Vec2f(1280, 720) / 2 + Vec2f(-90, -25), 3, tutorial_color);
	font.render_text(graphics, "Quit", Vec2f(1280, 720) / 2 + Vec2f(-55, 25), 3, quit_color);
}
#include <iostream>
void Menu::draw_gameover(Graphics& graphics, Font& font, i32 score) {
	font.render_text(graphics, "Game Over", Vec2f(1280, 720) / 2 - Vec2f(150, 225), 5);
	font.render_text(graphics, "Menu", Vec2f(1280, 720) / 2 + Vec2f(-55, -75), 3, start_color);
	std::stringstream ss;
	ss << "Your score: " << score;
	std::cout << ss.str() << std::endl;
	font.render_text(graphics, ss.str().c_str(), Vec2f(1280, 720) / 2 + Vec2f(-105, -115), 2);
}

void Menu::draw_score(Graphics& graphics, Font& font, i32 score) {
	std::stringstream ss;
	ss << "Kill Count: " << score;
	font.render_text(graphics, ss.str().c_str(), Vec2f(1280 - 250, 5), 2);
}

void Menu::draw_pause(Graphics& graphics, Font& font) {
	font.render_text(graphics, "Paused", Vec2f(1280, 720) / 2 - Vec2f(110, 225), 5);
	font.render_text(graphics, "Resume", Vec2f(1280, 720) / 2 + Vec2f(-75, -75), 3, start_color);
	font.render_text(graphics, "Menu", Vec2f(1280, 720) / 2 + Vec2f(-55, -25), 3, tutorial_color);
}

void Menu::update_menu(Vec2f mouse, void* game_state, bool clicked) {
	if ((*(i32*)game_state) == 1) {
		if (Collider::point_vs_rect(mouse, start_collider)) {
			start_color = 0xff0000ff;
			if (clicked) *(i32*)game_state = 2;  //PLAY
		} else {
			start_color = 0xffffffff;
		}
		
		if (Collider::point_vs_rect(mouse, tutorial_collider)) {
			tutorial_color = 0xff0000ff;
			if (clicked) *(i32*)game_state = 4;  //TUTORIAL
		} else {
			tutorial_color = 0xffffffff;
		}
		
		if (Collider::point_vs_rect(mouse, quit_collider)) {
			quit_color = 0xff0000ff;
			if (clicked) *(i32*)game_state = 5;
		} else {
			quit_color = 0xffffffff;
		}
	} else if ((*(i32*)game_state) == 3) {
		if (Collider::point_vs_rect(mouse, start_collider)) {
			start_color = 0xff0000ff;
			if (clicked) *(i32*)game_state = 2;  //PLAY
		} else {
			start_color = 0xffffffff;
		}
		
		if (Collider::point_vs_rect(mouse, tutorial_collider)) {
			tutorial_color = 0xff0000ff;
			if (clicked) *(i32*)game_state = 1;  //MENU
		} else {
			tutorial_color = 0xffffffff;
		}
	} else if (*(i32*)game_state == 6) {
		if (Collider::point_vs_rect(mouse, start_collider)) {
			start_color = 0xff0000ff;
			if (clicked) {
				*(i32*)game_state = 1;  //MENU
			}
		} else {
			start_color = 0xffffffff;
		}
	}
}

void Menu::draw_tutorial(Graphics& graphics) {
	tutorial_sprite->static_draw(graphics, {0, 0, 1280, 720}, {0, 0, 1280, 720}, 1);
}