#include "Sprite.h"
#include "Graphics.h"
#include "Camera.h"
#include <SDL2/SDL.h>

#include "Fatal.h"
#include <string>

using namespace types;

Sprite::Sprite(Graphics &graphics, std::string path)
{
	m_texture = graphics.load_image(path, m_width, m_height);
}

void Sprite::draw(Graphics &graphics, i32 dest_x, i32 dest_y, r32 scale)
{
	//graphics.blit_surface(m_texture, {dest_x, dest_y, m_width, m_height}, {dest_x - (i32)camera.pos.x, dest_y - (i32)camera.pos.y, m_width * (int)scale, m_height * (int)scale}, {0, 0});
	graphics.blit_surface(m_texture, {dest_x, dest_y, m_width, m_height}, {dest_x, dest_y, m_width * (int)scale, m_height * (int)scale}, {0, 0});
}


void Sprite::draw(Graphics &graphics, SDL_Rect src, SDL_Rect dest, r32 scale)
{
	//graphics.blit_surface(m_texture, src, {dest.x - (i32)camera.pos.x, dest.y - (i32)camera.pos.y, dest.w * (int)scale, dest.h * (int)scale}, {0, 0});
	Vec2f offset = Camera::get_instance().get_pos();
	graphics.blit_surface(m_texture, src, {dest.x + (i32)offset.x, dest.y + (i32)offset.y, (int)(dest.w * scale), (int)(dest.h * scale)}, {0, 0});
}

void Sprite::static_draw(Graphics &graphics, SDL_Rect src, SDL_Rect dest, r32 scale)
{
	//graphics.blit_surface(m_texture, src, {dest.x - (i32)camera.pos.x, dest.y - (i32)camera.pos.y, dest.w * (int)scale, dest.h * (int)scale}, {0, 0});
	Vec2f offset = Camera::get_instance().get_pos();
	graphics.blit_surface(m_texture, src, {dest.x, dest.y, (int)(dest.w * scale), (int)(dest.h * scale)}, {0, 0});
}


void Sprite::set_flip(bool flip)
{
	this->flip = flip;
}

bool Sprite::get_flip()
{
	return this->flip;
}

Sprite::~Sprite() {}