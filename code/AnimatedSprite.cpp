
#include "AnimatedSprite.h"
#include "Graphics.h"
#include <stb_image.h>
#include <SDL2/SDL.h>

using namespace types;

AnimatedSprite::AnimatedSprite(){}
AnimatedSprite::AnimatedSprite(Graphics &graphics, std::string path) : Sprite(graphics, path), elapsed_time(0), current_frame(0), current_animation("") {}

void AnimatedSprite::update(r32 dt)
{
	//float frame_time = 95.f; //67.0f;
	elapsed_time += dt;
	if (elapsed_time > frame_times[current_animation])
	{
		current_frame++;
		elapsed_time = 0; //-= frame_times[current_animation];
		if (!(current_frame < animations[current_animation].size() - 1))
			current_frame = 0;
	}
}

void AnimatedSprite::draw(Graphics &graphics, i32 dest_x, i32 dest_y, r32 scale)
{
	//SDL_Rect srcRect = {50 * current_frame, 37, 50, 37};
	SDL_Rect srcRect = animations[current_animation][current_frame];
	SDL_Rect destRect = {dest_x, dest_y, srcRect.w * (int)scale, srcRect.h * (int)scale};
	
	graphics.blit_surface(m_texture, srcRect, destRect, {srcRect.w / 2, destRect.h / 2}, flip);
}

void AnimatedSprite::add_animation(std::string name, i32 srcX, i32 srcY, i32 w, i32 h, i32 frames, i32 frames_per_sec)
{
	std::vector<SDL_Rect> rects;
	//m_width = 7 * w;
	for (int i = 0; i < frames; i++)
	{
		int x = (srcX + i) * w;
		int y = srcY * h + (x >= m_width ? (x / m_width) * h : 0);
		x %= m_width;
		SDL_Rect rect = {x, y, w, h};
		rects.push_back(rect);
	}
	animations[name] = rects;
	frame_times[name] = 1000.f / frames_per_sec;
	//animations.insert(std::pair<std::string, std::vector<SDL_Rect>>(name, rects));
	//frame_times.insert(std::pair<std::string, r32>(name, 1000.f / frames_per_sec));
}

void AnimatedSprite::play_animation(std::string animation_name)
{
	if (current_animation != animation_name)
	{
		current_animation = animation_name;
		current_frame = 0;
	}
}