#pragma once

#include "Enemy.h"

class Skeleton : public Enemy
{
	public:
	Skeleton(Graphics &graphics, Vec2f pos = {700, 200});
	~Skeleton();
	
	void draw(Graphics &graphics, types::r32 scale = 3.f) override;
	void debug_draw(Graphics &graphics, types::u8 scale = 10) override;
	void simulate(types::r32 dt, Map &map, Player& player) override;
	void update(types::r32 dt) override;
	
	//collision
	Rect get_collider() override;
	
	//player movement
	void move_left() override;
	void move_right() override;
	void stop_moving() override;
	void attack() override;
	void get_hurt(types::r32 dt) override;
	void die() override;
	
	private:
	void setup_animations() override;
	void respawn();
	void endAttack();
	void stop_attack();
	
	private:
	Vec2f pos;
	Rect collider;
	Rect attackCollider;
	
	AnimatedSprite *sprite;
	Vec2f vel;
	Vec2f accn;
	Vec2f vMax;
	Vec2f offsets;
	types::r32 gravity;
	types::r32 friction;
	types::r32 scale = 3.f;
	types::r32 health = 100.f;
	types::r32 invincible_timer = 0.0f;
	types::r32 time_to_respawn = 0.0f;
	
	//Nisans anim/player state identifiers
	bool idle = true;
	bool dead = false;
	bool running = false;
	bool hurting = false;
	bool other = false;
	bool first_attack = false;
	
	
	types::r32 attack_delay;
	bool attackBusy = false;
	bool countTime = false;
	types::i32 attackActiveTime = 0;
};
