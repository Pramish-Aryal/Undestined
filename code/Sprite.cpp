#include "Sprite.h"
#include "Graphics.h"
#include <SDL2/SDL.h>

#include "Fatal.h"
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <stb_image.h>

using namespace types;

Sprite::Sprite(Graphics &graphics, std::string path)
{
	int n;
	m_pixels = NULL;
	m_pixels = stbi_load(path.c_str(), &m_width, &m_height, &n, 4);
	if (!m_pixels)
		Fatal::fatal_error("Can't load image");
	m_texture = SDL_CreateTexture(graphics.get_renderer(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, m_width, m_height);
	SDL_UpdateTexture(m_texture, 0, m_pixels, m_width * 4);
	SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);
}

void Sprite::draw(Graphics &graphics, i32 dest_x, i32 dest_y, r32 scale)
{
	graphics.blit_surface(m_texture, {dest_x, dest_y, m_width, m_height}, {dest_x, dest_y, m_width * (int)scale, m_height * (int)scale}, {0, 0});
}

void Sprite::set_flip(bool flip)
{
	this->flip = flip;
}

Sprite::~Sprite()
{
	SDL_DestroyTexture(m_texture);
	stbi_image_free(m_pixels);
}