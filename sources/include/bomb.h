#ifndef BOMB_H_
#define BOMB_H_

#include <constant.h>
#include <player.h>
#include <game.h>


struct bomb;
struct CellBeforeExlosion;

// Creates a new bomb with a given range
struct bomb* bomb_init(struct bomb * bomb ,int area, struct player * player);
struct bomb* bomb_init_for_tab();
void initialisation_tab_bomb();
void   bomb_free(struct bomb *bomb);


struct Cell_before_explosion cell_init(int x, int y ,enum compose_type cellule);
struct Cell_before_explosion * cell_init_for_tab();
void declanchement_bomb_reaction(struct map * map,int x ,int y);
void initialisation_tab_cell();
void explosion_suite(struct map * map, struct Cell_before_explosion tab_cell); //modifie la map apres l'explosion


//change l'etat des bombes
void bomb_display(struct map * map);

//gere les explosions
int condition_explosion(struct map * map,int x ,int y);
void explosion(struct bomb * bomb, struct map * map, enum cell_type CELL);



int explosion_dead(struct map* map, int x,int y);
//main
void put_bomb(struct player* player,struct map* map);

#endif /* PLAYER_H_ */
