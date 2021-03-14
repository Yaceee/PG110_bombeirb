#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>


struct bomb {
	int x, y;
	int area;
};

struct bomb* bomb_init(int area, struct player * player) {
	struct bomb* bomb = malloc(sizeof(*bomb));
	if (!bomb)
		error("Memory error");

	bomb->area = area;
	bomb->x = player_get_x(player);
	bomb->y = player_get_y(player);
	return bomb;
}




void bomb_free(struct bomb * bomb) {
	assert(bomb);
	free(bomb);
}




///////////////// deposition de bombe pas encore finie
void put_bomb(struct player* player,struct map* map)
{
	
	struct bomb * bomb =bomb_init(2,player); ///////////////////// passer le 1 en parametre
	if (player_get_nb_bomb(player)==0)
	{
		return ;
	}
	else
	{
		player_dec_nb_bomb(player);
		map_set_cell_type(map,bomb->x , bomb->y, CELL_TREE);
		
		//mettre les differentes bombes
		///explosion
		while (bomb->area>=0){
			map_set_cell_type(map,bomb->x+(bomb->area) , bomb->y, CELL_TREE);
			map_set_cell_type(map,bomb->x , bomb->y+(bomb->area), CELL_TREE);
			map_set_cell_type(map,bomb->x-(bomb->area) , bomb->y, CELL_TREE);
			map_set_cell_type(map,bomb->x , bomb->y-(bomb->area), CELL_TREE);
			bomb->area-=1;
		}
		
		return ;
	}

}