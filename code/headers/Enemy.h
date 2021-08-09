#pragma once

#include "utils.h"
#include "Collision.h"

class AnimatedSprite;
class Graphics;
class Map;

class Enemy
{
	public:
	Enemy(Graphics &graphics);
	~Enemy();
	
	void draw(Graphics &graphics, types::r32 scale = 3.f);
	void debug_draw(Graphics& graphics, types::u8 scale = 10);
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
	void wall_slide();
	
	private:
	void setup_animations();
	void handle_animation_state();
	
	private:
	Vec2f pos;
	Vec2f cameraBuffer;
	Rect collider;
	
	AnimatedSprite *sprite;
	Vec2f vel;
	Vec2f accn;
	Vec2f vMax;
	Vec2f offsets;
	float gravity;
	float friction;
	types::r32 scale = 3.f;
	
	//Nisans anim/player state identifiers
	bool idle = true;
	bool running = false;
	bool falling = false;
	bool attacking = false;
	bool other = false;
};