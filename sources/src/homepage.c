#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>

int choice = 0;

void homepage_display()
{
    window_clear();
    window_display_image(sprite_get_homepage(), 0, 0);
    window_display_image(sprite_get_bomb(4), 45, 130+choice*90);
    window_refresh();
}

static short homepage_input_keyboard() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_UP:
                if(choice==0)
                {
                    choice = 2;
                }
                else{
                    choice -= 1;
                }
				break;
			case SDLK_DOWN:
				choice += 1;
                choice = choice%3;
				break;
			case SDLK_RETURN:
                return 1;
				break;
			default:
				break;
			}

			break;
		}
	}
	return 0;
}

int homepage_update() {
	if (homepage_input_keyboard())
		return 1; // exit game

	return 0;
}