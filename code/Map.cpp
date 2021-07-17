#include "Map.h"
#include "Graphics.h"
using namespace types;
#include <SDL2/SDL.h>
#define COUNT(array) (sizeof((array)) / sizeof(array[0]))

Map::Map(Graphics &graphics) : tile_set(Sprite(graphics, "data\\tileset.png"))
{
}

void Map::load_map(std::string path)
{
	i8 map_data[] = {
#include "..\\data\\map.data"
	};
	
#define MAP_WIDTH 48
#define TILE_SIZE 16
	for (i32 i = 0; i < COUNT(map_data); i++)
	{
		if (map_data[i] != -1)
		{
			i32 col = i % MAP_WIDTH;
			i32 row = i / MAP_WIDTH;
			i32 draw_col = map_data[i] % 12;
			i32 draw_row = map_data[i] / 12;
			Rect rect = {{col * TILE_SIZE * 3.f, row * TILE_SIZE * 3.f}, {TILE_SIZE * 3.f, TILE_SIZE * 3.f}};
			SDL_Rect src_rect = {draw_col * TILE_SIZE, draw_row * TILE_SIZE, TILE_SIZE, TILE_SIZE};
			bounding_boxes.push_back(rect);
			//tiles.push_back(rect);
			draw_tiles.push_back(src_rect);
		}
	}
}

void Map::draw(Graphics &graphics)
{
	for (i32 i = 0; i < draw_tiles.size(); i++)
	{
		Rect *box = &bounding_boxes[i];
		SDL_Rect rect = {(i32)box->pos.x, (i32)box->pos.y, (i32)box->size.w, (i32)box->size.h};
		SDL_SetRenderDrawColor(graphics.get_renderer(), 255, 0, 0, 255);
		tile_set.draw(graphics, draw_tiles[i], rect, 1.f);
		SDL_RenderDrawRect(graphics.get_renderer(), &rect);
	}
}