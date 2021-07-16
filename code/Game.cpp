#include "Game.h"

#include <SDL2/SDL.h>

#include <cstdlib>
#include <iostream>

#include "utils.h"
#include "AnimatedSprite.h"
#include "Fatal.h"
#include "Graphics.h"
#include "Input.h"
#include "Player.h"
#include "Map.h"

namespace
{
	const r32 FPS = 60.0f;
	const r32 MAX_FRAME_TIME = 5 * 1000.0f / FPS;
	const r32 FRAME_TIME = 1000.0f / FPS;
} // namespace

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

//testing
#include <algorithm>
#include <utility>
#include <vector>

#include "Collision.h"

void Game::game_loop()
{
	Input input;
	Graphics graphics;
	player = new Player(graphics);
	
	map = new Map(graphics);
	map->load_map("");
	
	r32 fixed_delta_time = FRAME_TIME;
	r32 accumulator = 0;
	r32 delta_time = fixed_delta_time;
	r32 current_time_ms = SDL_GetTicks();
	r32 last_time_ms = current_time_ms;
	
#define TESTIN
#ifdef TESTING
	std::vector<Rect> rects;
	
	rects.push_back({{10.f * 3, 10.f * 3}, {10.f * 3, 30.f * 3}});
	
	//rects.push_back({{10.f,200.f}, {20.f, 20.f}});
	rects.push_back({{30.f * 3, 200.f * 3}, {20.f * 3, 20.f * 3}});
	rects.push_back({{50.f * 3, 200.f * 3}, {20.f * 3, 20.f * 3}});
	rects.push_back({{70.f * 3, 200.f * 3}, {20.f * 3, 20.f * 3}});
	rects.push_back({{90.f * 3, 200.f * 3}, {20.f * 3, 20.f * 3}});
	rects.push_back({{110.f * 3, 200.f * 3}, {20.f * 3, 20.f * 3}});
	rects.push_back({{130.f * 3, 200.f * 3}, {20.f * 3, 20.f * 3}});
	rects.push_back({{150.f * 3, 200.f * 3}, {20.f * 3, 20.f * 3}});
	rects.push_back({{170.f * 3, 200.f * 3}, {20.f * 3, 20.f * 3}});
	rects.push_back({{190.f * 3, 200.f * 3}, {20.f * 3, 20.f * 3}});
	
	rects.push_back({{25.f * 3, 150.f * 3}, {5.f * 3, 70.f * 3}});
	rects.push_back({{210.f * 3, 150.f * 3}, {5.f * 3, 70.f * 3}});
	
	rects.push_back({{100.f * 3, 20.f * 3}, {30.f * 3, 100.f * 3}});
	rects.push_back({{50.f * 3, 60.f * 3}, {100.f * 3, 30.f * 3}});
	
	rects.push_back({{150.f * 3, 30.f * 3}, {5.f * 3, 5.f * 3}});
	
	Vec2f vel = {0, 0};
	bool left_clicked = false;
	
#endif
	
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
#ifdef TESTING
			if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
				left_clicked = true;
			if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
				left_clicked = false;
#endif
		}
		
		handle_input(input);
		accumulator += delta_time;
		while (accumulator >= fixed_delta_time)
		{
			simulate(fixed_delta_time);
			accumulator -= fixed_delta_time;
		}
		
		update(delta_time < MAX_FRAME_TIME ? delta_time : MAX_FRAME_TIME);
		
#ifdef TESTING
		//testing
		SDL_Renderer *renderer = graphics.get_renderer();
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		
		SDL_RenderClear(renderer);
		
		int x, y;
		SDL_GetMouseState(&x, &y);
		Vec2f mouse = {(r32)x, (r32)y};
		
		if (left_clicked)
			vel += (mouse - rects[0].pos).normal() * 0.001f * delta_time;
		
		Vec2f cp, cn;
		r32 t;
		
		std::vector<std::pair<int, float>> z;
		
		for (int i = 1; i < rects.size(); i++)
		{
			if (Collider::dynamic_rect_vs_rect(&rects[0], vel, &rects[i], cp, cn, t, delta_time))
				z.push_back({i, t});
			//vel += cn * Vec2f(ABS(vel.x), ABS(vel.y))  * ( 1 - t);
		}
		
		std::sort(z.begin(), z.end(), [](const std::pair<int, float> &a, const std::pair<int, float> &b)
				  { return a.second < b.second; });
		
		for (auto j : z)
		{
			if (Collider::dynamic_rect_vs_rect(&rects[0], vel, &rects[j.first], cp, cn, t, delta_time))
				vel += cn * Vec2f(ABS(vel.x), ABS(vel.y)) * (1 - t);
		}
		
		rects[0].pos += vel * delta_time;
		
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		for (const auto &r : rects)
		{
			SDL_Rect rect = {(i32)r.pos.x, (i32)r.pos.y, (i32)r.size.w, (i32)r.size.h};
			SDL_RenderDrawRect(renderer, &rect);
		}
		
		SDL_RenderPresent(renderer);
#else
		draw(graphics);
#endif
		
		if ((SDL_GetTicks() - current_time_ms) < FRAME_TIME)
			SDL_Delay(FRAME_TIME - (SDL_GetTicks() - current_time_ms));
		
		current_time_ms = SDL_GetTicks();
		delta_time = current_time_ms - last_time_ms;
		last_time_ms = current_time_ms;
	}
	
#pragma region previous_game_loop
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
#pragma endregion
}

void Game::simulate(r32 dt)
{
	player->simulate(dt, *map);
}

void Game::update(r32 dt)
{
	player->update(dt);
}

void Game::draw(Graphics &graphics)
{
	graphics.clear_screen(50, 100, 120);
	map->draw(graphics);
	player->draw(graphics);
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
	else if (input.key_held(SDL_SCANCODE_A))
		player->move_left();
	else if (input.key_held(SDL_SCANCODE_X))
		player->die();
	else if (input.key_held(SDL_SCANCODE_Z))
		player->get_hurt();
	else if (input.key_held(SDL_SCANCODE_Q))
		player->block_idle();
	if (input.key_held(SDL_SCANCODE_SPACE))
		player->jump();
	else if (input.key_held(SDL_SCANCODE_S))
		player->roll();
	else if (input.key_held(SDL_SCANCODE_F))
		player->fall();
}

bool Game::is_game_running()
{
	return m_game_is_running;
}

void Game::set_game_running(bool value)
{
	m_game_is_running = value;
}