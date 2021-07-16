
#include "Player.h"

#include "AnimatedSprite.h"
#include "Graphics.h"

using namespace types;

Player::Player(Graphics &graphics) {
  sprite = new AnimatedSprite(graphics, "data\\HeroKnight.png");
  setup_animations();
  pos = {50, 400};
  sprite->play_animation("idle");
  vel = {0, 0};
  accn = {0, 0};
  vMax = {.3, 9.0f};
  gravity = 0.009f;
}

void Player::draw(Graphics &graphics) {
  sprite->draw(graphics, (i32)pos.x, (i32)pos.y, scale);
}

void Player::update(r32 dt) {
  sprite->update(dt);
}

void Player::simulate(types::r32 dt) {
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

  //pos update
  pos.x += vel.x * dt;
  pos.y += vel.y * dt;

  pos.y = (pos.y > 500) ? 500 : pos.y;
  if (pos.y == 500 && is_jumping) {
    is_jumping = false;
  }

  //final setup and anims
  accn.x = 0;
  accn.y = 0;

  if (vel.y < 0)
    fall();
  if (vel.x == 0)
    stop_moving();
  return;
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
  if (!is_jumping) {
    accn.y -= 0.9f;
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