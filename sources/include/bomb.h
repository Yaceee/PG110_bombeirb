#ifndef BOMB_H_
#define BOMB_H_

#include <map.h>
#include <constant.h>
#include <player.h>
#include <game.h>
#include <game.h>


struct bomb;

// Creates a new bomb with a given range
struct bomb* bomb_init(int range);
void   bomb_free(struct bomb *bomb);

//change l'etat des bombes
void bomb_display(struct map * map);

//gere les explosions
int condition_explosion(struct map * map,int x ,int y);
void explosion(struct bomb * bomb, struct map * map, enum cell_type CELL);
struct bomb* bomb_init_for_tab();
void initialisation_tab_bomb();


//main
void put_bomb(struct player* player,struct map* map);

#endif /* PLAYER_H_ */
