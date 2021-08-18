#pragma once

#include <vector>

#include "Collision.h"
#include "utils.h"

class AnimatedSprite;
class Graphics;
class Map;

class Food {
 public:
  Food(Graphics &graphics);
  ~Food();

  Vec2f get_pos();
  Rect get_collider();
  bool get_status();
  void get_eaten();
  int scale;

 private:
  void draw(Graphics &graphics, types::r32 scale = 3.f);
  void debug_draw(Graphics &graphics, types::u8 scale = 10);
  void setup_animations();
  void respawn();
  void simulate(int dt);

 private:
  bool active = true;
  int inactiveTime = 0;
  Vec2f pos;
  Rect collider;
  AnimatedSprite *sprite;
};
