#pragma once

struct SDL_Window;
struct SDL_Renderer;

class Graphics;
class Input;
class AnimatedSprite;

#include "types.h"

using namespace types;

class Game
{

public:
	Game();
	~Game();

private:
	void game_loop();
	void simulate(r32 dt);
	void update(r32 dt);
	void draw(Graphics &graphics);
	void handle_input(Input &input);
	bool is_game_running();
	void set_game_running(bool value);

private:
	SDL_Window *_window;
	SDL_Renderer *_renderer;
	AnimatedSprite *player;
	bool m_game_is_running;
};
