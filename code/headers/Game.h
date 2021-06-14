#pragma once

struct SDL_Window;
struct SDL_Renderer;
class Graphics;

#include "AnimatedSprite.h"
#include "types.h"

class Game
{

public:
	Game();
	~Game();
	
	private:
	void game_loop();
	void update(types::r32 dt);
	void draw(Graphics& graphics);
	void input();
	bool is_game_running();
	void set_game_running(bool value);

private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	AnimatedSprite player;
	bool m_game_is_running;
};

