#include <SDL/SDL_image.h>
#include <assert.h>

#include <monster.h>
#include <player.h>
#include <bomb.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <stdlib.h>
#include <time.h>

struct monster {
    int x, y;
	enum direction direction;
	int speed;
	int compteur;
};

struct monster* monster_init(int s)
{
    struct monster* monster = malloc(sizeof(*monster));
    if (!monster)
		error("Memory error");

	monster->direction = NORTH;
	monster->speed = s;
	monster->compteur = 0;

	return monster;
}

void monster_free(struct monster* monster[], struct map* map, int *nb_monster) {
	assert(monster);
	for (int i = 0;i<*nb_monster;i++)
	{
		free(monster[i]);
	}
	*nb_monster = 0;
}

void monster_load(struct monster* monster[], struct map* map, int *nb_monster, int level)
{
	for (int i = 0; i<STATIC_MAP_WIDTH;i++)
	{
		for (int j = 0;j<STATIC_MAP_HEIGHT;j++)
		{
			if(map_get_cell(map, i, j) == CELL_MONSTER)
			{
				monster[*nb_monster] = monster_init(1000-(level*75));
				monster_set_position(monster[*nb_monster], i, j);
				map_set_cell_type(map, i, j, CELL_EMPTY);
				*nb_monster += 1;
			}
		}
	}
}

void monster_save(struct monster* monster[], struct map* map, int nb_monster)
{
	for(int i = 0;i<nb_monster;i++)
	{
		map_set_cell(map,monster_get_x(monster[i]),monster_get_y(monster[i]), CELL_MONSTER);
	}
}

void monster_set_position(struct monster *monster, int x, int y) {
	assert(monster);
	monster->x = x;
	monster->y = y;
}

int monster_get_x(struct monster* monster) {
	assert(monster != NULL);
	return monster->x;
}

int monster_get_y(struct monster* monster) {
	assert(monster != NULL);
	return monster->y;
}

int monster_on_pos(struct monster* monster[], int nb_monster, int x, int y)
{
	if(!nb_monster)
	{
		return 1;
	}
	for(int i = 0;i<nb_monster;i++)
	{
		if((monster_get_x(monster[i]) == x) && (monster_get_y(monster[i]) == y))
		{
			return 0;
		}
	}

	return 1;
}

void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->direction = way;
}

static int monster_move_aux(struct monster* monster, struct map* map, int x, int y) { //ajout direction en entrée pour la gestion des boites

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0; // pas de déplacement possible si décor
		break;

	case CELL_BOX:
		return 0; 
		break;

	case CELL_BONUS:
		return 1;
		break;

	default:
		break;
	}

	// monster has moved
	return 1;
}

static int monster_near_door(struct monster* monster, struct map* map, int x, int y)
{
	if (!map_is_inside(map, x, y))
	{
		return 0;
	}

	if ((map_get_cell(map, x, y) & 0xf0) == CELL_DOOR)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int monster_move(struct monster* monster, struct map* map) {
	int x = monster->x;
	int y = monster->y;
	int move = 0;

	switch (monster->direction) {
	case NORTH:
		if (monster_move_aux(monster, map, x, y - 1)) {
			monster->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (monster_move_aux(monster, map, x, y + 1)) {
			monster->y++;
			move = 1;
		}
		break;

	case WEST:
		if (monster_move_aux(monster, map, x - 1, y)) {
			monster->x--;
			move = 1;
		}
		break;

	case EAST:
		if (monster_move_aux(monster, map, x + 1, y)) {
			monster->x++;
			move = 1;
		}
		break;
	}

	return move;
}

int monster_possible_dir(struct monster* monster, struct map* map, enum direction dir[])
{
	int i = 0;
	if (monster_move_aux(monster, map, monster->x, monster->y - 1) && !monster_near_door(monster, map, monster->x, monster->y-2) && !monster_near_door(monster, map, monster->x, monster->y-1))
	{
		dir[i] = NORTH;
		i++;
	}
	if (monster_move_aux(monster, map, monster->x + 1, monster->y) && !monster_near_door(monster, map, monster->x+2, monster->y) && !monster_near_door(monster, map, monster->x+1, monster->y))
	{
		dir[i] = EAST;
		i++;
	}
	if (monster_move_aux(monster, map, monster->x, monster->y + 1) && !monster_near_door(monster, map, monster->x, monster->y+2) && !monster_near_door(monster, map, monster->x, monster->y+1))
	{
		dir[i] = SOUTH;
		i++;
	}
	if (monster_move_aux(monster, map, monster->x - 1, monster->y) && !monster_near_door(monster, map, monster->x-2, monster->y) && !monster_near_door(monster, map, monster->x-1, monster->y))
	{
		dir[i] = WEST;
		i++;
	}
	return i;
}

void monster_random(struct monster* monster, struct map* map)
{
	srand(time(NULL));
	if (monster->compteur >= DEFAULT_GAME_FPS*monster->speed/1000)
	{
		enum direction dir[4] = {0,0,0,0};
		int n = monster_possible_dir(monster, map, dir);
		int random_dir = rand() % n;
		monster->direction = dir[random_dir];
		monster_move(monster, map);
		monster->compteur = 0;
	}
	
	else{
		monster->compteur += 1;
	}
	
}

int monster_kill(struct monster* monster, struct map * map)
{
		if (explosion_dead(map,monster->x,monster->y)==0)
		{
			return 1;
		}
	return 0;
}

void monster_dammage(struct player* player, struct monster* monster)
{
	if ((player_get_x(player) == monster->x) && (player_get_y(player) == monster->y))
	{
		if(invulnerability()==0)
		{
			adjust_Ancien_temps();
			player_dec_life(player);
		}
	}
}

void monster_display(struct monster* monster, struct map* map, struct player* player) {
	assert(monster);
	monster_random(monster, map);
	monster_dammage(player, monster);
	window_display_image(sprite_get_monster(monster->direction),
			monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
}