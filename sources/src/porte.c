#include <SDL/SDL_image.h>
#include <assert.h>
#include <player.h>
#include <lecture.h>
#include <game.h>

void player_on_doors(struct player* player, struct map * map, struct game* game)
{
	if (map_get_cell_type(map,player_get_x(player),player_get_y(player))==CELL_DOOR)
	{
		enum compose_type type=map_get_cell(map,player_get_x(player),player_get_y(player)) & 0x01;
		if((type==0x00) & (player_get_key>0)) 
		{
			map_set_cell(map,player_get_x(player),player_get_y(player),CELL_DOOR_OPEN);
			player_dec_key(player);
		}
		
		if(type== 0x01 )
		{
			int * map_test;
			switch(map_get_cell(map,player_get_x(player),player_get_y(player)) & 0x0e)
			{
				case 0x00:
					game_change_level(game, 0);
					break;	
				case 0x02:
					game_change_level(game, 1);
					break;
				case 0x04:
					game_change_level(game, 2);
					break;	
				case 0x06:
					game_change_level(game, 2);
					break;
				case 0x08:
					game_change_level(game, 2);
					break;	
				case 0x0a:
					game_change_level(game, 2);
					break;
				case 0x0c:
					game_change_level(game, 2);
					break;	
				case 0x0e:
					game_change_level(game, 2);
					break;
			}
			for (int i = 0; i < 12;i++)
			{
				//printf("passage bpucle\n");
				for (int j = 0; j< 12; j++)
				{
					map_set_cell_type(map,i,j, map_test[i+j*map_get_width(map)]);
				}
			}
			
		}
	}
}

































