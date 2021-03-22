#include <SDL/SDL_image.h>
#include <assert.h>

#include <monster.h>
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

void monster_free(struct monster* monster) {
	assert(monster);
	free(monster);
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

void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->direction = way;
}

static int monster_move_aux(struct monster* monster, struct map* map, int x, int y, int dir) { //ajout direction en entrée pour la gestion des boites

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0; // pas de déplacement possible si décor
		break;

	case CELL_BOX:
		return 1; 
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

int monster_move(struct monster* monster, struct map* map) {
	int x = monster->x;
	int y = monster->y;
	int move = 0;

	switch (monster->direction) {
	case NORTH:
		if (monster_move_aux(monster, map, x, y - 1, NORTH)) {
			monster->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (monster_move_aux(monster, map, x, y + 1, SOUTH)) {
			monster->y++;
			move = 1;
		}
		break;

	case WEST:
		if (monster_move_aux(monster, map, x - 1, y, WEST)) {
			monster->x--;
			move = 1;
		}
		break;

	case EAST:
		if (monster_move_aux(monster, map, x + 1, y, EAST)) {
			monster->x++;
			move = 1;
		}
		break;
	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
	}
	return move;
}

void monster_random(struct monster* monster, struct map* map)
{
	srand(time(NULL));
	if (monster->compteur >= DEFAULT_GAME_FPS*(monster->speed/1000))
	{
		int random_dir = rand() % 4;
		monster->direction = random_dir;
		monster_move(monster, map);
		monster->compteur = 0;
	}
	
	else{
		monster->compteur += 1;
	}
	
}

void monster_display(struct monster* monster, struct map* map) {
	assert(monster);
	monster_random(monster, map);
	window_display_image(sprite_get_monster(monster->direction),
			monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
}