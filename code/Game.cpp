#include "Game.h"

#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>

#include "AnimatedSprite.h"
#include "Fatal.h"
#include "Graphics.h"
#include "Input.h"
#include "Map.h"
#include "Player.h"
#include "Backdrop.h"
#include "Background.h"
#include "Camera.h"
#include "utils.h"

using namespace types;

namespace
{
	const r32 FPS = 60.0f;
	const r32 MAX_FRAME_TIME = 5 * 1000.0f / FPS;
	const r32 FRAME_TIME = 1000.0f / FPS;
} // anonymous namespace

Game::Game()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		Fatal::fatal_error("Couldn't init SDL");
	set_game_running(true);
	game_loop();
}

Game::~Game()
{
	SDL_Quit();
}

void Game::game_loop()
{
	Input input;
	Graphics graphics;
	player = new Player(graphics);
	
	map = new Map(graphics);
	map->load_map("", 2 ); 
	
	backdrop = new Backdrop(graphics);
	backdrop->load_backdrop("", 2);
	
	background = new Background(graphics);
	
	Camera::get_instance().get_pos() = graphics.get_display_resolution() / 2;
	
	r32 fixed_delta_time = FRAME_TIME;
	r32 accumulator = 0;
	r32 delta_time = fixed_delta_time;
	r32 current_time_ms = SDL_GetTicks();
	r32 last_time_ms = current_time_ms;
	
	while (is_game_running())
	{
		input.begin_new_frame();
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				m_game_is_running = false;
			
			if (event.type == SDL_KEYDOWN)
				input.key_down_event(event);
			
			if (event.type == SDL_KEYUP)
				input.key_up_event(event);
		}
		
		handle_input(input);
		accumulator += delta_time;
		while (accumulator >= fixed_delta_time)
		{
			simulate(fixed_delta_time);
			accumulator -= fixed_delta_time;
		}
		
		update(delta_time < MAX_FRAME_TIME ? delta_time : MAX_FRAME_TIME);
		draw(graphics);
		
		if ((SDL_GetTicks() - current_time_ms) < FRAME_TIME)
			SDL_Delay(FRAME_TIME - (SDL_GetTicks() - current_time_ms));
		
		current_time_ms = SDL_GetTicks();
		delta_time = current_time_ms - last_time_ms;
		last_time_ms = current_time_ms;
		// delta_time /= 10.0f;
	}
}

void Game::simulate(r32 dt)
{
	player->simulate(dt, *map);
}

void Game::update(r32 dt)
{
	player->update(dt);
}

bool DEBUG = false;
u8 scale = 10;
r32 player_scale = 1.5f;

void Game::draw(Graphics &graphics)
{
	graphics.clear_screen(50, 100, 120);
	background->draw(graphics);
	backdrop->draw(graphics);
	map->draw(graphics);
	if(DEBUG)
		map->debug_draw(graphics, scale);
	player->draw(graphics, player_scale);
	if(DEBUG)
		player->debug_draw(graphics, scale);
	graphics.display();
}


void Game::handle_input(Input &input)
{
	if (input.key_held(SDL_SCANCODE_ESCAPE))
		set_game_running(false);
	
	if (input.key_held(SDL_SCANCODE_E))
		player->attack();
	if (input.key_held(SDL_SCANCODE_D))
		player->move_right();
	if (input.key_held(SDL_SCANCODE_A))
		player->move_left();
	if (input.key_held(SDL_SCANCODE_SPACE))
		player->jump();
	// if (input.key_held(SDL_SCANCODE_X))
	//   player->die();
	// if (input.key_held(SDL_SCANCODE_Z))
	//   player->get_hurt();
	// if (input.key_held(SDL_SCANCODE_Q))
	//   player->block_idle();
	// if (input.key_held(SDL_SCANCODE_S))
	//   player->roll();
	// if (input.key_held(SDL_SCANCODE_F))
	//   player->fall();
	
	if(input.key_pressed(SDL_SCANCODE_O))
		if(scale < 255) scale++;
	if(input.key_pressed(SDL_SCANCODE_P))
		if(scale > 0)scale--;
	if(input.key_pressed(SDL_SCANCODE_M))
		if(player_scale < 255) player_scale += 0.1f;
	if(input.key_pressed(SDL_SCANCODE_N))
		if(player_scale > 0)  player_scale -= 0.1f;
	
	if (input.key_held(SDL_SCANCODE_UP))
		Camera::get_instance().get_pos().y += 5.f;
	if (input.key_held(SDL_SCANCODE_DOWN))
		Camera::get_instance().get_pos().y -= 5.f;
	if (input.key_held(SDL_SCANCODE_LEFT))
		Camera::get_instance().get_pos().x += 5.f;
	if (input.key_held(SDL_SCANCODE_RIGHT))
		Camera::get_instance().get_pos().x -= 5.f;
	if(input.key_pressed(SDL_SCANCODE_C))
		Camera::get_instance().follow = !(Camera::get_instance().follow);
	if(input.key_pressed(SDL_SCANCODE_X))
		DEBUG = !DEBUG;
}

bool Game::is_game_running()
{
	return m_game_is_running;
}

void Game::set_game_running(bool value)
{
	m_game_is_running = value;
}