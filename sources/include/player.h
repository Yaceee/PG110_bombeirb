/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#ifndef PLAYER_H_
#define PLAYER_H_

#include <constant.h>
#include <monster.h>
#include <game.h>

struct player;

// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number, int life);
void   player_free(struct player* player);

// Set the position of the player
void player_set_position(struct player *player, int x, int y);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
int player_get_range_bomb(struct player* player);
int player_get_nb_life(struct player* player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);
void player_inc_range_bomb(struct player * player);
void player_dec_range_bomb(struct player * player);
void player_inc_life(struct player* player);
void player_dec_life(struct player* player);
int player_get_key(struct player* player);
void player_inc_key(struct player* player);
void player_dec_key(struct player* player);

// Move the player according to the current direction
int player_move(struct player* player, struct map* map, struct monster* monster[], int nb_monster);

int invulnerability();
void adjust_Ancien_temps();
void player_dammage(struct player* player, struct map * map);

// Display the player on the screen
void player_display(struct player* player, struct map * map, struct game* game);

#endif /* PLAYER_H_ */
