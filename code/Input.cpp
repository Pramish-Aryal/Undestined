#include "input.h"

void Input::begin_new_frame()
{
	m_pressed_keys.clear();
	m_released_keys.clear();
}

void Input::key_up_event(SDL_Event &event)
{
	m_released_keys[event.key.keysym.scancode] = true;
	m_held_keys[event.key.keysym.scancode] = false;
}

void Input::key_down_event(SDL_Event &event)
{
	m_pressed_keys[event.key.keysym.scancode] = true;
	m_held_keys[event.key.keysym.scancode] = true;
}

bool Input::Input::key_pressed(SDL_Scancode key)
{
	return m_pressed_keys[key];
}

bool Input::key_released(SDL_Scancode key)
{
	return m_released_keys[key];
}

bool Input::key_held(SDL_Scancode key)
{
	return m_held_keys[key];
}