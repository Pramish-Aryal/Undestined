#include "Game.h"

#include <SDL2/SDL.h>
#include <iostream>
#include <cstdlib>

#include "Graphics.h"
#include "Fatal.h"
#include "Sprite.h"

Game::Game()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		Fatal::fatal_error("Couldn't init SDL");
	game_loop();
}

Game::~Game()
{
	SDL_Quit();
}

void Game::game_loop()
{
	Graphics graphics;
	player = Sprite(graphics, "data\\rgba_tester.png");
	bool game_is_running = true;
	SDL_Event event;
	
	while (game_is_running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				game_is_running = false;
			
			if (event.type == SDL_KEYDOWN)
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				game_is_running = false;
		}
		
		draw(graphics);
	}
} 

void Game::update(float dt)
{
}

void Game::draw(Graphics& graphics)
{
	graphics.clear_screen(50, 100, 120);
	player.draw(graphics, 0, 0, 10.0f);
	graphics.display();
}

void Game::input()
{
	
}