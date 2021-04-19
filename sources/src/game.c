/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>
#include <bomb.h>

int p = 0;

struct game {
	struct map** maps;       // the game's map
	short levels;        // nb maps of the game
	short level;
	struct player* player;
	struct monster* monster[100];
	int nb_monster;
};

struct game*
game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->maps = malloc(sizeof(struct game));
	game->maps[0] = map_get_static();
	game->levels = 1;
	game->level = 0;
	game->nb_monster = 0;

	game->player = player_init(3, 3);
	// Set default location of the player
	player_set_position(game->player, 1, 0);

	monster_load(game->monster,game->maps[0],&game->nb_monster);

	return game;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	for (int i = 0; i < game->levels; i++)
		map_free(game->maps[i]);
}

struct map* game_get_current_map(struct game* game) {
	assert(game);
	return game->maps[game->level];
}


struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = game_get_current_map(game);

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
	int x = 0;//white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = 0* white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_life(game_get_player(game))), x, y);

	x = 1 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 1 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
	sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 2 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 2 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_range_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 6 * SIZE_BLOC;
	window_display_image(sprite_get_banner_key(), x, y);

	x = 3 * white_bloc + 7 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_key(game_get_player(game))), x, y);
}

void game_display(struct game* game) {
	assert(game);

	window_clear();
	game_banner_display(game);
	map_display(game_get_current_map(game));
	bomb_display(game_get_current_map(game));

	player_display(game->player, game_get_current_map(game));
	for(int i = 0;i<game->nb_monster;i++)
	{
		monster_display(game->monster[i], game_get_current_map(game), game->player);
		
		if(monster_kill(game->monster[i],game_get_current_map(game)))
		{
			for(int j = i;j<game->nb_monster;j++){
				game->monster[j] = game->monster[j+1];
			}
			game->nb_monster -= 1;
		}
		
	}
	window_refresh();
}

int game_get_pause()
{
	return p;
}

static short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = game_get_current_map(game);


	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_p:
				if(!p)
				{
					p = 1;
				}
				else
				{
					p = 0;
					delay = SDL_GetTicks() - ticks;
				}
				break;
			case SDLK_ESCAPE:
				if(!p)
				{
					return 1;
				}
			case SDLK_UP:
				if(!p){
					player_set_current_way(player, NORTH);
					player_move(player, map);	
				}
				break;
			case SDLK_DOWN:
				if(!p){
					player_set_current_way(player, SOUTH);
					player_move(player, map);
				}
				break;
			case SDLK_RIGHT:
				if(!p){
					player_set_current_way(player, EAST);
					player_move(player, map);
				}
				break;
			case SDLK_LEFT:
				if(!p){
					player_set_current_way(player, WEST);
					player_move(player, map);
				}
				break;
			case SDLK_SPACE:
				if(!p){
					put_bomb(player,map);
				}
				break;
			default:
				break;
			}

			break;
		}
	}
	return 0;
}

int game_update(struct game* game) {
	if (input_keyboard(game) || player_get_nb_life(game->player) <1)
		return 1; // exit game

	return 0;
}
