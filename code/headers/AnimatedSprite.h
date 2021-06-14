#pragma once

#include "Sprite.h"

class AnimatedSprite: public Sprite
{

public:
	AnimatedSprite() {}
	AnimatedSprite(Graphics& graphics, std::string path);
	void update(types::r32 dt);
	void draw(Graphics& graphics);
private:
	float elapsedTime;
	int currentFrame;
	
};