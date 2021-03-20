#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>

void bonus_effect(struct player *player, unsigned char type)
{
    assert(player);
    switch (type & 0x0f)
    {
    case BONUS_BOMB_NB_INC:
        player_inc_nb_bomb(player);
        break;
    case BONUS_BOMB_NB_DEC:
        player_dec_nb_bomb(player);
        break;
    case BONUS_BOMB_RANGE_INC:
        player_inc_range_bomb(player);
        break;
    case BONUS_BOMB_RANGE_DEC:
        player_dec_range_bomb(player);
        break;
    case BONUS_LIFE:
        player_inc_life(player);
        break;
    default:
        break;
    }
}