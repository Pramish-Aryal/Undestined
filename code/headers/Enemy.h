#pragma once

#include "utils.h"
#include "Collision.h"
#include <vector>

class AnimatedSprite;
class Graphics;
class Map;
class Player;

class Enemy
{
	public:
	virtual void draw(Graphics &, types::r32) = 0;
	virtual void debug_draw(Graphics &graphics, types::u8 scale = 10) = 0;
	virtual void simulate(types::r32 dt, Map &map, Player& player) = 0;
	virtual void update(types::r32 dt) = 0;
	virtual void set_spawn_points(std::vector<Vec2f>& spawns) = 0;
	//collision
	virtual Rect get_collider() = 0;
	
	//player movement
	virtual void move_left() = 0;
	virtual void move_right() = 0;
	virtual void stop_moving() = 0;
	virtual void attack() = 0;
	virtual void get_hurt(types::r32 dt) = 0;
	virtual void die() = 0;
	
	static types::u32 score;
	static types::u32 get_score() { return score;}
	private:
	virtual void setup_animations() = 0;
};
