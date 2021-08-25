#pragma once

struct SDL_Window;
struct SDL_Renderer;

class Graphics;
class Input;
class Player;
class Map;
class Backdrop;
class Background;
class Enemy;
class Font;
class Food;
class Menu;

#include "utils.h"
#include <vector>

class Game
{
	public:
	Game();
	~Game();
	
	void game_loop();
	
	private:
	void simulate(types::r32 dt);
	void update(types::r32 dt);
	void draw(Graphics &graphics, Font &font);
	void handle_input(Input &input);
	bool is_game_running();
	void set_game_running(bool value);
	
	private:
	SDL_Window *_window;
	SDL_Renderer *_renderer;
	Player *player;
	Map *map;
	Menu *menu;
	Backdrop *backdrop;
	Background *background;
	bool m_game_is_running;
	bool playing = true;
	std::vector<Enemy *> enemylist;
	Food *food;
	enum
	{
		MENU = 1,
		PLAY = 2,
		PAUSE = 3,
		TUTORIAL = 4,
		QUIT = 5,
		GAMEOVER = 6
	} game_state = MENU;
};
