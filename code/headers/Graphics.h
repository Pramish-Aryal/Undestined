#pragma once

struct SDL_Window;
struct SDL_Renderer;

class Graphics
{
	public:
	Graphics();
	~Graphics();
	
	void clear_screen(unsigned char r, unsigned char g, unsigned char b);
	//void blit_to_screen();
	
	void display();
	
	private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
};

