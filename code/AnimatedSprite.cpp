
#include "AnimatedSprite.h"
#include "Graphics.h"
#include <stb_image.h>
#include <SDL2/SDL.h>


AnimatedSprite::AnimatedSprite(Graphics& graphics, std::string path):
Sprite(graphics, path)
{
	elapsedTime = 0;
	currentFrame = 0;
}


void AnimatedSprite::update(r32 dt)
{
	float frameTime = 95.f;//67.0f;
	elapsedTime += dt;
	if (elapsedTime > frameTime) {
		currentFrame++;
		elapsedTime = 0;
		if (!(currentFrame < 6))
			currentFrame = 0;
	}
}

void AnimatedSprite::draw(Graphics& graphics, i32 dest_x, i32 dest_y, r32 scale)
{
	SDL_Rect srcRect = {50 * currentFrame, 0, 50, m_height};
	SDL_Rect destRect = {dest_x, dest_y, srcRect.w * (int)scale, srcRect.h * (int)scale};
	
	graphics.blit_surface(m_texture, srcRect, destRect);
}