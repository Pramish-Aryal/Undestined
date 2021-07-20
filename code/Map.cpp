#include "Map.h"
#include "Graphics.h"
#include "Camera.h"
#include <SDL2/SDL.h>

using namespace types;

#define COUNT(array) (sizeof((array)) / sizeof(array[0]))

Map::Map(Graphics &graphics) : tile_set(Sprite(graphics, "data\\tileset.png")) {}

void Map::load_map(std::string path)
{
	i8 map_data[] = {
#include "..\\data\\map.data"
	};
	
#define MAP_WIDTH 96
#define TILE_SIZE 16
	for (i32 i = 0; i < COUNT(map_data); i++)
	{
		if (map_data[i] >= 0)
		{
			i32 col = i % MAP_WIDTH;
			i32 row = i / MAP_WIDTH;
			i32 draw_col = map_data[i] % 12;
			i32 draw_row = map_data[i] / 12;
			Rect rect = {{col * TILE_SIZE * 3.f, row * TILE_SIZE * 3.f}, {TILE_SIZE * 3.f, TILE_SIZE * 3.f}};
			//Rect rect = {{(r32)col * TILE_SIZE, (r32)row * TILE_SIZE }, {(r32)TILE_SIZE , TILE_SIZE}};
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
		//if(box->pos.x < Camera::get_instance().get_pos().x || box->pos.x > Camera::get_instance().get_pos().x + graphics.get_display_resolution().w ||
		//box->pos.y < Camera::get_instance().get_pos().y || box->pos.y > Camera::get_instance().get_pos().y + graphics.get_display_resolution().h )
		//continue;
		//SDL_Rect rect = {(i32)(box->pos.x - camera.pos.x), (i32)(box->pos.y - camera.pos.y), (i32)box->size.w, (i32)box->size.h};
		SDL_Rect rect = {(i32)(box->pos.x), (i32)(box->pos.y), (i32)box->size.w, (i32)box->size.h};
		SDL_Rect rect2 = {(i32)(box->pos.x / 3), (i32)(box->pos.y / 3), (i32)(box->size.w / 3), (i32)(box->size.h / 3)};
		SDL_SetRenderDrawColor(graphics.get_renderer(), 255, 0, 0, 255);
		tile_set.draw(graphics, draw_tiles[i], rect, 1.f);
		SDL_RenderDrawRect(graphics.get_renderer(), &rect2);
	}
}