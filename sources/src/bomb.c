#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>

struct bomb {
	int x, y;
	int range;
};

struct bomb* bomb_init(int range) {
	struct bomb* bomb = malloc(sizeof(*bomb));
	if (!bomb)
		error("Memory error");
	bomb->range=range;
	return bomb;
}


void bomb_set_position(struct player *player,struct bomb *bomb) {
	assert(player);
	assert(bomb);
	bomb->x = player_get_x(player);
	bomb->y = player_get_y(player);
}


void bomb_free(struct bomb * bomb) {
	assert(bomb);
	free(bomb);
}



void bomb_display(struct bomb * bomb) {
	assert(bomb);
	window_display_image(sprite_get_bomb(),
			bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
}





///////////////// deposition de bombe pas encore finie
void put_bomb(struct player* player,struct map* map)
{
	//struct bomb * bomb =bomb_init(1); ///////////////////// passer le 1 en parametre
	if (player_get_nb_bomb(player)==0)
	{
		return ;
	}
	else
	{
		player_dec_nb_bomb(player);
		//bomb_display(bomb);
		map_set_cell_type(map,player_get_x(player) , player_get_y(player), CELL_BOMB);
		return ;
	}

}