#pragma once

#include "utils.h"

class AnimatedSprite;
class Graphics;

class Player
{
	public:
	Player(Graphics &graphics);
	~Player();
	
	void draw(Graphics &graphics);
	void simulate(types::r32 dt);
	void update(types::r32 dt);
	
	//player movement
	void move_left();
	void move_right();
	void stop_moving();
	void attack();
	void jump();
	
	private:
	void setup_animations();
	
	private:
	Vec2f pos;
	// TODO(Pramish): Implement basic jumping and movement physics using them i.e. eqns of motion
	Vec2f vel;
	Vec2f accn;
	AnimatedSprite *sprite;
	types::r32 scale = 3;
};