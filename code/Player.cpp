
#include "Player.h"

#include <algorithm>
#include <utility>
#include <vector>

#include "AnimatedSprite.h"
#include "Graphics.h"
#include "Map.h"

using namespace types;

Player::Player(Graphics &graphics) {
  sprite = new AnimatedSprite(graphics, "data\\HeroKnight.png");
  setup_animations();
  pos = {50, 250};
  sprite->play_animation("idle");
  vel = {0, 0};
  accn = {0, 0};
  vMax = {.3, 9.0f};
  gravity = 0.0085f;
  //player size = 28 x 42, 36 x 13
  offsets = {36.f, 13.f};
  collider.pos = {pos.x + offsets.x, pos.y + offsets.y};
  collider.size = {28.f, 42.f};
}

void Player::draw(Graphics &graphics) {
  SDL_Rect rect = {(i32)collider.pos.x, (i32)collider.pos.y, (i32)collider.size.w, (i32)collider.size.h};

  sprite->draw(graphics, (i32)pos.x, (i32)pos.y, scale);
  SDL_RenderDrawRect(graphics.get_renderer(), &rect);
}

void Player::update(r32 dt) {
  sprite->update(dt);
}

void Player::simulate(types::r32 dt, Map &map) {
  float dirX;

  vel.x += accn.x * dt;
  vel.y += accn.y * dt;

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

  //collider = {{pos.x /*+ offsets.x*/, pos.y /*+ offsets.y*/}, {28.f, 48.f}};
  collider.pos = {pos.x + offsets.x, pos.y + offsets.y};

  Vec2f cp, cn;
  r32 t;

  std::vector<std::pair<int, float>> z;

  for (int i = 0; i < map.bounding_boxes.size(); i++) {
    if (Collider::dynamic_rect_vs_rect(&collider, vel, &map.bounding_boxes[i], cp, cn, t, dt))
      {z.push_back({i, t});
      is_jumping = false;
      }
    //vel += cn * Vec2f(ABS(vel.x), ABS(vel.y))  * ( 1 - t);
  }

  std::sort(z.begin(), z.end(), [](const std::pair<int, float> &a, const std::pair<int, float> &b) { return a.second < b.second; });

  for (auto j : z) {
    if (Collider::dynamic_rect_vs_rect(&collider, vel, &map.bounding_boxes[j.first], cp, cn, t, dt))
      {vel += cn * Vec2f(ABS(vel.x), ABS(vel.y)) * (1 - t);
      }
  }

  //pos update
  pos.x += vel.x * dt;
  pos.y += vel.y * dt;

  // pos.y = (pos.y > 500) ? 500 : pos.y;

  //final setup and anims
  accn.x = 0;
  accn.y = 0;

  if (vel.y < 0)
    fall();
  if (vel.x == 0)
    stop_moving();
}

void Player::setup_animations() {
  sprite->add_animation("Idle", 0, 0, 100, 55, 8, 7);

  sprite->add_animation("Run", 8, 0, 100, 55, 10, 7);

  sprite->add_animation("Attack 1", 8, 1, 100, 55, 6, 10);
  sprite->add_animation("Attack 2", 4, 2, 100, 55, 6, 10);
  sprite->add_animation("Attack 3", 0, 3, 100, 55, 8, 10);

  sprite->add_animation("Jump", 7, 3, 100, 55, 4, 10);
  sprite->add_animation("Fall", 1, 4, 100, 55, 4, 10);

  sprite->add_animation("Hurt", 5, 4, 100, 55, 3, 10);
  sprite->add_animation("Death", 8, 4, 100, 55, 10, 10);

  sprite->add_animation("Block Idle", 8, 5, 100, 55, 8, 10);
  sprite->add_animation("Block", 6, 6, 100, 55, 5, 10);

  sprite->add_animation("Roll", 2, 7, 100, 55, 9, 10);

  /* 	
		sprite->add_animation("Ledge Grab", 0, 8, 100, 55, 5, 10);
		sprite->add_animation("Wall Slide", 8, 8, 100, 55, 3, 10);
	 */
}

// TODO(Pramish): Incorporate these with the acceleration
void Player::move_left() {
  accn.x -= 0.003f;
  sprite->set_flip(true);
  sprite->play_animation("Run");
}

void Player::move_right() {
  accn.x += 0.003f;
  sprite->set_flip(false);
  sprite->play_animation("Run");
}

void Player::stop_moving() {
  sprite->play_animation("Idle");
}

void Player::attack() {
  sprite->play_animation("Attack 1");
}

void Player::jump() {
  if ( !is_jumping){
    vel.y = -1.25f;
    is_jumping = true;
  }

  sprite->play_animation("Jump");
}

void Player::fall() {
  sprite->play_animation("Fall");
}

void Player::roll() {
  sprite->play_animation("Roll");
}

void Player::get_hurt() {
  sprite->play_animation("Hurt");
}

void Player::die() {
  sprite->play_animation("Death");
}

void Player::block() {
  sprite->play_animation("Block");
}

void Player::block_idle() {
  sprite->play_animation("Block Idle");
}

Player::~Player() {
  delete sprite;
}