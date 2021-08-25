#include "Game.h"

#ifndef CONSOLE
#include <Windows.h>
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main(int argc, char **argv)
#endif
{
	Game game;
	game.game_loop();
	return 0;
}