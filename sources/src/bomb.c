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

int condition_explosion(struct map * map,int x ,int y){
	if(map_is_inside(map,x,y)){// si dans le terrain
		if(map_get_cell_type(map,x,y)==0 || map_get_cell_type(map,x,y)==32){ // si c'est cassable
			return 1;
		}
	}
	return 0;
}

void explosion(struct bomb * bomb, struct map * map)
{
	int compteur=1;
	while (bomb->area>=compteur && condition_explosion(map,bomb->x+compteur,bomb->y)==1){
		map_set_cell_type(map,bomb->x+(compteur) , bomb->y, CELL_TREE);
		compteur+=1;
	}
	compteur=1;
	while (bomb->area>=compteur && condition_explosion(map,bomb->x,bomb->y+(compteur))==1){
		map_set_cell_type(map,bomb->x , bomb->y+(compteur), CELL_TREE);
		compteur+=1;
	}
	compteur=1;
	while (bomb->area>=compteur && condition_explosion(map,bomb->x-(compteur),bomb->y)==1){
		map_set_cell_type(map,bomb->x-(compteur) , bomb->y, CELL_TREE);
		compteur+=1;
	}
	compteur=1;
	while (bomb->area>=compteur && condition_explosion(map,bomb->x,bomb->y-(compteur))==1){
		map_set_cell_type(map,bomb->x , bomb->y-(compteur), CELL_TREE);
		compteur+=1;	
	}
			
		
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

		///explosion affichage 
		explosion(bomb,map);
		return ;
	}

}