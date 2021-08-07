#pragma once

struct SDL_Window;
struct SDL_Renderer;

class Graphics;
class Input;
class Player;
class Map;
class Backdrop;
class Background;

#include "utils.h"

class Game
{
	
	public:
	Game();
	~Game();
	
	private:
	void game_loop();
	void simulate(types::r32 dt);
	void update(types::r32 dt);
	void draw(Graphics &graphics);
	void handle_input(Input &input);
	bool is_game_running();
	void set_game_running(bool value);
	
	private:
	SDL_Window *_window;
	SDL_Renderer *_renderer;
	Player *player;
	Map* map;
	Backdrop* backdrop;
	Background* background;
	bool m_game_is_running;
};
