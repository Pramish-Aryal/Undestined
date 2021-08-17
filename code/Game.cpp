#include "Game.h"

#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>

#include "AnimatedSprite.h"
#include "Backdrop.h"
#include "Background.h"
#include "Camera.h"
#include "Fatal.h"
#include "FlyingEye.h"
#include "Graphics.h"
#include "Input.h"
#include "Map.h"
#include "Player.h"
#include "Skeleton.h"
#include "UI.h"
#include "utils.h"

using namespace types;

namespace {
	const r32 FPS = 60.0f;
	const r32 MAX_FRAME_TIME = 5 * 1000.0f / FPS;
	const r32 FRAME_TIME = 1000.0f / FPS;
}  // anonymous namespace

Game::Game() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		Fatal::fatal_error("Couldn't init SDL");
	set_game_running(true);
	game_loop();
}

Game::~Game() {
	delete player;
	for (size_t i = 0; i < enemylist.size(); i++)
		delete enemylist[i];
	delete map;
	delete backdrop;
	delete background;
	delete menu;
	SDL_Quit();
}

void Game::game_loop() {
	Input input;
	Graphics graphics;
	Font font(graphics);
	
	enemylist.push_back(new Skeleton(graphics));
	enemylist.push_back(new Skeleton(graphics, {100, 200}));
	enemylist.push_back(new Skeleton(graphics, {900, 100}));
	enemylist.push_back(new Skeleton(graphics, {400, 300}));
	enemylist.push_back(new Skeleton(graphics, {300, 200}));
	enemylist.push_back(new FlyingEye(graphics));
	// enemylist.push_back(new FlyingEye(graphics, {150, 200}));
	// enemylist.push_back(new FlyingEye(graphics, {300, 250}));
	// enemylist.push_back(new FlyingEye(graphics, {500, 200}));
	
	player = new Player(graphics);
	map = new Map(graphics);
	map->load_map("", 2);
	
	backdrop = new Backdrop(graphics);
	backdrop->load_backdrop("", 2);
	
	background = new Background(graphics);
	
	menu = new Menu;
	
	Camera::get_instance().get_pos() = graphics.get_display_resolution() / 2 - Vec2f(225, 250);
	
	r32 fixed_delta_time = FRAME_TIME;
	r32 accumulator = 0;
	r32 delta_time = fixed_delta_time;
	r32 current_time_ms = SDL_GetTicks();
	r32 last_time_ms = current_time_ms;
	
	while (is_game_running()) {
		input.begin_new_frame();
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				m_game_is_running = false;
			
			if (event.type == SDL_KEYDOWN)
				input.key_down_event(event);
			
			if (event.type == SDL_KEYUP)
				input.key_up_event(event);
			
			if(event.type == SDL_MOUSEBUTTONDOWN)
				input.mouse_down_event();
			
			
			if(event.type == SDL_MOUSEBUTTONUP)
				input.mouse_up_event();
		}
		
		handle_input(input);
		if (game_state == PLAY) {
			accumulator += delta_time;
			while (accumulator >= fixed_delta_time) {
				simulate(fixed_delta_time);
				accumulator -= fixed_delta_time;
			}
			
			update(delta_time < MAX_FRAME_TIME ? delta_time : MAX_FRAME_TIME);
		}
		
		draw(graphics, font);
		
		if ((SDL_GetTicks() - current_time_ms) < FRAME_TIME)
			SDL_Delay(FRAME_TIME - (SDL_GetTicks() - current_time_ms));
		
		current_time_ms = SDL_GetTicks();
		delta_time = current_time_ms - last_time_ms;
		last_time_ms = current_time_ms;
		//delta_time /= 5.0f;
	}
}

void Game::simulate(r32 dt) {
	player->simulate(dt, *map, enemylist);
	for (size_t i = 0; i < enemylist.size(); i++)
		enemylist[i]->simulate(dt, *map, *player);
}

void Game::update(r32 dt) {
	player->update(dt);
	for (size_t i = 0; i < enemylist.size(); i++)
		enemylist[i]->update(dt);
}

bool DEBUG = false;
u8 scale = 10;
r32 player_scale = 1.5f;

void Game::draw(Graphics &graphics, Font &font) {
	graphics.clear_screen(50, 100, 120);
	background->draw(graphics);
	backdrop->draw(graphics);
	map->draw(graphics);
	if (game_state == MENU) {
		menu->draw_menu(graphics, font);
	} else if (game_state == PAUSE) {
		menu->draw_pause(graphics, font);
	} else {
		if (DEBUG)
			map->debug_draw(graphics, scale);
		
		for (size_t i = 0; i < enemylist.size(); i++)
			enemylist[i]->draw(graphics, player_scale);
		if (DEBUG)
			for (size_t i = 0; i < enemylist.size(); i++)
			enemylist[i]->debug_draw(graphics, 3.f);
		player->draw(graphics, player_scale);
		if (DEBUG)
			player->debug_draw(graphics, scale);
		menu->draw_score(graphics, font, Enemy::get_score());
	}
	graphics.display();
}

void Game::handle_input(Input &input) {
	if (input.key_held(SDL_SCANCODE_ESCAPE))
		set_game_running(false);
	if (game_state == PLAY) {
		if (input.key_held(SDL_SCANCODE_P))
			game_state = PAUSE;
	}
	
	if (game_state == PLAY) {
		if (input.key_held(SDL_SCANCODE_E) || input.mouse_pressed())
			player->attack();
		if (input.key_held(SDL_SCANCODE_R))
			enemylist[6]->stop_moving();
		
		if (input.key_held(SDL_SCANCODE_Q))
			enemylist[6]->attack();
		
		if (input.key_held(SDL_SCANCODE_D) && !input.key_held(SDL_SCANCODE_A))
			player->move_right();
		if (input.key_held(SDL_SCANCODE_A) && !input.key_held(SDL_SCANCODE_D))
			player->move_left();
		
		if (input.key_held(SDL_SCANCODE_SPACE))
			player->jump();
		
		if (input.key_pressed(SDL_SCANCODE_I))
			if (scale < 255)
			scale++;
		if (input.key_pressed(SDL_SCANCODE_O))
			if (scale > 0)
			scale--;
		
		if (input.key_pressed(SDL_SCANCODE_X))
			DEBUG = !DEBUG;
		
	} else {
		if (input.key_pressed(SDL_SCANCODE_RETURN))
			game_state = PLAY;
	}
}

bool Game::is_game_running() {
	return m_game_is_running;
}

void Game::set_game_running(bool value) {
	m_game_is_running = value;
}