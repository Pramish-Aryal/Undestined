#include "FlyingEye.h"

#include <algorithm>
#include <cmath>
#include <utility>

#include "AnimatedSprite.h"
#include "Camera.h"
#include "Graphics.h"
#include "Map.h"
#include "Player.h"

using namespace types;

namespace {
const r32 ANGULAR_VELOCITY = 120.0f / 1000.0f;
const r32 RESPAWN_TIME = 560;        // 5 seconds
const r32 INVINCIBLE_TIME = 530.0f;  // 3 frames
}  // namespace

FlyingEye::FlyingEye(Graphics &graphics, Vec2f posi) {
  sprite = new AnimatedSprite(graphics, "data\\FlyingEye.png");
  setup_animations();
  pos = posi;
  vel = {0, 0};
  accn = {0, 0};
  vMax = {.3, 9.0f};

  flight_angle = 0;
  //enemy size = 45 x 51, 60 x 50
  offsets = {60.f, 50.f};
  collider.pos = {pos.x + offsets.x * scale, pos.y + offsets.y * scale};
  collider.size = {45.f * scale, 51.f * scale};
  Vec2f screen_size = {1280.f, 720.f};
  Camera::get_instance().get_pos().x = -1 * (screen_size.x / 3.0f - pos.x);
  Camera::get_instance().get_pos().y = -1 * (screen_size.y * 5.7f / 10.0f - pos.y);
  cameraBuffer = Camera::get_instance().get_pos();
  sprite->play_animation("Idle");
}

void FlyingEye::draw(Graphics &graphics, r32 scale) {
  this->scale = scale;
  sprite->draw(graphics, (i32)pos.x, (i32)pos.y, scale);
}

void FlyingEye::debug_draw(Graphics &graphics, u8 scale) {
  r32 o_x = Camera::get_instance().get_pos().x;
  r32 o_y = Camera::get_instance().get_pos().y;
  SDL_Rect rect = {(i32)(collider.pos.x - o_x), (i32)(collider.pos.y - o_y), (i32)(collider.size.w), (i32)(collider.size.h)};
  SDL_RenderDrawRect(graphics.get_renderer(), &rect);
}

void FlyingEye::update(r32 dt) {
  sprite->update(dt);
}

static bool sort_func_ptr(const std::pair<int, float> &a, const std::pair<int, float> &b) {
  return a.second < b.second;
}

void FlyingEye::simulate(types::r32 dt, Map &map, Player &player) {
  float dirX;

  if (ABS((player.get_pos() - pos).x) >= 10) {
    if ((player.get_pos() - pos).normal().x > 0)
      move_right();
    else if ((player.get_pos() - pos).normal().x < 0)
      move_left();
  } else
    stop_moving();

  //just for fun, might need to comment them out
  collider.size = {45.f * scale, 51.f * scale};
  collider.pos = {pos.x + offsets.x * scale, pos.y + offsets.y * scale};
  vel += accn * dt;

  //fraction
  if (vel.x != 0) {
    dirX = (vel.x / abs(vel.x));
    float friction = (abs(.0012f * dt) <= abs(vel.x)) ? abs(.0012f * dt) : abs(vel.x);
    vel.x -= dirX * friction;
  }

  //clampers
  vel.x = (vel.x < vMax.x) ? vel.x : vMax.x;
  vel.x = (-vel.x < vMax.x) ? vel.x : -vMax.x;

  collider.pos = {pos.x + offsets.x * scale, pos.y + offsets.y * scale};

  //pos update
  if (!dead) {
    pos.x += vel.x * dt;

    flight_angle += ANGULAR_VELOCITY * dt;
    pos.y += (2.5f * sinf(flight_angle * 3.141592f / 180.0f));
  }
  //final Resets
  accn.x = 0;
  accn.y = 0;

  //----------Invincible And Respawn Count-----------
  if (invincible_timer > 0)
    invincible_timer -= dt;
  if (dead) {
    if (invincible_timer <= 0) {
      respawn();
    }
  }
}

void FlyingEye::setup_animations() {
  sprite->add_animation("Idle", 0, 0, 150, 150, 8, 7);
  sprite->add_animation("Hurt", 0, 1, 150, 150, 4, 7);
  sprite->add_animation("Die", 0, 2, 150, 150, 4, 7);
  sprite->add_animation("Attack", 0, 3, 150, 150, 8, 7);
}

void FlyingEye::move_left() {
  if (!dead) {
    accn.x -= 0.003f;
    sprite->set_flip(true);
  }
}

void FlyingEye::move_right() {
  if (!dead) {
    accn.x += 0.003f;
    sprite->set_flip(false);
  }
}

void FlyingEye::stop_moving() {
  idle = true;
}

void FlyingEye::attack() {
  sprite->play_animation("Attack");
}

void FlyingEye::get_hurt(types::r32 dt) {
  if (!dead) {
    if ((invincible_timer <= 0)) {
      sprite->play_animation("Hurt", 1);
      // std::cout << "hurt\n";
      invincible_timer = INVINCIBLE_TIME;
      health -= 45.f;
    }
    if (health <= 0 && !dead) {
      die();
      invincible_timer = RESPAWN_TIME;
    }
  }
}

void FlyingEye::die() {
  dead = true;
  idle = false;
  hurting = false;
  sprite->play_animation("Die", 1);
}

Rect FlyingEye::get_collider() {
  return collider;
}

void FlyingEye::respawn() {
  pos = {700, 200};
  health = 100.0f;
  invincible_timer = 0;
  dead = false;
  hurting = false;
  idle = true;
  vel = {0, 0};
  accn = {0, 0};
  vMax = {.3, 9.0f};
  dead = false;

  //enemy size = 45 x 51, 60 x 50
  offsets = {60.f, 50.f};
  collider.pos = {pos.x + offsets.x * scale, pos.y + offsets.y * scale};
  collider.size = {45.f * scale, 51.f * scale};
  Vec2f screen_size = {1280.f, 720.f};
  sprite->play_animation("Idle");
}

FlyingEye::~FlyingEye() {
  delete sprite;
}