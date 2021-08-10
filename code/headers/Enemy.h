#pragma once

#include "utils.h"
#include "Collision.h"
#include <vector>

class AnimatedSprite;
class Graphics;
class Map;

class Enemy
{
	public:
	Enemy(Graphics &graphics, Vec2f pos = {700, 200} );
	~Enemy();
	
	void draw(Graphics &graphics, types::r32 scale = 3.f);
	void debug_draw(Graphics& graphics, types::u8 scale = 10);
	void simulate(types::r32 dt, Map &map);
	void update(types::r32 dt);
	
	//collision
	Rect get_collider();
	
	//player movement
	void move_left();
	void move_right();
	void stop_moving();
	void attack();
	void get_hurt();
	void die();
	
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
	bool attacking = false;
	bool hurting = false;
	bool other = false;
};