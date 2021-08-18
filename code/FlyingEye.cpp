#include "FlyingEye.h"

#include <algorithm>
#include <cmath>
#include <utility>

#include "AnimatedSprite.h"
#include "Camera.h"
#include "Graphics.h"
#include "Map.h"
#include "Player.h"
#include "Random.h"

using namespace types;

namespace {
const r32 ANGULAR_VELOCITY = 120.0f / 1000.0f;
const r32 RESPAWN_TIME = 560;        // 5 seconds
const r32 INVINCIBLE_TIME = 530.0f;  // 3 frames
const r32 HOVER_DISTANCE = 250.0f;
const r32 ATTACK_DELAY = (1.9f * 1000.f);  //1.75 seconds
}  // namespace

FlyingEye::FlyingEye(Graphics &graphics) {
  sprite = new AnimatedSprite(graphics, "data\\FlyingEye.png");
  setup_animations();
  possible_spawn_points = {{100, 200}, {900, 100}, {400, 300}, {300, 200}};
  pos = possible_spawn_points[Random::get_random(0, possible_spawn_points.size() - 1)];
  hoverPos = pos;
  hoverPos.x += 200;
  vel = {0, 0};
  accn = {0, 0};
  vMax = {
      Random::get_random(0, 100) / 500.f,
      .3};

  flight_angle = 0;
  //enemy size = 45 x 51, 60 x 50
  offsets = {55.f, 50.f};
  collider.pos = {pos.x + offsets.x * scale, pos.y + offsets.y * scale};
  collider.size = {43.f * scale, 45.f * scale};
  sprite->play_animation("Idle");
}

void FlyingEye::draw(Graphics &graphics, r32 scale) {
  this->scale = scale;
  sprite->draw(graphics, (i32)pos.x, (i32)pos.y, scale);
  draw_health(graphics);
}

void FlyingEye::debug_draw(Graphics &graphics, u8 scale) {
  r32 o_x = Camera::get_instance().get_pos().x;
  r32 o_y = Camera::get_instance().get_pos().y;
  SDL_Rect rect = {(i32)(collider.pos.x - o_x), (i32)(collider.pos.y - o_y), (i32)(collider.size.w), (i32)(collider.size.h)};
  SDL_RenderDrawRect(graphics.get_renderer(), &rect);
}

void FlyingEye::draw_health(Graphics &graphics) {
  r32 o_x = Camera::get_instance().get_pos().x;
  r32 o_y = Camera::get_instance().get_pos().y;

  SDL_Rect rect = {(i32)(collider.pos.x - o_x - 10), (i32)(collider.pos.y - o_y), 50, 5};
  SDL_SetRenderDrawColor(graphics.get_renderer(), 18, 18, 18, 255);
  SDL_RenderDrawRect(graphics.get_renderer(), &rect);

  i32 health_width = (health / 100.f) > 0 ? 50.f * (health / 100.f) : 0;
  rect = {(i32)(collider.pos.x - o_x - 10), (i32)(collider.pos.y - o_y), health_width, 5};
  SDL_SetRenderDrawColor(graphics.get_renderer(), 200, 18, 18, 255);
  SDL_RenderFillRect(graphics.get_renderer(), &rect);
}

void FlyingEye::update(r32 dt) {
  sprite->update(dt);
}

static bool sort_func_ptr(const std::pair<int, float> &a, const std::pair<int, float> &b) {
  return a.second < b.second;
}

void FlyingEye::simulate(types::r32 dt, Map &map, Player &player) {
  float dirX;

  Vec2f distance = player.get_collider().pos + player.get_collider().size / 2 - collider.pos - collider.size / 2;

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

    if (((playerBufferedPos - pos).y - 60) > 5) {
      move_down();
    } else if (((playerBufferedPos - pos).y - 60) < -5) {
      move_up();
    }

    if (((dirx * (playerBufferedPos.x - pos.x)) + 75) < 0 && !midway) {
      playerBufferedPos.y -= 150;
      midway = true;
    }

    if (midway && (playerBufferedPos.y + 10) > pos.y)
      endAttack();
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

  //------Attack Trigger-----------

  if (ABS(distance.x) <= 200 && ABS(distance.y) <= 150) {
    if (attack_delay >= ATTACK_DELAY) {
      attack_delay = 0;
      attack();
    }
  }

  attack_delay += dt;

  //-------Damage the player
  if (!hit && attacking) {
    if (Collider::rect_vs_rect(this->collider, player.get_collider())) {
      player.get_hurt(dt);
      hit = true;
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
  // std::vector<std::string> PossibleStates = {};
  // if (contain(PossibleStates, sprite->current_animation)) {
  sprite->play_animation("Idle");
  // }
}

void FlyingEye::endAttack() {
  std::vector<std::string> PossibleStates = {"Idle", "Attack", "Hurt"};
  if (contain(PossibleStates, sprite->current_animation)) {
    hoverPos = pos;
    hoverPos.y -= 65.5f;
    attacking = false;
    buffer = false;
    hit = true;
    midway = true;
    flight_angle = 90;
    attack_delay = 0;
    sprite->play_animation("Idle");
  }
}
void FlyingEye::attack() {
  std::vector<std::string> PossibleStates = {"Idle"};
  if (contain(PossibleStates, sprite->current_animation)) {
    sprite->play_animation("Attack");
    attacking = true;
    buffer = true;
    hit = false;
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
    if (attacking || !hit) {
      endAttack();
      move_up();
      sprite->play_animation("Hurt", 1);
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
  score++;

  pos = possible_spawn_points[Random::get_random(0, possible_spawn_points.size() - 1)];
  ;
  hoverPos = pos;
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
  sprite->play_animation("Idle");
}

void FlyingEye::set_spawn_points(std::vector<Vec2f> &spawns) {
  possible_spawn_points = spawns;
}

FlyingEye::~FlyingEye() {
  delete sprite;
}
