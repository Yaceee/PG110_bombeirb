#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#define SIZE 9


//////////////
//structures//
//////////////
struct bomb {
	int x, y;
	int area;
	int time;
	int used; ///// toutes les structures sont init et on regarde juste si les bombes sont utilisées 
};

//////////////////////
//variables globales//
//////////////////////
int verification_init=0;
struct bomb tab_bomb[SIZE]; //tableau des bombes deja posées


/////////////
//fonctions//
/////////////

struct bomb bomb_init(struct bomb * bomb ,int area, struct player * player) {
	bomb->area = area;
	bomb->x = player_get_x(player);
	bomb->y = player_get_y(player);
	bomb->time = SDL_GetTicks();
	bomb->used = 1;
	return *bomb;
}


void bomb_free(struct bomb * bomb) {
	assert(bomb);
	free(bomb);
}

//// Permet d'initialiser les bombes au debut
struct bomb* bomb_init_for_tab() {
	struct bomb* bomb = malloc(sizeof(*bomb));
	if (!bomb)
		error("Memory error");

	bomb->area = 0;
	bomb->x = 0;
	bomb->y = 0;
	bomb->time = 0;
	bomb->used = 0;
	return bomb;
}
//// Permet d'initialiser le tableau avec des structs bomb au debut
void initialisation_tab_bomb()
{
	for(int i=0 ;i<9; i++)
	{
		struct bomb * bomb=bomb_init_for_tab();
		tab_bomb[i]=*bomb;
	}
}

int condition_explosion(struct map * map,int x ,int y){
	if(map_is_inside(map,x,y)){// si dans le terrain
		if(map_get_cell_type(map,x,y)==0 || map_get_cell_type(map,x,y)==32  || (map_get_cell_type(map,x,y) & 0xf0 )== CELL_BOMB){ // si c'est cassable
			return 1;
		}
	}
	return 0;
}

void explosion(struct bomb * bomb, struct map * map, enum cell_type CELL)
{
	int compteur=0;
	while (bomb->area>=compteur && condition_explosion(map,bomb->x+compteur,bomb->y)==1){
		map_set_cell_type(map,bomb->x+(compteur) , bomb->y, CELL);
		compteur+=1;
	}
	compteur=0;
	while (bomb->area>=compteur && condition_explosion(map,bomb->x,bomb->y+(compteur))==1){
		map_set_cell_type(map,bomb->x , bomb->y+(compteur), CELL);
		compteur+=1;
	}
	compteur=0;
	while (bomb->area>=compteur && condition_explosion(map,bomb->x-(compteur),bomb->y)==1){
		map_set_cell_type(map,bomb->x-(compteur) , bomb->y, CELL);
		compteur+=1;
	}
	compteur=0;
	while (bomb->area>=compteur && condition_explosion(map,bomb->x,bomb->y-(compteur))==1){
		map_set_cell_type(map,bomb->x , bomb->y-(compteur), CELL);
		compteur+=1;	
	}
		
}


void bomb_display(struct map * map)
{
	//initialise le tableau de bombe seulement au premier passage
	if (verification_init==0)
	{
		verification_init+=1;
		initialisation_tab_bomb();
	}

	for(int i=0;i<9;i++)
	{

		if (tab_bomb[i].used!=0)
		{
			int tempsActuel = SDL_GetTicks();
			int temps_vie = tempsActuel-tab_bomb[i].time;

			if (temps_vie<1000)
			{
				map_set_cell_type(map,tab_bomb[i].x , tab_bomb[i].y, CELL_BOMB_1);
			}
			if ( temps_vie>1000 &&  temps_vie<2000)
			{
				map_set_cell_type(map,tab_bomb[i].x , tab_bomb[i].y, CELL_BOMB_2);
			}
			if ( temps_vie>2000 &&  temps_vie<3000)
			{
				map_set_cell_type(map,tab_bomb[i].x , tab_bomb[i].y, CELL_BOMB_3);
			}
			if ( temps_vie>3000 &&  temps_vie<4000)
			{
				map_set_cell_type(map,tab_bomb[i].x , tab_bomb[i].y, CELL_BOMB_4);
			}
			if( temps_vie >4000 && temps_vie< 5000)
			{
				explosion(&tab_bomb[i], map,CELL_EXPLOSION);
			}
			if( temps_vie >5000 && temps_vie< 6000)
			{
				explosion(&tab_bomb[i], map,CELL_EMPTY);
				tab_bomb[i].used=0;
			}
		}
	}

	
}







///////////////// deposition de bombe p
void put_bomb(struct player* player,struct map* map)
{

	if (player_get_nb_bomb(player)==0)
	{
		return ;
	}
	else
	{
		//acces a une nouvelle case du tableau
		int i=0;
		while(tab_bomb[i].used!=0 && i<9)
		{
			i+=1;
		}
		tab_bomb[i]=bomb_init(&tab_bomb[i],2,player);
		player_dec_nb_bomb(player);

		return ;
	}

}