#pragma once

#include <SDL2/SDL.h>
#include <map>

class Input
{
	std::map<SDL_Scancode, bool> m_pressed_keys;
	std::map<SDL_Scancode, bool> m_released_keys;
	std::map<SDL_Scancode, bool> m_held_keys;

public:
	void begin_new_frame();
	void key_up_event(SDL_Event &event);
	void key_down_event(SDL_Event &event);
	bool key_pressed(SDL_Scancode key);
	bool key_released(SDL_Scancode key);
	bool key_held(SDL_Scancode key);
};