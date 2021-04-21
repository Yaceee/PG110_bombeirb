
/*******************************************************************************
 * This file is part of Bombeirb.
 * Copyright (C) 2018 by Laurent Réveillère
 ******************************************************************************/
#include <SDL/SDL.h>

#include <constant.h>
#include <game.h>
#include <window.h>
#include <sprite.h>
#include <misc.h>
#include <homepage.h>
#include <player.h>

int ticks;
int delay = 0;
int isHomepage = 0;
int inGame = 0;

int main(int argc, char *argv[]) {

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		error("Can't init SDL:  %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	window_create(SIZE_BLOC * STATIC_MAP_WIDTH,
	SIZE_BLOC * STATIC_MAP_HEIGHT + BANNER_HEIGHT + LINE_HEIGHT);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	int ideal_speed = 1000 / DEFAULT_GAME_FPS;
	int timer, execution_speed;

	// game loop
	// static time rate implementation
	int done = 0;
	while (!done) {
		
		sprite_load();
		while(!isHomepage)
		{
			timer = SDL_GetTicks();
			isHomepage = homepage_update();
			ticks = timer;
			homepage_display();

			execution_speed = SDL_GetTicks() - timer;
			if (execution_speed < ideal_speed)
				SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
		}

		if(choice == 0)
		{
			int nb_map = 0, start_map = 0, x = 0, y = 0;
			char* name = malloc(5*sizeof(char));
			read_world_file("./map/easy", &nb_map, &start_map, &x, &y, name);

			struct game* game = game_new(nb_map, name, x, y, start_map);
			while(!inGame)
			{
				timer = SDL_GetTicks();
				
				inGame = game_update(game);
				if(!game_get_pause())
				{
					game_display(game);
					ticks = timer - delay;
				}

				if(game_get_pause())
				{
					window_display_image(sprite_get_pause(), 80 , 200);
					window_refresh();
				}

				execution_speed = SDL_GetTicks() - timer;
				if (execution_speed < ideal_speed)
					SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
			}
			isHomepage = 0;
			inGame = 0;
			game_free(game);
		}
		
		if(choice == 1)
		{
			isHomepage = 0;
		}		

		if(choice == 2)
		{
			done = 1;
		}
		
	}

	SDL_Quit();

	return EXIT_SUCCESS;
}


