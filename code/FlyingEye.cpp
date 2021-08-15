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
const r32 HOVER_DISTANCE = 250.0f;

}  // namespace

FlyingEye::FlyingEye(Graphics &graphics, Vec2f posi) {
  sprite = new AnimatedSprite(graphics, "data\\FlyingEye.png");
  setup_animations();
  pos = posi;
  hoverPos = posi;
  hoverPos.x += 200;
  vel = {0, 0};
  accn = {0, 0};
  vMax = {.15, .15};

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

  if (!attacking) {
    if (ABS((player.get_pos() - hoverPos).x) >= 150) {
      if ((player.get_pos() - 95 - hoverPos).normal().x > 0)
        move_right();
      else if ((player.get_pos() - 95 - hoverPos).normal().x < 0)
        move_left();
    }

    if (((player.get_pos() - hoverPos).y) >= 175) {
      if ((player.get_pos() - hoverPos).normal().y > 0)
        move_down();
    }
    if (((player.get_pos() - hoverPos).y) < -25) {
      if ((player.get_pos() - hoverPos).normal().y < 0)
        move_up();
    }

    if ((player.get_pos() - pos).normal().x > 0)
      sprite->set_flip(false);
    else if ((player.get_pos() - pos).normal().x < 0)
      sprite->set_flip(true);

  } else {
    //---------------------Bat swing down physics------------
    if (buffer) {  //buffering target
      playerBufferedPos = player.get_pos();
      buffer = false;
      if ((playerBufferedPos - pos).normal().x > 0) {
        sprite->set_flip(false);
        dirx = 1;
      } else if ((playerBufferedPos - pos).normal().x < 0) {
        sprite->set_flip(true);
        dirx = -1;
      }
      endAttackTargetPos = playerBufferedPos;
      endAttackTargetPos.x += 500 * dirx;
    }
    if (dirx > 0)
      move_right();
    if (dirx < 0)
      move_left();

    if (((playerBufferedPos - pos).y - 70) > 15) {
      move_down();
    } else if (((playerBufferedPos - pos).y - 70) < 15) {
      move_up();
    }
    if (((dirx * (playerBufferedPos.x - pos.x))+75 ) < 0 && !midway) {
      playerBufferedPos.y -= 150;
      midway = true;
    }
    hoverPos = pos;
  }

  vel += accn * dt;

  // friction
  if (vel.x != 0) {
    dirX = SIGNOF(vel.x);
    float friction = (abs(.0012f * dt) <= abs(vel.x)) ? abs(.0012f * dt) : abs(vel.x);
    vel.x -= dirX * friction;
  }
  if (vel.y != 0) {
    dirX = SIGNOF(vel.y);
    float friction = (abs(.0012f * dt) <= abs(vel.y)) ? abs(.0012f * dt) : abs(vel.y);
    vel.y -= dirX * friction;
  }

  //clampers
  vel.x = (ABS(vel.x) < vMax.x) ? vel.x : (vMax.x * SIGNOF(vel.x));
  vel.y = (ABS(vel.y) < vMax.y) ? vel.y : (vMax.y * SIGNOF(vel.y));

  //pos update

  if (!dead) {
    if (!attacking)
      hoverPos += vel * dt;
    else
      pos += vel * dt;

    flight_angle += ANGULAR_VELOCITY * dt;
    if (!attacking) {
      pos.y = hoverPos.y + (65.5f * sinf(flight_angle * 3.141592f / 180.0f));
      pos.x = hoverPos.x + (15.5f * sinf((flight_angle - 90) * 3.141592f / 180.0f));
    }
  }

  //just for fun, might need to comment them out (tf is this comment bro)
  collider.size = {45.f * scale, 51.f * scale};
  collider.pos = {pos.x + offsets.x * scale, pos.y + offsets.y * scale};

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
  std::vector<std::string> PossibleStates = {"Idle", "Hurt", "Attack"};
  if (contain(PossibleStates, sprite->current_animation)) {
    if (!dead) {
      accn.x -= 0.003f;
      sprite->set_flip(true);
    }
  }
}

void FlyingEye::move_right() {
  std::vector<std::string> PossibleStates = {"Idle", "Hurt", "Attack"};
  if (contain(PossibleStates, sprite->current_animation)) {
    if (!dead) {
      accn.x += 0.003f;
      sprite->set_flip(false);
    }
  }
}
void FlyingEye::move_up() {
  std::vector<std::string> PossibleStates = {"Idle", "Hurt", "Attack"};
  if (contain(PossibleStates, sprite->current_animation)) {
    if (!dead) {
      accn.y -= 0.003f;
    }
  }
}
void FlyingEye::move_down() {
  std::vector<std::string> PossibleStates = {"Idle", "Hurt", "Attack"};
  if (contain(PossibleStates, sprite->current_animation)) {
    if (!dead) {
      accn.y += 0.003f;
    }
  }
}

void FlyingEye::stop_moving() {
  std::vector<std::string> PossibleStates = {"Run", "Fall", "Attack"};
  if (contain(PossibleStates, sprite->current_animation)) {
    attacking = false;
    buffer = false;
    sprite->play_animation("Idle");
  }
}

void FlyingEye::attack() {
  std::vector<std::string> PossibleStates = {"Idle"};
  if (contain(PossibleStates, sprite->current_animation)) {
    sprite->play_animation("Attack");
    attacking = true;
    buffer = true;
    midway = false;
  }
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
  hoverPos = {700, 200};
  health = 100.0f;
  invincible_timer = 0;
  dead = false;
  hurting = false;
  idle = true;
  vel = {0, 0};
  accn = {0, 0};

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