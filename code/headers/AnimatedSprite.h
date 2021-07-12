#pragma once

#include "Sprite.h"

#include <map>
#include <vector>
#include <string>

//forward declaring it here
struct SDL_Rect;

class AnimatedSprite : public Sprite
{
	float elapsed_time;
	int current_frame;
	std::string current_animation;

	std::map<std::string, std::vector<SDL_Rect>> animations;
	std::map<std::string, types::r32> frame_times;

public:
	AnimatedSprite(Graphics &graphics, std::string path);
	void update(types::r32 dt) override;
	virtual void draw(Graphics &graphics, types::i32 dest_x, types::i32 dest_y, types::r32 scale) override;
	void AnimatedSprite::add_animation(std::string name, types::i32 srcX, types::i32 srcY, types::i32 w, types::i32 h, types::i32 frames, types::i32 frames_per_sec);
	void play_animation(std::string animation_name);
};