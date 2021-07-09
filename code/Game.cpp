#include "Game.h"
//hello this is change

#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>

#include "Input.h"
#include "Graphics.h"
#include "Fatal.h"
#include "AnimatedSprite.h"
#include "types.h"

namespace
{
	const r32 FPS = 60.0f;
	const r32 MAX_FRAME_TIME = 5 * 1000.0f / FPS;
	const r32 FRAME_TIME = 1000.0f / FPS;
}

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

static int x, y;

void Game::game_loop()
{
	Input input;
	Graphics graphics;
	player = new AnimatedSprite(graphics, "data\\HeroKnight.png");

	player->add_animation("idle", 0, 0, 100, 55, 8, 10);
	player->add_animation("run", 8, 0, 100, 55, 10, 10);
	player->add_animation("attack", 8, 1, 100, 55, 6, 10);
	//player->add_animation("idle", 0, 0, 50, 37, 4, 10);
	//player->add_animation("crouch", 4, 0, 50, 37, 4, 10);
	//player->add_animation("run", 1, 1, 50, 37, 7, 10);
	// player->add_animation("run and jump", 1, 1, 50, 37, 7, 10);
	// player->add_animation("run and jump", 1, 1, 50, 37, 7, 10);
	// player->add_animation("run and jump", 1, 1, 50, 37, 7, 10);
	player->play_animation("idle");

	types::r32 fixed_delta_time = FRAME_TIME;
	types::r32 accumulator = 0;
	types::r32 delta_time = fixed_delta_time;
	types::r32 current_time_ms = SDL_GetTicks();
	types::r32 last_time_ms = current_time_ms;
	while (m_game_is_running)
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
	}

	/* 
		while (m_game_is_running)
		{
			types::u32 start_time_ms = SDL_GetTicks();
	
			input();
	
			types::u32 current_time_ms = SDL_GetTicks();
			types::i32 elapsed_time = current_time_ms - last_time_ms;
			update(elapsed_time < MAX_FRAME_TIME ? elapsed_time : MAX_FRAME_TIME);
			last_time_ms = current_time_ms;
	
			draw(graphics);
	
			elapsed_time = SDL_GetTicks() - start_time_ms;
			if(elapsed_time < FRAME_TIME)
				SDL_Delay(FRAME_TIME - elapsed_time);
		}
	 */
}

void Game::simulate(r32 dt)
{
}

void Game::update(r32 dt)
{
	player->update(dt);
}

void Game::draw(Graphics &graphics)
{
	graphics.clear_screen(50, 100, 120);
	player->draw(graphics, x, y, 3);
	graphics.display();
}

void Game::handle_input(Input &input)
{
	if (input.key_held(SDL_SCANCODE_ESCAPE))
		set_game_running(false);
	if (input.key_held(SDL_SCANCODE_D))
	{
		x += 5;
		player->left = false;
		player->play_animation("run");
	}
	else if (input.key_held(SDL_SCANCODE_A))
	{
		x -= 5;
		player->left = true;
		player->play_animation("run");
	}
	else if (input.key_held(SDL_SCANCODE_W))
		y -= 10;
	else if (input.key_held(SDL_SCANCODE_S))
		y += 10;
	else if (input.key_pressed(SDL_SCANCODE_R))
		player->play_animation("idle");
	else if (input.key_pressed(SDL_SCANCODE_T))
		player->play_animation("crouch");
	else if (input.key_pressed(SDL_SCANCODE_Y))
		player->play_animation("run");
	else if (input.key_held(SDL_SCANCODE_SPACE))
		player->play_animation("attack");
	else
		player->play_animation("idle");
}

bool Game::is_game_running()
{
	return m_game_is_running;
}

void Game::set_game_running(bool value)
{
	m_game_is_running = value;
}