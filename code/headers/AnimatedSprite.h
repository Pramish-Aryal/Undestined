#pragma once

#include "Sprite.h"

#include <map>
#include <vector>
#include <string>

using namespace types;

//forward declaring it here
struct SDL_Rect;

class AnimatedSprite : public Sprite
{
	float elapsed_time;
	int current_frame;
	std::string current_animation;

	std::map<std::string, std::vector<SDL_Rect>> animations;
	std::map<std::string, r32> frame_times;

public:
	bool left = false;
	AnimatedSprite() {}
	AnimatedSprite(Graphics &graphics, std::string path);
	void update(r32 dt) override;
	virtual void draw(Graphics &graphics, i32 dest_x, i32 dest_y, r32 scale) override;
	void AnimatedSprite::add_animation(std::string name, i32 srcX, i32 srcY, i32 w, i32 h, i32 frames, i32 frames_per_sec);
	void play_animation(std::string animation_name);
};