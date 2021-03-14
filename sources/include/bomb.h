#ifndef BOMB_H_
#define BOMB_H_

#include <map.h>
#include <constant.h>
#include <player.h>
#include <game.h>


struct bomb;

// Creates a new bomb with a given range
struct bomb* bomb_init(int range);
void   bomb_free(struct bomb *bomb);

// Set the position of the bomb

void put_bomb(struct player* player,struct map* map);

#endif /* PLAYER_H_ */
