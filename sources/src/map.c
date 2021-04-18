/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>
#define SIZE 10

struct map {
	int width;
	int height;
	unsigned char* grid;
};

#define CELL(i,j) ( (i) + (j) * map->width)

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
	  for (j = 0; j < height; j++)
	    map->grid[CELL(i,j)] = CELL_EMPTY;


	return map;
}

int map_is_inside(struct map* map, int x, int y) // Vérifie si la case de coordonnée x,y est dans la map
{
	assert(map);
	if ((x>map->width-1) || (x<0) || (y>( map->height-1)) || (y<0)) // (x,y) allant de (0,0) à (width-1, height-1)
	{
		return 0;
	}
	return 1;
}

void map_free(struct map *map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0xf0;
}

enum compose_type map_get_cell(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)];
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type & 0x0f) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;
	case BONUS_LIFE:
		window_display_image(sprite_get_bonus(BONUS_LIFE), x, y);
		break;
	}
}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) { // sub-types are encoded with the 4 less significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	}
}
void different_bomb_display(struct map* map, int x, int  y, unsigned char type)
{
	switch (type & 0x0f) {
		case BOMB_1:
	      window_display_image(sprite_get_bomb(1), x, y);
		case BOMB_2:
	      window_display_image(sprite_get_bomb(2), x, y);
	      break;
	    case BOMB_3:
	      window_display_image(sprite_get_bomb(3), x, y);
	      break;
	    case BOMB_4:
	      window_display_image(sprite_get_bomb(4), x, y);
	      break;
	    case BOMB_5:
	      window_display_image(sprite_get_bomb(5), x, y);
	      break;
	}
}

void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
	  for (int j = 0; j < map->height; j++) {
	    x = i * SIZE_BLOC;
	    y = j * SIZE_BLOC;

	    unsigned char type = map->grid[CELL(i,j)];
	    
	    switch (type & 0xf0) {
		case CELL_SCENERY:
		  display_scenery(map, x, y, type);
		  break;
	    case CELL_BOX:
	      window_display_image(sprite_get_box(), x, y);
	      break;

	    case CELL_BOMB:
	      different_bomb_display(map, x, y, type);
	      break;
	    
	    case CELL_BONUS:
	      display_bonus(map, x, y, type);
	      break;
	    case CELL_KEY:
	      window_display_image(sprite_get_key(), x, y);
	      break;
	    case CELL_DOOR:
	      // pas de gestion du type de porte
	      window_display_image(sprite_get_door_opened(), x, y);
	      break;
	    }
	  }
	}
}

int box_collsion(struct map* map, int x, int y, int dir) //prend en entrée la map, coordonnées de la boite et directin de déplacement
{
	switch (dir) //conditionnement sur la direction
	{
	case NORTH:
		if(map_is_inside(map, x, y-1)) //test pour savoir si la position d'arrivée est dans la map
		{
			if(map_get_cell_type(map, x, y-1) == CELL_EMPTY) //test si la position d'arrivée est vide
			{
				map_set_cell_type(map, x, y-1, CELL_BOX); //échange case d'arrivée et case de départ
				map_set_cell_type(map, x, y, CELL_EMPTY);
				return 1; //déplacement effectué, retourne 1
			}
			else
			{
				return 0; //déplacement impossible
			}
		}
		break;
	
	case SOUTH:
		if(map_is_inside(map, x, y+1))
		{
			if(map_get_cell_type(map, x, y+1) == CELL_EMPTY)
			{
				map_set_cell_type(map, x, y+1, CELL_BOX);
				map_set_cell_type(map, x, y, CELL_EMPTY);
				return 1;
			}
			else
			{
				return 0;
			}
		}
		break;

	case EAST:
		if(map_is_inside(map, x+1,y))
		{
			if(map_get_cell_type(map, x+1, y) == CELL_EMPTY)
			{
				map_set_cell_type(map, x+1, y, CELL_BOX);
				map_set_cell_type(map, x, y, CELL_EMPTY);
				return 1;
			}
			else
			{
				return 0;
			}
		}
		break;

	case WEST:
		if(map_is_inside(map, x-1, y))
		{
			if(map_get_cell_type(map, x-1, y) == CELL_EMPTY)
			{
				map_set_cell_type(map, x-1, y, CELL_BOX);
				map_set_cell_type(map, x, y, CELL_EMPTY);
				return 1;
			}
			else
			{
				return 0;
			}
		}
		break;

	default:
		break;
	}
	return 0; //retourne 0 si déplacement hors de la map
}




struct map* map_get_static(void)
{
	struct map* map = map_new(12, 12);

	/*unsigned char themap[STATIC_MAP_WIDTH * STATIC_MAP_HEIGHT] = {
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
	  CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX_RANGEINC, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX, CELL_MONSTER, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_BOX, CELL_STONE, CELL_EMPTY, CELL_MONSTER, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_RANGEINC, CELL_LIFE, CELL_RANGEDEC, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_BOX, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_BOX,  CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
	  CELL_BOX_MONSTER, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_BOX_LIFE, CELL_EMPTY,
	  CELL_BOX,  CELL_EMPTY, CELL_DOOR, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
		};*/



	int * map_test=write_map("./map/map_0");
	for (int i = 0; i < 144; i++)
		map->grid[i] = map_test[i];

	return map;
}
