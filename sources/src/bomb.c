#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#define SIZE 9
#define SIZEBOMB 108
#define SIZEAREA 12 //12 est le nombre max de case detruite par bomb


//////////////
//structures//
//////////////
struct bomb {
	int x, y;
	int area;
	int time;
	int reaction;
	int used; ///// toutes les structures sont init et on regarde juste si les bombes sont utilisées 
};

struct CellBeforeExplosion
{
	int x, y;
	enum compose_type Cell_type;
	int used;
};

//////////////////////
//variables globales//
//////////////////////
int verification_init=0;
struct bomb tab_bomb[SIZE]; //tableau des bombes deja posées
struct CellBeforeExplosion tab_cell[SIZEBOMB];

/////////////
//fonctions//
/////////////

struct bomb bomb_init(struct bomb * bomb ,int area, struct player * player) {
	bomb->area = area;
	bomb->x = player_get_x(player);
	bomb->y = player_get_y(player);
	bomb->time = ticks;
	bomb->reaction = 0;
	bomb->used = 1;
	return *bomb;
}

struct CellBeforeExplosion cell_init(struct CellBeforeExplosion * cell, int x, int y ,enum compose_type cellule)
{
	cell->x=x;
	cell->y=y;
	cell-> Cell_type=cellule;
	cell->used=1;
	return * cell;
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
	bomb->reaction =0;
	bomb->used = 0;
	return bomb;
}

struct CellBeforeExplosion * cell_init_for_tab() {
	struct CellBeforeExplosion* cell = malloc(sizeof(*cell));
	if (!cell)
		error("Memory error");

	cell->x=0;
	cell->y=0;
	cell->Cell_type=0x00;
	cell->used =0;
	return cell;
}

//// Permet d'initialiser le tableau avec des structs bomb au debut
void initialisation_tab_bomb()
{
	for(int i=0 ;i<SIZE; i++)
	{
		struct bomb * bomb=bomb_init_for_tab();
		tab_bomb[i]=*bomb;
	}
}

void initialisation_tab_cell()
{
	for(int i=0 ;i<SIZEBOMB; i++)
	{
		struct CellBeforeExplosion * cell=cell_init_for_tab();
		tab_cell[i]=*cell;
	}
}

void declanchement_bomb_reaction(struct map * map,int x ,int y)
{
	for(int i=0;i<SIZE;i++)
	{

		if (tab_bomb[i].x==x && tab_bomb[i].y==y && tab_bomb[i].reaction==0)
		{
			tab_bomb[i].reaction=1;
			tab_bomb[i].time=ticks-4000;
		}
	}
	
}

int condition_explosion(struct map * map,int x ,int y){
	if(map_is_inside(map,x,y)){// si dans le terrain
		if(map_get_cell_type(map,x,y)==0 || map_get_cell_type(map,x,y)==32  ){ // si c'est cassable
			return 1;
		}
		if ((map_get_cell_type(map,x,y) & 0xf0 )== CELL_BOMB)
		{
			declanchement_bomb_reaction(map,x,y);
			return 1;
		}
	}
	return 0;
}


void explosion(struct bomb * bomb, struct map * map, enum cell_type CELL,int indice_tab)
{
	bomb->reaction=1;
	int j=indice_tab*SIZEAREA;
	int k=0;
	int compteur=0;
	while (bomb->area>=compteur && condition_explosion(map,bomb->x+compteur,bomb->y)!=0){
		int i=0;
		while(tab_cell[j+i+k].used!=0 && i<SIZEAREA)
		{
			i+=1;
		}
		if ( map_get_cell(map,bomb->x+(compteur), bomb->y)!= CELL_EXPLOSION)// Pour pas qu'il garde en memoire la CELL_EXPLOSION
		{
			tab_cell[j+i+k]=cell_init(&tab_cell[j+i+k],bomb->x+(compteur) , bomb->y, map_get_cell(map,bomb->x+(compteur) , bomb->y));
		}
		map_set_cell_type(map,bomb->x+(compteur) , bomb->y, CELL);
		compteur+=1;
	}
	k=3;
	compteur=0;
	while (bomb->area>=compteur && condition_explosion(map,bomb->x,bomb->y+(compteur))!=0){
		int i=0;
		while(tab_cell[j+i+k].used!=0 && i<SIZEAREA)
		{
			i+=1;
		}
		if ( map_get_cell(map,bomb->x, bomb->y+(compteur))!= CELL_EXPLOSION)
		{
			tab_cell[j+i+k]=cell_init(&tab_cell[j+i+k],bomb->x , bomb->y+compteur, map_get_cell(map,bomb->x , bomb->y+(compteur)));
		}
		map_set_cell_type(map,bomb->x , bomb->y+(compteur), CELL);
		compteur+=1;
	}
	k=6;
	compteur=0;
	while (bomb->area>=compteur && condition_explosion(map,bomb->x-(compteur),bomb->y)!=0){
		int i=0;
		while(tab_cell[j+i+k].used!=0 && i<SIZEAREA)
		{
			i+=1;
		}
		if ( map_get_cell(map,bomb->x-(compteur), bomb->y)!= CELL_EXPLOSION)// Pour pas qu'il garde en memoire la CELL_EXPLOSION
		{
			tab_cell[j+i+k]=cell_init(&tab_cell[j+i+k],bomb->x-(compteur) , bomb->y, map_get_cell(map,bomb->x-(compteur) , bomb->y));
		}
		map_set_cell_type(map,bomb->x-(compteur) , bomb->y, CELL);
		compteur+=1;
	}
	k=9;
	compteur=0;
	while (bomb->area>=compteur && condition_explosion(map,bomb->x,bomb->y-(compteur))!=0){
		int i=0;
		while(tab_cell[j+i+k].used!=0 && i<SIZEAREA)
		{
			i+=1;
		}
		if ( map_get_cell(map,bomb->x, bomb->y-(compteur))!= CELL_EXPLOSION)
		{
			tab_cell[j+i+k]=cell_init(&tab_cell[j+i+k],bomb->x , bomb->y-compteur, map_get_cell(map,bomb->x , bomb->y-(compteur)));
		}
		map_set_cell_type(map,bomb->x , bomb->y-(compteur), CELL);
		compteur+=1;	
	}
}

void explosion_suite(struct map * map, struct CellBeforeExplosion * tab_cell ,int indice_tab)
{
	int j=indice_tab*SIZEAREA;
	
	for (int i=0;i<SIZEAREA;i++) //12 est le nombre max de case detruite
	{
		
		if(tab_cell[j+i].used!=0)
		{
			switch (tab_cell[j+i].Cell_type & 0xf0)
			{
				case CELL_BOX:
					switch (tab_cell[j+i].Cell_type & 0x0f)
					{
						case BONUS_BOMB_RANGE_DEC:
							map_set_cell_type(map,tab_cell[j+i].x,tab_cell[j+i].y,CELL_RANGEDEC);
							break;
						case BONUS_BOMB_RANGE_INC:
							map_set_cell_type(map,tab_cell[j+i].x,tab_cell[j+i].y,CELL_RANGEINC);
							break;
						case BONUS_BOMB_NB_INC:
							map_set_cell_type(map,tab_cell[j+i].x,tab_cell[j+i].y,CELL_BOMBINC);
							break;
						case BONUS_BOMB_NB_DEC:
							map_set_cell_type(map,tab_cell[j+i].x,tab_cell[j+i].y,CELL_BOMBDEC);
							break;
						case BONUS_LIFE:
							map_set_cell_type(map,tab_cell[j+i].x,tab_cell[j+i].y,CELL_LIFE);
							break;
						case BONUS_MONSTER:
							map_set_cell_type(map,tab_cell[j+i].x,tab_cell[j+i].y,CELL_MONSTER);
							break;
						default:
							map_set_cell_type(map,tab_cell[j+i].x,tab_cell[j+i].y,CELL_EMPTY);
					}
					break;
				default:
					map_set_cell_type(map,tab_cell[j+i].x,tab_cell[j+i].y,CELL_EMPTY);
					break;
				
				
			}
			tab_cell[j+i].used=0;
		}
	}
}

void bomb_display(struct map * map)
{
	//initialise le tableau de bombe seulement au premier passage
	if (verification_init==0)
	{
		verification_init+=1;
		initialisation_tab_bomb();
		initialisation_tab_cell();
	}

	for(int i=0;i<9;i++)
	{

		if (tab_bomb[i].used!=0)
		{
			int tempsActuel = ticks;
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
				explosion(&tab_bomb[i], map,CELL_EXPLOSION,i);
			}
			if( temps_vie >5000 && temps_vie< 6000)
			{
				explosion_suite(map,tab_cell,i);
				tab_bomb[i].used=0;
			}
		}
	}
	
}




int explosion_dead(struct map* map, int x,int y)
{
	if (map_get_cell(map,x, y)==CELL_EXPLOSION)
	{
		return 0;
	}
	return 1;
}






///////////////// deposition de bombe p
void put_bomb(struct player* player,struct map* map)
{

	if (player_get_nb_bomb(player)==0 ||map_get_cell_type(map,player_get_x(player),player_get_y(player))==CELL_DOOR)
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
		tab_bomb[i]=bomb_init(&tab_bomb[i],player_get_range_bomb(player),player);
		player_dec_nb_bomb(player);

		return ;
	}

}