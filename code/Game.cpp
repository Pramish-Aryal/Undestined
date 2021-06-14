#include "Game.h"

#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>

#include "Graphics.h"
#include "Fatal.h"
#include "AnimatedSprite.h"
#include "types.h"

namespace 
{
	const types::r32 FPS = 60.0f;
	const types::r32 MAX_FRAME_TIME = 5 * 1000.0f / FPS;
	const types::r32 FRAME_TIME = 1000.0f / FPS;
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

void Game::game_loop()
{
	Graphics graphics;
	player = AnimatedSprite(graphics, "data\\Run.png");
	
	types::u32 last_time_ms = SDL_GetTicks();

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
}

void Game::update(types::r32 dt)
{
	player.update(dt);
}

void Game::draw(Graphics& graphics)
{
	graphics.clear_screen(50, 100, 120);
	player.draw(graphics);
	graphics.display();
}

void Game::input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			m_game_is_running = false;
		
		if (event.type == SDL_KEYDOWN)
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
			m_game_is_running = false;
	}
}

bool Game::is_game_running()
{
	return m_game_is_running;
}

void Game::set_game_running(bool value)
{
	m_game_is_running = value;
}