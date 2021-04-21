/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef MONSTER_H_
#define MONSTER_H_

#include <constant.h>
#include <player.h>

struct monster;

// Creates a new monster with a given number of available bombs
struct monster* monster_init(int s);
void monster_free(struct monster* monster[], struct map* map, int *nb_monster);

void monster_load(struct monster* monster[], struct map* map, int* nb_monster);

// Set the position of the monster
void monster_set_position(struct monster *monster, int x, int y);
int monster_on_pos(struct monster* monster[], int nb_monster, int x, int y);

// Returns the current position of the monster
int monster_get_x(struct monster* monster);
int monster_get_y(struct monster* monster);

// Set the direction of the next move of the monster
void monster_set_current_way(struct monster * monster, enum direction direction);

// Move the monster according to the current direction
int monster_move(struct monster* monster, struct map* map);

void monster_random(struct monster* monster, struct map* map);
int monster_kill(struct monster* monster, struct map * map);
void monster_dammage(struct player* player, struct monster* monster);

// Display the monster on the screen
void monster_display(struct monster* monster, struct map* map, struct player* player);

#endif /* monster_H_ */
