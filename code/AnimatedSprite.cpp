
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

void AnimatedSprite::update(types::r32 dt)
{
	float frameTime = 67.0f;
	elapsedTime += dt;
	if (elapsedTime > frameTime) {
		currentFrame++;
		elapsedTime = 0;
		if (!(currentFrame < 8))
			currentFrame = 0;
	}
}

void AnimatedSprite::draw(Graphics& graphics)
{
	SDL_Rect srcRect = {80 * currentFrame, 0, 80, m_height};
	SDL_Rect destRect = {0, 0, srcRect.w * (int)3, srcRect.h * (int)3};

	graphics.blit_surface(m_texture, srcRect, destRect);
}