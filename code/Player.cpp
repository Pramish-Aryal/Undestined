
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
void Player::move_left()
{
	pos.x -= 5;
	sprite->set_flip(true);
	sprite->play_animation("Run");
}

void Player::move_right()
{
	pos.x += 5;
	sprite->set_flip(false);
	sprite->play_animation("Run");
}

void Player::stop_moving()
{
	sprite->play_animation("Idle");
}

void Player::attack()
{
	sprite->play_animation("Attack 1");
}

void Player::jump()
{
	sprite->play_animation("Jump");
}

void Player::fall()
{
	sprite->play_animation("Fall");
}

void Player::roll()
{
	sprite->play_animation("Roll");
}

void Player::get_hurt()
{
	sprite->play_animation("Hurt");
}

void Player::die()
{
	sprite->play_animation("Death");
}

void Player::block()
{
	sprite->play_animation("Block");
}

void Player::block_idle()
{
	sprite->play_animation("Block Idle");
}


Player::~Player()
{
	delete sprite;
}