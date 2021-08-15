#include "Game.h"

#ifdef WINDOW
#include <Windows.h>
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main(int argc, char **argv)
#endif
{
	try {
		Game game;
	} catch(...) {
		
	}
	return 0;
}