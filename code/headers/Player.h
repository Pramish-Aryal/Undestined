#pragma once

#include "utils.h"
#include "Collision.h"
#include <vector>

class AnimatedSprite;
class Graphics;
class Map;
class Enemy;

class Player
{
	public:
	Player(Graphics &graphics);
	~Player();
	
	void draw(Graphics &graphics, types::r32 scale = 3.f);
	void debug_draw(Graphics &graphics, types::u8 scale = 10);
	void simulate(types::r32 dt, Map &map, std::vector<Enemy *> &enemylist);
	void update(types::r32 dt);
	
	Vec2f get_pos();
	Rect get_collider();
	//player movement
	void move_left();
	void move_right();
	void stop_moving();
	void stop_falling();
	void attack();
	void endAttack();
	void setupCombo();
	void jump();
	void fall();
	void roll();
	void get_hurt(types::r32 dt);
	void die();
	void block();
	void block_idle();
	
	private:
	void setup_animations();
	
	private:
	Vec2f pos;
	Vec2f cameraBuffer;
	Rect collider;
	Rect attackCollider;
	
	AnimatedSprite *sprite;
	Vec2f vel;
	Vec2f accn;
	Vec2f vMax;
	Vec2f offsets;
	float gravity;
	float friction;
	types::r32 scale = 3.f;
	types::r32 jump_timer;
	types::r32 health = 100.f;
	types::r32 invincible_timer = 0.0f;
	
	bool cameraMovingx = false;
	bool cameraMovingy = false;
	//Nisans anim/player state identifiers
	
	bool falling = false;
	bool is_jumping = false;
	bool blocking = false;
	bool dead = false;
	bool hurting = false;
	bool other = false;
	
	bool attackBusy = false;
	bool comboReady = false;
	bool countTime = false;
	types::i32 attackState = 0;
	types::i32 attackActiveTime = 0;
};