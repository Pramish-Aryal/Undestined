#include "Food.h"

#include "AnimatedSprite.h"
#include "Camera.h"
#include "Graphics.h"
#include "Map.h"
#include "Random.h"
#include "utils.h"

using namespace types;

Food::Food(Graphics &graphics, r32 scale) {
  sprite = new AnimatedSprite(graphics, "data\\Food.png");
  setup_animations();
  this->scale = scale;
  sprite->play_animation("Idle");
  pos = {35 * 32, 12 * 32};
  possible_spawn_points = {{35 * 32, 12 * 32}, {35 * 32, 7 * 32}, {11 * 32, 14 * 32}, {61 * 32, 15 * 32}, {22 * 32, 27 * 32}, {49 * 32, 27 * 32}, {35 * 32, 12 * 32}};
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
  pos = possible_spawn_points[Random::get_random(0, possible_spawn_points.size() - 1)];
  collider.pos = pos;
  inactiveTime = 0;
}

Food::~Food() {
  delete sprite;
}