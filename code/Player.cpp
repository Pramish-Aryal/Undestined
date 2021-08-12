#include "Player.h"

#include <algorithm>
#include <utility>

#include "AnimatedSprite.h"
#include "Camera.h"
#include "Enemy.h"
#include "Graphics.h"
#include "Map.h"

using namespace types;

namespace
{
  const r32 JUMP_TIMER_MAX = 575.f;
}

Player::Player(Graphics &graphics)
{
  sprite = new AnimatedSprite(graphics, "data\\HeroKnight.png");
  setup_animations();
  pos = {36 * 16 * 2, 10 * 16 * 2};
  sprite->play_animation("idle");
  vel = {0, 0};
  accn = {0, 0};
  vMax = {.3, 9.0f};

  gravity = 0.0045f;
  //player size = 28 x 42, 36 x 13
  offsets = {36.f, 13.f};

  collider.pos = {pos.x + offsets.x * scale, pos.y + offsets.y * scale};
  collider.size = {28.f * scale, 41.f * scale};

  attackCollider.pos = {pos.x + offsets.x * scale + 28 * 1.5f, pos.y + offsets.y * scale + 3};
  attackCollider.size = {30.f * scale, 35.f * scale};

  jump_timer = JUMP_TIMER_MAX;

  handle_animation_state();

  Vec2f screen_size = {1280.f, 720.f};
  Camera::get_instance().get_pos().x = -1 * (screen_size.x / 3.0f - pos.x);
  Camera::get_instance().get_pos().y = -1 * (screen_size.y * 5.7f / 10.0f - pos.y);
  cameraBuffer = Camera::get_instance().get_pos();
}

void Player::draw(Graphics &graphics, r32 scale)
{
  this->scale = scale;
  sprite->draw(graphics, (i32)pos.x, (i32)pos.y, scale);
}

void Player::debug_draw(Graphics &graphics, u8 scale)
{
  r32 o_x = Camera::get_instance().get_pos().x;
  r32 o_y = Camera::get_instance().get_pos().y;
  SDL_Rect rect = {(i32)(collider.pos.x - o_x), (i32)(collider.pos.y - o_y), (i32)(collider.size.w), (i32)(collider.size.h)};
  SDL_RenderDrawRect(graphics.get_renderer(), &rect);
  if (attackBusy)
    rect = {(i32)(attackCollider.pos.x - o_x), (i32)(attackCollider.pos.y - o_y), (i32)(attackCollider.size.w), (i32)(attackCollider.size.h)};
  SDL_RenderDrawRect(graphics.get_renderer(), &rect);
}

void Player::update(r32 dt)
{
  sprite->update(dt);
}

bool sort_func_ptr(const std::pair<int, float> &a, const std::pair<int, float> &b)
{
  return a.second < b.second;
}

void Player::simulate(types::r32 dt, Map &map, std::vector<Enemy *> &enemylist)
{
  float dirX;

  //---------- Attack Handling ---------
  if (countTime)
    attackActiveTime += dt;

  if (attackActiveTime > 700)
  {
    endAttack();
  }

  if (attackActiveTime > 400)
  {
    comboReady = true;
    attackBusy = false;
  }

  //------------ Actual Physics ------------
  vel += accn * dt;

  //-------------friction------------
  if (vel.x != 0)
  {
    dirX = SIGNOF(vel.x);
    float friction = (abs(.0012f * dt) <= abs(vel.x)) ? abs(.0012f * dt) : abs(vel.x);
    vel.x -= dirX * friction;
  }

  vel.y += gravity * dt; // gravity ofc

  //--------velocity clampers-------------
  vel.x = (vel.x < vMax.x) ? vel.x : vMax.x;
  vel.x = (-vel.x < vMax.x) ? vel.x : -vMax.x;
  vel.y = (vel.y < vMax.y) ? vel.y : vMax.y;
  vel.y = (-vel.y < vMax.y) ? vel.y : -vMax.y;

  //----------- Ground check for jumps? (@pramish tell me what the following code does )---------------
  Vec2f cp, cn;
  r32 t;

  std::vector<std::pair<int, float>> z;
  bool is_on_ground = false;

  for (int i = 0; i < map.bounding_boxes.size(); i++)
  {
    if (Collider::dynamic_rect_vs_rect(&collider, vel, &map.bounding_boxes[i], cp, cn, t, dt))
    {
      z.push_back({i, t});

      if (cn.y <= 0)
        is_jumping = false;

      if (cn.y == -1)
        is_on_ground = true;
    }
    //vel += cn * Vec2f(ABS(vel.x), ABS(vel.y))  * ( 1 - t);
  }

  jump_timer += dt;

  //std::sort(z.begin(), z.end(), [](const std::pair<int, float> &a, const std::pair<int, float> &b) { return a.second < b.second; });
  std::sort(z.begin(), z.end(), sort_func_ptr);

  for (i32 i = 0; i < z.size(); i++)
  {
    if (Collider::dynamic_rect_vs_rect(&collider, vel, &map.bounding_boxes[z[i].first], cp, cn, t, dt))
    {
      vel += cn * Vec2f(ABS(vel.x), ABS(vel.y)) * (1 - t);
    }
  }

  //-----------Positon Update (overloading hehe)---------
  pos += vel * dt;

  //---- final resets and anim state handling -----
  accn.x = 0;
  accn.y = 0;

  if (is_jumping && !falling && vel.y >= 0.0f)
    fall();
  if (!is_on_ground && !is_jumping)
    fall();
  if (falling && vel.y == 0)
    stop_falling();
  if (vel.x == 0 && vel.y == 0 && !is_jumping && !falling)
    stop_moving();

  //---------Collider updates-------------
  collider.size = {28.f * scale, 41.f * scale};
  collider.pos = {pos.x + offsets.x * scale, pos.y + offsets.y * scale};
  if (!sprite->get_flip())
  {
    attackCollider.size = {30.f * scale, 35.f * scale};
    attackCollider.pos = {pos.x + offsets.x * scale + 28 * 1.5f, pos.y + offsets.y * scale + 3};
  }
  else
  {
    attackCollider.size = {30.f * scale, 35.f * scale};
    attackCollider.pos = {pos.x + offsets.x * scale - attackCollider.size.x, pos.y + offsets.y * scale + 3};
  }

  //--------TODO: Attack  ----------

  if (attackBusy)
  {
    for (i32 i = 0; i < enemylist.size(); i++)
    {
      if (Collider::rect_vs_rect(this->attackCollider, enemylist[i]->get_collider()))
        enemylist[i]->get_hurt();
    }
  }

  // -----------Camera Settings--------------

  Vec2f screen_size = {1280.f, 720.f};

  Vec2f img_rect_size = {100.f, 100.f};
  static Vec2f img_rect_pos = pos - img_rect_size / 2;
  ;
  //if(Camera::get_instance().follow && img_rect_pos.x > pos.x || img_rect_pos.x + img_rect_size.w <  pos.x + collider.size.x)
  if (sprite->get_flip())
    cameraBuffer.x = pos.x - screen_size.x * 1.8f / 3.0f;
  else
    cameraBuffer.x = pos.x - screen_size.x / 3.0f;
  cameraBuffer.y = pos.y - (screen_size.y * 0.57f);

  r32 delx = cameraBuffer.x - Camera::get_instance().get_pos().x;
  r32 dely = cameraBuffer.y - Camera::get_instance().get_pos().y;

  if (Camera::get_instance().follow && (abs(delx) > 500))
    cameraMovingx = true;
  if (Camera::get_instance().follow && (abs(dely) > 100))
    cameraMovingy = true;

  if (cameraMovingx)
    Camera::get_instance().get_pos().x += (abs(delx / 20) > 2) ? (delx / 20) : 2 * SIGNOF(delx);
  if (cameraMovingy)
    Camera::get_instance().get_pos().y += (abs(dely / 8) > 2) ? (dely / 8) : 2 * SIGNOF(dely);

  if (abs(delx) < 40)
    cameraMovingx = false;
  if (abs(dely) < 20)
    cameraMovingy = false;

  //Bound Camera
  if (Camera::get_instance().get_pos().x < 0)
    Camera::get_instance().get_pos().x = 0;
  if (Camera::get_instance().get_pos().x > 64 * 16 * 2 - screen_size.x) //2 is map scale?
    Camera::get_instance().get_pos().x = 64 * 16 * 2 - screen_size.x;

  if (Camera::get_instance().get_pos().y < 0)
    Camera::get_instance().get_pos().y = 0;
  if (Camera::get_instance().get_pos().y > 32 * 16 * 2 - screen_size.y) //2 is map scale?
    Camera::get_instance().get_pos().y = 32 * 16 * 2 - screen_size.y;
}

void Player::setup_animations()
{
  sprite->add_animation("Idle", 0, 0, 100, 55, 8, 7);

  sprite->add_animation("Run", 8, 0, 100, 55, 10, 15);

  sprite->add_animation("Attack 1", 8, 1, 100, 55, 6, 12);
  sprite->add_animation("Attack 2", 4, 2, 100, 55, 6, 12);
  sprite->add_animation("Attack 3", 0, 3, 100, 55, 8, 16);

  sprite->add_animation("Jump", 8, 3, 100, 55, 4, 10);
  sprite->add_animation("Fall", 1, 4, 100, 55, 4, 10);

  sprite->add_animation("Hurt", 5, 4, 100, 55, 3, 10);
  sprite->add_animation("Death", 8, 4, 100, 55, 10, 10);

  sprite->add_animation("Block Idle", 8, 5, 100, 55, 8, 10);
  sprite->add_animation("Block", 6, 6, 100, 55, 5, 10);

  sprite->add_animation("Roll", 2, 7, 100, 55, 9, 10);

  //sprite->add_animation("Wall Slide", 5, 8, 100, 55, 5, 10);
}

// TODO(Pramish): Incorporate these with the acceleration
void Player::move_left()
{
  if (!attackBusy)
  {
    accn.x -= 0.003f;
    sprite->set_flip(true);
    if (!is_jumping)
      handle_animation_state();
    running = true;
    idle = false;
  }
}
void Player::move_right()
{
  if (!attackBusy)
  {
    accn.x += 0.003f;
    sprite->set_flip(false);
    if (!is_jumping)
      handle_animation_state();
    running = true;
    idle = false;
  }
}

void Player::stop_moving()
{
  idle = true;
  running = false;
  handle_animation_state();
}
void Player::stop_falling()
{
  falling = false;
  handle_animation_state();
}

void Player::attack()
{
  if (!attackBusy && attackState == 0)
  {
    attackState++;
    countTime = true;
    attackBusy = true;
  }
  if (!attackBusy && comboReady && attackState != 0)
  {
    attackState++;
    countTime = true;
    attackBusy = true;
    attackActiveTime = 0;
  }
  if (attackState > 3)
  {
    attackState = 0;
    countTime = false;
    attackBusy = false;
    attackActiveTime = 0;
  }

  handle_animation_state();
}

void Player::endAttack()
{
  attackBusy = false;
  comboReady = false;
  countTime = false;
  attackState = 0;
  attackActiveTime = 0;
  handle_animation_state();
}

void Player::jump()
{
  if (!is_jumping && jump_timer >= JUMP_TIMER_MAX && !attackBusy)
  {
    vel.y = -1.25f;
    is_jumping = true;
    falling = false;
    jump_timer = 0.f;
  }
  handle_animation_state();
}

void Player::fall()
{
  falling = true;
  handle_animation_state();
}

void Player::roll()
{
  sprite->play_animation("Roll");
  handle_animation_state();
}

void Player::get_hurt()
{
  sprite->play_animation("Hurt");
  handle_animation_state();
}

void Player::die()
{
  sprite->play_animation("Death");
  handle_animation_state();
}

void Player::block()
{
  sprite->play_animation("Block");
  handle_animation_state();
}

void Player::block_idle()
{
  sprite->play_animation("Block Idle");
  handle_animation_state();
}

void Player::handle_animation_state()
{
  if (attackBusy && attackState == 1)
    sprite->play_animation("Attack 1");
  else if (attackBusy && attackState == 2)
    sprite->play_animation("Attack 2");
  else if (attackBusy && attackState == 3)
    sprite->play_animation("Attack 3");
  else if (falling)
    sprite->play_animation("Fall");
  else if (is_jumping)
    sprite->play_animation("Jump");
  else if (idle)
    sprite->play_animation("Idle");
  else if (running)
    sprite->play_animation("Run");
}

Vec2f Player::get_pos()
{
  return pos;
}

Player::~Player()
{
  delete sprite;
}