#include "Backdrop.h"

#include "Graphics.h"
#include "Camera.h"
#include <SDL2/SDL.h>

using namespace types;

#define COUNT(array) (sizeof((array)) / sizeof(array[0]))

Backdrop::Backdrop(Graphics &graphics) : tile_set(Sprite(graphics, "data\\stringstar.png")) {}

void Backdrop::load_backdrop_game(r32 scale)
{
	i16 map_data[] = {
#include "..\\data\\map_bg.data"
	};
	
#define MAP_WIDTH 70
#define TILE_SIZE 16
	for (i32 i = 0; i < COUNT(map_data); i++)
	{
		if (map_data[i] >= 0)
		{
			i32 col = i % MAP_WIDTH;
			i32 row = i / MAP_WIDTH;
			i32 draw_col = map_data[i] % 18;
			i32 draw_row = map_data[i] / 18;
			Rect rect = {{col * TILE_SIZE * scale, row * TILE_SIZE * scale}, {TILE_SIZE * scale, TILE_SIZE * scale}};
			//Rect rect = {{(r32)col * TILE_SIZE, (r32)row * TILE_SIZE }, {(r32)TILE_SIZE , TILE_SIZE}};
			SDL_Rect src_rect = {draw_col * TILE_SIZE, draw_row * TILE_SIZE, TILE_SIZE, TILE_SIZE};
			positions.push_back(rect);
			//tiles.push_back(rect);
			draw_tiles.push_back(src_rect);
		}
	}
#undef MAP_WIDTH
#undef TILE_SIZE
}



void Backdrop::load_backdrop_menu(r32 scale)
{
	i16 map_data[] = {
#include "..\\data\\menu_bg.data"
	};
	
#define MAP_WIDTH 40
#define TILE_SIZE 16
	for (i32 i = 0; i < COUNT(map_data); i++)
	{
		if (map_data[i] >= 0)
		{
			i32 col = i % MAP_WIDTH;
			i32 row = i / MAP_WIDTH;
			i32 draw_col = map_data[i] % 18;
			i32 draw_row = map_data[i] / 18;
			Rect rect = {{col * TILE_SIZE * scale, row * TILE_SIZE * scale}, {TILE_SIZE * scale, TILE_SIZE * scale}};
			//Rect rect = {{(r32)col * TILE_SIZE, (r32)row * TILE_SIZE }, {(r32)TILE_SIZE , TILE_SIZE}};
			SDL_Rect src_rect = {draw_col * TILE_SIZE, draw_row * TILE_SIZE, TILE_SIZE, TILE_SIZE};
			positions.push_back(rect);
			//tiles.push_back(rect);
			draw_tiles.push_back(src_rect);
		}
	}
}

void Backdrop::draw(Graphics &graphics, r32 scale)
{
	for (i32 i = 0; i < draw_tiles.size(); i++)
	{
		Rect *box = &positions[i];
		SDL_Rect rect = {(i32)(box->pos.x), (i32)(box->pos.y), (i32)(box->size.w), (i32)(box->size.h)};
		tile_set.draw(graphics, draw_tiles[i], rect, scale);
	}
}
