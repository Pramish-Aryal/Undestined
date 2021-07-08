#pragma once

#include "Sprite.h"

class AnimatedSprite: public Sprite
{
	
	public:
	AnimatedSprite() {}
	AnimatedSprite(Graphics& graphics, std::string path);
	void update(types::r32 dt) override;
	virtual void draw(Graphics& graphics, types::i32 dest_x, types::i32 dest_y, types::r32 scale) override;
	private:
	float elapsedTime;
	int currentFrame;
	
};