#pragma once

#include "Enemy.h"

class Skeleton : public Enemy {
 public:
  Skeleton(Graphics &graphics);
  ~Skeleton();

  void draw(Graphics &graphics, types::r32 scale = 3.f) override;
  void debug_draw(Graphics &graphics, types::u8 scale = 10) override;
  void simulate(types::r32 dt, Map &map, Player &player) override;
  void update(types::r32 dt) override;

  void set_spawn_points(std::vector<Vec2f> &spawns) override;

  //collision
  Rect get_collider() override;

  //player movement
  void move_left() override;
  void move_right() override;
  void stop_moving() override;
  void attack() override;
  void get_hurt(types::r32 dt) override;
  void die() override;

 private:
  void setup_animations() override;
  void respawn();
  void endAttack();
  void stop_attack();
  void draw_health(Graphics &graphics);

 private:
  Vec2f pos;
  Rect collider;
  Rect attackCollider;
  std::vector<Vec2f> possible_spawn_points;
  AnimatedSprite *sprite;
  Vec2f vel;
  Vec2f accn;
  Vec2f vMax;
  Vec2f offsets;
  types::r32 gravity = 0.0045f;
  types::r32 friction;
  types::r32 scale = 3.f;
  types::r32 health = 100.f;
  types::r32 invincible_timer = 0.0f;
  types::r32 time_to_respawn = 0.0f;

  //Nisans anim/player state identifiers
  bool dead = false;
  bool hurting = false;
  bool other = false;
  bool first_attack = false;

  types::r32 attack_delay;
  bool attackBusy = false;
  bool countTime = false;
  types::i32 attackActiveTime = 0;
};
