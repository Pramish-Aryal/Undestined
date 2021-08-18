#include "Food.h"

#include "AnimatedSprite.h"
#include "Camera.h"
#include "Graphics.h"
#include "Map.h"

Food::Food(Graphics &graphics) {
  sprite = new AnimatedSprite(graphics, "data\\Food.png");
  setup_animations();
  sprite->play_animation("Idle");
  Vec2f offsets = {0.f, 0.f};
  pos = { 250 , 250};
  collider.pos = {pos.x + offsets.x * scale, pos.y + offsets.y * scale};
  collider.size = {9.f * scale, 13.f * scale};
}

void Food::simulate(int dt) {
  if (!active) {
    inactiveTime += dt;
    if (inactiveTime >= 15.0f)
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
  inactiveTime = 0;
}

Food::~Food() {
  delete sprite;
}