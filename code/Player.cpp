
#include "Player.h"
#include "Graphics.h"
#include "AnimatedSprite.h"

using namespace types;

Player::Player(Graphics &graphics)
{
	sprite = new AnimatedSprite(graphics, "data\\HeroKnight.png");
	setup_animations();
	pos = {0, 400};
	sprite->play_animation("idle");
}

void Player::draw(Graphics &graphics)
{
	sprite->draw(graphics, (i32)pos.x, (i32)pos.y, scale);
}

void Player::update(r32 dt)
{
	sprite->update(dt);
}

void simulate(types::r32 dt)
{
}

void Player::setup_animations()
{
	sprite->add_animation("idle", 0, 0, 100, 55, 8, 7);
	sprite->add_animation("run", 8, 0, 100, 55, 10, 7);
	sprite->add_animation("attack", 8, 1, 100, 55, 6, 10);
}

void Player::move_left()
{
	pos.x -= 5;
	sprite->set_flip(true);
	sprite->play_animation("run");
}

void Player::move_right()
{
	pos.x += 5;
	sprite->set_flip(false);
	sprite->play_animation("run");
}

void Player::stop_moving()
{
	sprite->play_animation("idle");
}

void Player::attack()
{
	sprite->play_animation("attack");
}

void Player::jump()
{
}

Player::~Player()
{
	delete sprite;
}