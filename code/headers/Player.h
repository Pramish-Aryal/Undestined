#pragma once

#include "utils.h"
#include "Collision.h"

class AnimatedSprite;
class Graphics;
class Map;

class Player
{
public:
	Player(Graphics &graphics);
	~Player();

	void draw(Graphics &graphics);
	void simulate(types::r32 dt, Map &map);
	void update(types::r32 dt);

	//player movement
	void move_left();
	void move_right();
	void stop_moving();
	void stop_falling();
	void attack();
	void jump();
	void fall();
	void roll();
	void get_hurt();
	void die();
	void block();
	void block_idle();

private:
	void setup_animations();
	void handle_animation_state();

private:
	Vec2f pos;
	Rect collider;
	// TODO(Pramish): Implement basic jumping and movement physics using them i.e. eqns of motion
	// eqns of motion? why?
	Vec2f vel;
	Vec2f accn;
	Vec2f vMax;
	Vec2f offsets;
	float gravity;
	float friction;
	AnimatedSprite *sprite;
	types::r32 scale = 3.f;

	//Nisans anim/player state identifiers

	bool idle = true;
	bool running = false;
	bool falling = false;
	bool is_jumping = false;
	bool attacking = false;
	bool blocking = false;
	bool other = false;
};