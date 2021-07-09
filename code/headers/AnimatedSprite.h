#pragma once

#include "Sprite.h"

using namespace types;

class AnimatedSprite: public Sprite
{
	
	public:
	AnimatedSprite() {}
	AnimatedSprite(Graphics& graphics, std::string path);
	void update(r32 dt) override;
	virtual void draw(Graphics& graphics, i32 dest_x, i32 dest_y, r32 scale) override;
	private:
	float elapsedTime;
	int currentFrame;
	
};