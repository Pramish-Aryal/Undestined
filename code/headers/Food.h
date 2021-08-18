#pragma once

#include <vector>

#include "Collision.h"
#include "utils.h"

class AnimatedSprite;
class Graphics;
class Map;

class Food {
 public:
  Food(Graphics &graphics, types::r32 scale = 3.f);
  ~Food();

  Vec2f get_pos();
  Rect get_collider();
  bool get_status();
  void get_eaten();
  types::r32 scale;

  void draw(Graphics &graphics, types::r32 scale = 3.f);
  void debug_draw(Graphics &graphics, types::u8 scale = 10);
  void setup_animations();
  void simulate(types::r32 dt);

 private:
  void respawn();

 private:
  bool active = true;
  int inactiveTime = 0;
  Vec2f pos;
  Rect collider;
  std::vector<Vec2f> possible_spawn_points;
  AnimatedSprite *sprite;
};
