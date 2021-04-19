#include <SDL/SDL_image.h>
#include <assert.h>
#include <player.h>
#include <lecture.h>
#include <map.h>

void player_on_doors(struct player* player, struct map * map)
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
			printf("passage porte ouverte\n");
			int * map_test;
			switch(map_get_cell(map,player_get_x(player),player_get_y(player)) & 0x0e)
			{
				case 0x00:
					printf("passage dans 0\n");
					map_test=write_map("./src/map_0.txt");
					break;	
				case 0x02:
					printf("passage dans 1\n");
					map_test=write_map("./src/map_1.txt");
					break;
				case 0x04:
					map_test=write_map("./src/map_2.txt");
					break;	
				case 0x06:
					map_test=write_map("./src/map_3.txt");
					break;
				case 0x08:
					map_test=write_map("./src/map_4.txt");
					break;	
				case 0x0a:
					map_test=write_map("./src/map_5.txt");
					break;
				case 0x0c:
					map_test=write_map("./src/map_6.txt");
					break;	
				case 0x0e:
					map_test=write_map("./src/map_7.txt");
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

































