#pragma once

#include "Enemy.h"

class FlyingEye : public Enemy {
	public:
	FlyingEye(Graphics &graphics, Vec2f pos = {700, 200});
	~FlyingEye();
	
	void draw(Graphics &graphics, types::r32 scale = 3.f) override;
	void debug_draw(Graphics &graphics, types::u8 scale = 10) override;
	void simulate(types::r32 dt, Map &map, Player &player) override;
	void update(types::r32 dt) override;
	
	//collision
	Rect get_collider() override;
	
	//player movement
	void move_left() override;
	void move_right() override;
	void move_up();
	void move_down();
	
	void stop_moving() override;
	void attack() override;
	void endAttack();
	void get_hurt(types::r32 dt) override;
	void die() override;
	
	private:
	void setup_animations() override;
	void respawn();
	void draw_health(Graphics& graphics);
	
	private:
	Vec2f pos;
	Vec2f hoverPos;
	Vec2f playerBufferedPos;
	Vec2f endAttackTargetPos;
	Vec2f cameraBuffer;
	Rect collider;
	
	AnimatedSprite *sprite;
	Vec2f vel;
	Vec2f accn;
	Vec2f vMax;
	Vec2f offsets;
	types::r32 flight_angle;
	types::r32 friction;
	types::r32 scale = 3.f;
	types::r32 invincible_timer = 0.0f;
	types::r32 health = 100.f;
	
	bool first_attack;
	types::r32 attack_delay = 0.f;
	
	bool idle = true;
	int dirx = 0;
	bool dead = false;
	bool hurting = false;
	bool attacking = false;
	bool buffer = false;
	bool midway = false;
	bool hit = false;
	bool other = false;
};
