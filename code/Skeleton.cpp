#include "Skeleton.h"

#include <algorithm>
#include <utility>

#include "AnimatedSprite.h"
#include "Camera.h"
#include "Graphics.h"
#include "Map.h"
#include "Player.h"

using namespace types;

namespace {
	const r32 RESPAWN_TIME = 560;        // 5 seconds
	const r32 INVINCIBLE_TIME = 530.0f;  // 3 frames
	const r32 ATTACK_DELAY = 3 * 1000.f; //3 seconds
}  // namespace

Skeleton::Skeleton(Graphics &graphics, Vec2f posi) {
	sprite = new AnimatedSprite(graphics, "data\\Skeleton.png");
	setup_animations();
	pos = posi;
	
	vel = {0, 0};
	accn = {0, 0};
	vMax = {.3, 9.0f};
	
	gravity = 0.0045f;
	//enemy size = 45 x 51, 60 x 50
	offsets = {60.f, 50.f};
	collider.pos = {pos.x + offsets.x * scale, pos.y + offsets.y * scale};
	collider.size = {45.f * scale, 51.f * scale};
	
	attackCollider.pos = {pos.x + offsets.x * scale + 45 * 1.5f, pos.y + offsets.y * scale + 3};
	attackCollider.size = {30.f * scale, 35.f * scale};
	
	Vec2f screen_size = {1280.f, 720.f};
	sprite->play_animation("Idle");
}

void Skeleton::draw(Graphics &graphics, r32 scale) {
	this->scale = scale;
	sprite->draw(graphics, (i32)pos.x, (i32)pos.y, scale);
}

void Skeleton::debug_draw(Graphics &graphics, u8 scale) {
	r32 o_x = Camera::get_instance().get_pos().x;
	r32 o_y = Camera::get_instance().get_pos().y;
	SDL_Rect rect = {(i32)(collider.pos.x - o_x), (i32)(collider.pos.y - o_y), (i32)(collider.size.w), (i32)(collider.size.h)};
	SDL_RenderDrawRect(graphics.get_renderer(), &rect);
	if (attackBusy && attackActiveTime > 555 ) {
		rect = {(i32)(attackCollider.pos.x - o_x), (i32)(attackCollider.pos.y - o_y), (i32)(attackCollider.size.w), (i32)(attackCollider.size.h)};
		SDL_RenderDrawRect(graphics.get_renderer(), &rect);
	}
}

void Skeleton::update(r32 dt) {
	sprite->update(dt);
}

static bool sort_func_ptr(const std::pair<int, float> &a, const std::pair<int, float> &b) {
	return a.second < b.second;
}

#include <iostream>

void Skeleton::simulate(types::r32 dt, Map &map, Player &player) {
	float dirX;
	
	Vec2f distance = player.get_collider().pos + player.get_collider().size / 2 - collider.pos - collider.size / 2;
	
	//r32 limit = sprite->get_flip() ? 
	
	if (ABS(distance.x) <= 250 && ABS(distance.x) >= 100 && ABS(distance.y) <= 250) {
		if (distance.normal().x > 0)
			move_right();
		else if (distance.normal().x < 0)
			move_left();
	} else
		stop_moving();
	
	sprite->set_flip( distance.x < 0 ? true : false);
	
	vel += accn * dt;
	
	//fraction
	if (vel.x != 0) {
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
	
	Vec2f cp, cn;
	r32 t;
	
	std::vector<std::pair<int, float>> z;
	bool is_on_ground = false;
	
	for (int i = 0; i < map.bounding_boxes.size(); i++) {
		if (Collider::dynamic_rect_vs_rect(&collider, vel, &map.bounding_boxes[i], cp, cn, t, dt)) {
			z.push_back({i, t});
			
			if (cn.y == -1)
				is_on_ground = true;
		}
		//vel += cn * Vec2f(ABS(vel.x), ABS(vel.y))  * ( 1 - t);
	}
	
	//std::sort(z.begin(), z.end(), [](const std::pair<int, float> &a, const std::pair<int, float> &b) { return a.second < b.second; });
	std::sort(z.begin(), z.end(), sort_func_ptr);
	
	for (i32 i = 0; i < z.size(); i++) {
		if (Collider::dynamic_rect_vs_rect(&collider, vel, &map.bounding_boxes[z[i].first], cp, cn, t, dt)) {
			vel += cn * Vec2f(ABS(vel.x), ABS(vel.y)) * (1 - t);
		}
	}
	
	//pos update
	pos += vel * dt;
	
	//final Resets
	accn.x = 0;
	accn.y = 0;
	
	collider.size = {45.f * scale, 51.f * scale};
	collider.pos = {pos.x + offsets.x * scale, pos.y + offsets.y * scale};
	if (!sprite->get_flip()) {
		attackCollider.size = {30.f * scale, 35.f * scale};
		attackCollider.pos = {pos.x + offsets.x * scale + 45 * 1.5f, pos.y + offsets.y * scale + 3};
	} else {
		attackCollider.size = {30.f * scale, 35.f * scale};
		attackCollider.pos = {pos.x + offsets.x * scale - attackCollider.size.x, pos.y + offsets.y * scale + 3};
	}
	
	std::cout << distance.y << std::endl;
	if(ABS(distance.x) < 105 && ABS(distance.y) <= 10) {
		attack();
	}
	
	if (countTime)
		attackActiveTime += dt;
	
	if (attackActiveTime > 725) {
		endAttack();
	}
	
	//--------TODO: Attack  ----------
	
	if (attackBusy) {
		if (Collider::rect_vs_rect(this->attackCollider, player.get_collider())) {
			if (attackActiveTime > 555)
				player.get_hurt(dt);
		}
	}
	
	//----------Invincible And Respawn Count-----------
	if (invincible_timer > 0)
		invincible_timer -= dt;
	if (dead) {
		if (invincible_timer <= 0) {
			respawn();
		}
	}
}

void Skeleton::setup_animations() {
	sprite->add_animation("Idle", 0, 0, 150, 150, 4, 7);
	sprite->add_animation("Run", 0, 1, 150, 150, 4, 7);
	sprite->add_animation("Die", 0, 2, 150, 150, 4, 7);
	sprite->add_animation("Hurt", 0, 3, 150, 150, 4, 11);
	sprite->add_animation("Attack", 0, 4, 150, 150, 8, 11);
}

void Skeleton::move_left() {
	std::vector<std::string> PossibleStates = {"Idle", "Run", "Jump", "Fall", "Hurt"};
	if (contain(PossibleStates, sprite->current_animation)) {
		accn.x -= 0.003f;
		sprite->set_flip(true);
	}
	PossibleStates = {"Idle"};
	if (contain(PossibleStates, sprite->current_animation)) {
		sprite->play_animation("Run");
	}
}

void Skeleton::move_right() {
	std::vector<std::string> PossibleStates = {"Idle", "Run", "Jump", "Hurt"};
	if (contain(PossibleStates, sprite->current_animation)) {
		accn.x += 0.003f;
		sprite->set_flip(false);
	}
	PossibleStates = {"Idle"};
	if (contain(PossibleStates, sprite->current_animation)) {
		sprite->play_animation("Run");
	}
}

void Skeleton::stop_moving() {
	std::vector<std::string> PossibleStates = {"Run", "Fall"};
	if (contain(PossibleStates, sprite->current_animation)) {
		sprite->play_animation("Idle");
	}
}

void Skeleton::attack() {
	std::vector<std::string> PossibleStates = {"Idle", "Run"};
	if (contain(PossibleStates, sprite->current_animation)) {
		if (!attackBusy) {
			attackBusy = true;
			countTime = true;
			sprite->play_animation("Attack", 1);
		}
	}
}

void Skeleton::endAttack() {
	attackBusy = false;
	countTime = false;
	attackActiveTime = 0;
	sprite->play_animation("Idle");
}

void Skeleton::get_hurt(r32 dt) {
	if (!dead) {
		if ((invincible_timer <= 0)) {
			sprite->play_animation("Hurt", 1);
			invincible_timer = INVINCIBLE_TIME;
			health -= 45.f;
		}
		if (health <= 0 && !dead) {
			die();
			invincible_timer = RESPAWN_TIME;
		}
	}
}

void Skeleton::die() {
	dead = true;
	idle = false;
	hurting = false;
	running = false;
	sprite->play_animation("Die", 1);
}

Rect Skeleton::get_collider() {
	return collider;
}

void Skeleton::respawn() {
	pos = {700, 200};
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
	Vec2f screen_size = {1280.f, 720.f};
	sprite->play_animation("Idle");
}

Skeleton::~Skeleton() {
	delete sprite;
}