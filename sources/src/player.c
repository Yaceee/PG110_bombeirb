/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include "constant.h"
#include <bonus.h>
#include <bomb.h>

int Ancien_temps=0;

struct player {
	int x, y;
	enum direction direction;
	int bombs;
	int bomb_range;
	int life;
};

struct player* player_init(int bombs, int life) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->direction = NORTH;
	player->bomb_range = 1;
	player->bombs = bombs;
	player->life = life;

	return player;
}


void player_set_position(struct player *player, int x, int y) {
	assert(player);
	player->x = x;
	player->y = y;
}


void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->direction = way;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->bombs;
}

int player_get_range_bomb(struct player* player)
{
	assert(player);
	return player->bomb_range;
}

int player_get_nb_life(struct player* player)
{
	assert(player);
	return player->life;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->bombs += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	player->bombs -= 1;
}

void player_inc_range_bomb(struct player* player)
{
	assert(player);
	player->bomb_range += 1;
}

void player_dec_range_bomb(struct player* player)
{
	assert(player);
	if (player->bomb_range > 1)
	{
		player->bomb_range -= 1;
	}
}

void player_inc_life(struct player* player)
{
	assert(player);
	player->life += 1;
}

void player_dec_life(struct player* player)
{
	assert(player);
	player->life -= 1;
}

static int player_move_aux(struct player* player, struct map* map, int x, int y, int dir) { //ajout direction en entrée pour la gestion des boites

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0; // pas de déplacement possible si décor
		break;

	case CELL_BOX:
		return box_collsion(map, x, y, dir); //retourne résultat fontion collsion des boites
		break;

	case CELL_BONUS:
		bonus_effect(player, map_get_cell(map, x, y));
		map_set_cell_type(map, x, y, CELL_EMPTY);
		return 1;
		break;

	case CELL_MONSTER:
		break;

	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->direction) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1, NORTH)) {
			player->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1, SOUTH)) {
			player->y++;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y, WEST)) {
			player->x--;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y, EAST)) {
			player->x++;
			move = 1;
		}
		break;
	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
	}
	return move;
}

int invulnerability()
{
	int tempsActuel = ticks;
	if (tempsActuel-Ancien_temps<1000)
	{
		return 1;
	}
	return 0;
}

void adjust_Ancien_temps()
{
	Ancien_temps = ticks;
}

void player_dammage(struct player* player, struct map * map)
{
	
	if (invulnerability()==0)//invulnerabilité du personnage pendant 1 seconde
	{
		if (explosion_dead(map,player->x,player->y)==0) // si il se trouve sur une bombe
		{
			adjust_Ancien_temps();
			player_dec_life(player);
		}
	}
}

void player_display(struct player* player, struct map * map) {
	assert(player);
	player_dammage(player,map);
	window_display_image(sprite_get_player(player->direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}

