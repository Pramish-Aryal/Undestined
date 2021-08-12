#include "Skeleton.h"

#include <algorithm>
#include <utility>

#include "AnimatedSprite.h"
#include "Graphics.h"
#include "Map.h"
#include "Camera.h"
#include "Player.h"

using namespace types;

namespace
{
	const r32 RESPAWN_TIME = 5 * 1000.f; // 5 seconds
	const r32 INVINCIBLE_TIME = 25 * 16.f; // 3 frames
}



Skeleton::Skeleton(Graphics &graphics, Vec2f posi)
{
	sprite = new AnimatedSprite(graphics, "data\\Skeleton.png");
	setup_animations();
	pos = posi;
	sprite->play_animation("idle");
	vel = {0, 0};
	accn = {0, 0};
	vMax = {.3, 9.0f};
	
	gravity = 0.0045f;
	//enemy size = 45 x 51, 60 x 50
	offsets = {60.f, 50.f};
	collider.pos = {pos.x + offsets.x * scale, pos.y + offsets.y * scale};
	collider.size = {45.f * scale, 51.f * scale};
	handle_animation_state();
	Vec2f screen_size = {1280.f, 720.f};
}

void Skeleton::draw(Graphics &graphics, r32 scale)
{
	this->scale = scale;
	sprite->draw(graphics, (i32)pos.x, (i32)pos.y, scale);
}

void Skeleton::debug_draw(Graphics &graphics, u8 scale)
{
	r32 o_x = Camera::get_instance().get_pos().x;
	r32 o_y = Camera::get_instance().get_pos().y;
	SDL_Rect rect = {(i32)(collider.pos.x - o_x), (i32)(collider.pos.y - o_y), (i32)(collider.size.w), (i32)(collider.size.h)};
	SDL_RenderDrawRect(graphics.get_renderer(), &rect);
}

void Skeleton::update(r32 dt)
{
	sprite->update(dt);
}

#include <iostream>

static bool sort_func_ptr(const std::pair<int, float> &a, const std::pair<int, float> &b)
{
	return a.second < b.second;
}

void Skeleton::simulate(types::r32 dt, Map &map, Player &player)
{
	if(dead) {
		if(time_to_respawn >= RESPAWN_TIME)
			respawn();
		time_to_respawn += dt;
		return;
	}
	
	
	float dirX;
	
	
	if(ABS((player.get_pos() - pos).x) >= 10)
	{
		if ((player.get_pos() - pos).normal().x > 0)
			move_right();
		else if ((player.get_pos() - pos).normal().x < 0)
			move_left();
	}
	else
		stop_moving();
	
	//just for fun, might need to comment them out
	collider.size = {45.f * scale, 51.f * scale};
	collider.pos = {pos.x + offsets.x * scale, pos.y + offsets.y * scale};
	vel += accn * dt;
	
	//fraction
	if (vel.x != 0)
	{
		dirX = (vel.x / abs(vel.x));
		float friction = (abs(.0012f * dt) <= abs(vel.x)) ? abs(.0012f * dt) : abs(vel.x);
		vel.x -= dirX * friction;
	}
	
	vel.y += gravity * dt;
	
	//clampers
	vel.x = (vel.x < vMax.x) ? vel.x : vMax.x;
	vel.x = (-vel.x < vMax.x) ? vel.x : -vMax.x;
	
	vel.y = (vel.y < vMax.y) ? vel.y : vMax.y;
	vel.y = (-vel.y < vMax.y) ? vel.y : -vMax.y;
	
	collider.pos = {pos.x + offsets.x * scale, pos.y + offsets.y * scale};
	
	Vec2f cp, cn;
	r32 t;
	
	std::vector<std::pair<int, float>> z;
	bool is_on_ground = false;
	
	for (int i = 0; i < map.bounding_boxes.size(); i++)
	{
		if (Collider::dynamic_rect_vs_rect(&collider, vel, &map.bounding_boxes[i], cp, cn, t, dt))
		{
			z.push_back({i, t});
			
			if (cn.y == -1)
				is_on_ground = true;
		}
		//vel += cn * Vec2f(ABS(vel.x), ABS(vel.y))  * ( 1 - t);
	}
	
	//std::sort(z.begin(), z.end(), [](const std::pair<int, float> &a, const std::pair<int, float> &b) { return a.second < b.second; });
	std::sort(z.begin(), z.end(), sort_func_ptr);
	
	for (i32 i = 0; i < z.size(); i++)
	{
		if (Collider::dynamic_rect_vs_rect(&collider, vel, &map.bounding_boxes[z[i].first], cp, cn, t, dt))
		{
			vel += cn * Vec2f(ABS(vel.x), ABS(vel.y)) * (1 - t);
		}
	}
	
	//pos update
	pos += vel * dt;
	
	//pos.y = (pos.y > 500) ? 500 : pos.y;
	
	//final setup and anims
	accn.x = 0;
	accn.y = 0;
}

void Skeleton::setup_animations()
{
	sprite->add_animation("Idle", 0, 0, 150, 150, 4, 7);
	sprite->add_animation("Move", 0, 1, 150, 150, 4, 7);
	sprite->add_animation("Die", 0, 2, 150, 150, 4, 7);
	sprite->add_animation("Hurt", 0, 3, 150, 150, 4, 7);
	sprite->add_animation("Attack", 0, 4, 150, 150, 6, 7);
}

void Skeleton::move_left()
{
	accn.x -= 0.003f;
	sprite->set_flip(true);
	handle_animation_state();
	running = true;
	idle = false;
}

void Skeleton::move_right()
{
	accn.x += 0.003f;
	sprite->set_flip(false);
	handle_animation_state();
	running = true;
	idle = false;
}

void Skeleton::stop_moving()
{
	idle = true;
	running = false;
	handle_animation_state();
}

void Skeleton::attack()
{
	sprite->play_animation("Attack");
	handle_animation_state();
}

void Skeleton::get_hurt()
{
	sprite->play_animation("Hurt");
	hurting = true;
	if(health <= 0){
		die();
	} else{
		if(invincible_timer >= INVINCIBLE_TIME) {
			invincible_timer = 0.0f;
			health -= 25.f;
		} else {
			invincible_timer += 16.f;
		}
	}
	handle_animation_state();
}

void Skeleton::die()
{
	dead = true;
	idle = false;
	hurting = false;
	running = false;
	sprite->play_animation("Die");
	handle_animation_state();
}

void Skeleton::handle_animation_state()
{
	if (hurting)
		sprite->play_animation("Hurt");
	else if(dead)
		sprite->play_animation("Die");
	else if (idle)
		sprite->play_animation("Idle");
	else if (running)
		sprite->play_animation("Move");
	
}

Rect Skeleton::get_collider()
{
	return collider;
}

void Skeleton::respawn()
{
	pos = { 700, 200};
	sprite->play_animation("Idle");
	health = 100.0f;
	time_to_respawn = 0.f;
	invincible_timer = 0;
	dead = false;
	hurting = false;
	running = false;
	idle = true;
	vel = {0, 0};
	accn = {0, 0};
	vMax = {.3, 9.0f};
	dead = false;
	gravity = 0.0045f;
	//enemy size = 45 x 51, 60 x 50
	offsets = {60.f, 50.f};
	collider.pos = {pos.x + offsets.x * scale, pos.y + offsets.y * scale};
	collider.size = {45.f * scale, 51.f * scale};
	handle_animation_state();
	Vec2f screen_size = {1280.f, 720.f};
}

Skeleton::~Skeleton()
{
	delete sprite;
}