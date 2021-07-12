#pragma once

#include "utils.h"

class AnimatedSprite;
class Graphics;

class Player
{
public:
	Player(Graphics &graphics);
	void draw(Graphics &graphics);
	void simulate(types::r32 dt);
	void update(types::r32 dt);
	~Player();
	void move_left();
	void move_right();
	void jump();

private:
	void setup_animations();

private:
	Vec2f pos;
	AnimatedSprite *sprite;
	types::r32 scale = 3;
};