#pragma once

struct SDL_Window;
struct SDL_Renderer;
class Graphics;

#include "Sprite.h"
class Game
{
	public:
	Game();
	~Game();
	
	private:
	void game_loop();
	void update(float dt);
	void draw(Graphics& graphics);
	void input();
	
	private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	Sprite player;
};

