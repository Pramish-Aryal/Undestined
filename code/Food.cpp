#include "Food.h"

#include "AnimatedSprite.h"
#include "Camera.h"
#include "Graphics.h"
#include "Map.h"
#include "utils.h"
#include "Random.h"

using namespace types;

Food::Food(Graphics &graphics, r32 scale) {
	sprite = new AnimatedSprite(graphics, "data\\Food.png");
	setup_animations();
	this->scale = scale;
	sprite->play_animation("Idle");
	pos = { 250 , 225};
	collider.pos = pos;
	collider.size = {9.f * scale, 13.f * scale};
}

void Food::simulate(r32 dt) {
	if (!active) {
		inactiveTime += dt;
		if (inactiveTime >= 15000.0f)
			respawn();
	}
	
}

void Food::setup_animations() {
	sprite->add_animation("Idle", 0, 0, 9, 13, 4, 4);
}

void Food::draw(Graphics &graphics, r32 scale) {
	if (active) {
		this->scale = scale;
		sprite->draw(graphics, (i32)pos.x, (i32)pos.y, scale);
	}
}

void Food::get_eaten() {
	active = false;
	inactiveTime = 0;
}

bool Food::get_status() {
	return active;
}

Rect Food::get_collider() {
	return collider;
}

void Food::respawn() {
	active = true;
	/* 
		pos = {
			(r32)Random::get_random(0, 1280),
			(r32)Random::get_random(0, 720)
		};
	 */
	collider.pos = pos;
	inactiveTime = 0;
}

Food::~Food() {
	delete sprite;
}