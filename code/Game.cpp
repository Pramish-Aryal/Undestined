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
#include "Food.h"
#include "Graphics.h"
#include "Input.h"
#include "Map.h"
#include "Player.h"
#include "Random.h"
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
	delete background;
	delete menu;
	SDL_Quit();
}

void Game::game_loop() {
	Input input;
	Graphics graphics;
	Font font(graphics);
	int threshold = 2;
	int lastInc = 0;
	
	enemylist.push_back(new Skeleton(graphics));
	enemylist.push_back(new FlyingEye(graphics));
	
	player = new Player(graphics);
	food = new Food(graphics);
	map = new Map(graphics);
	map->load_map("", 2);
	
	Backdrop *backdrops[2];
	backdrops[0] = new Backdrop(graphics);
	backdrops[0]->load_backdrop_menu("", 2);
	
	backdrops[1] = new Backdrop(graphics);
	backdrops[1]->load_backdrop_game("", 2);
	
	background = new Background(graphics);
	
	menu = new Menu(graphics);
	
	Camera::get_instance().get_pos() = graphics.get_display_resolution() / 2 - Vec2f(225, 250);
	
	r32 fixed_delta_time = FRAME_TIME;
	r32 accumulator = 0;
	r32 delta_time = fixed_delta_time;
	r32 current_time_ms = SDL_GetTicks();
	r32 last_time_ms = current_time_ms;
	
	//MAIN LOOP
	while (is_game_running()) {
		input.begin_new_frame();
		SDL_Event event;
		//Even polls
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT)
				m_game_is_running = false;
			
			if (event.type == SDL_KEYDOWN)
				input.key_down_event(event);
			
			if (event.type == SDL_KEYUP)
				input.key_up_event(event);
			
			if (event.type == SDL_MOUSEBUTTONDOWN)
				input.mouse_down_event();
			
			if (event.type == SDL_MOUSEBUTTONUP)
				input.mouse_up_event();
		}
		
		handle_input(input);  //Input Checking
		
		//Enemy Spawning
		int score = Enemy::get_score();
		if (score - lastInc == threshold && score != 0) {
			(Random::get_random(0, 2)) ? enemylist.push_back(new Skeleton(graphics)) : enemylist.push_back(new FlyingEye(graphics));
			lastInc = Enemy::get_score();
			if (threshold < 8)
				threshold++;
		}
		//gamestates checking stuff for newgame
		//Physics Simulation & Anim change
		if (game_state == PLAY) {
			accumulator += delta_time;
			while (accumulator >= fixed_delta_time) {
				simulate(fixed_delta_time);
				accumulator -= fixed_delta_time;
			}
			
			update(delta_time < MAX_FRAME_TIME ? delta_time : MAX_FRAME_TIME);
		} else {
			i32 x, y;
			graphics.get_mouse(x, y);
			menu->update_menu(Vec2f(x, y), &game_state, input.mouse_pressed());
		}
		
		if (player->get_endState() && playing) {
			game_state = GAMEOVER;
			threshold = 2;
			lastInc = 0;
			playing = false;
		}
		
		if (game_state == PLAY && !playing) {
			enemylist.clear();
			enemylist.push_back(new Skeleton(graphics));
			enemylist.push_back(new FlyingEye(graphics));
			Enemy::set_score(0);
			player->respawn();
			playing = true;
		}
		if (game_state == QUIT)
			set_game_running(false);
		
		if (game_state == MENU || game_state == PAUSE || game_state == GAMEOVER) {
			backdrop = backdrops[0];
			Camera::get_instance().get_pos() = {0, 0};
		} else
			backdrop = backdrops[1];
		
		draw(graphics, font);
		
		if ((SDL_GetTicks() - current_time_ms) < FRAME_TIME)
			SDL_Delay(FRAME_TIME - (SDL_GetTicks() - current_time_ms));
		
		current_time_ms = SDL_GetTicks();
		delta_time = current_time_ms - last_time_ms;
		last_time_ms = current_time_ms;
		//delta_time /= 5.0f;
	}
	delete backdrops[0];
	delete backdrops[1];
}

void Game::simulate(r32 dt) {
	player->simulate(dt, *map, enemylist, *food);
	food->simulate(dt);
	for (size_t i = 0; i < enemylist.size(); i++)
		enemylist[i]->simulate(dt, *map, *player);
}

void Game::update(r32 dt) {
	player->update(dt);
	for (size_t i = 0; i < enemylist.size(); i++)
		enemylist[i]->update(dt);
}

void Game::draw(Graphics &graphics, Font &font) {
	graphics.clear_screen(50, 100, 120);
	background->draw(graphics);
	backdrop->draw(graphics);
	
	//Draw frame per game state
	if (game_state == MENU) {
		menu->draw_menu(graphics, font);
	} else if (game_state == PAUSE) {
		menu->draw_pause(graphics, font);
	} else if (game_state == TUTORIAL) {
		menu->draw_tutorial(graphics);
	} else if (game_state == GAMEOVER) {
		menu->draw_gameover(graphics, font, Enemy::get_score());
	} else if (game_state == PLAY) {
		map->draw(graphics);
		food->draw(graphics);
		for (size_t i = 0; i < enemylist.size(); i++)
			enemylist[i]->draw(graphics, 1.5f);
		player->draw(graphics, 1.5f);
		menu->draw_score(graphics, font, Enemy::get_score());
	}
	graphics.display();
}

void Game::handle_input(Input &input) {
	if (input.key_pressed(SDL_SCANCODE_ESCAPE)) {
		if (game_state == MENU)
			set_game_running(false);
		else if (game_state == PAUSE || game_state == TUTORIAL || game_state == GAMEOVER)
			game_state = MENU;
		else
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
	}
}

bool Game::is_game_running() {
	return m_game_is_running;
}

void Game::set_game_running(bool value) {
	m_game_is_running = value;
}