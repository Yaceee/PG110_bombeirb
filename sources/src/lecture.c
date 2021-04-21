#include <stdlib.h>
#include <stdio.h>

#define SIZE 10
int char_to_int(char c)
{
	int val=c;
	return val-48;
}

int power(int a, int b)
{
	if(b==0)
	{
		return 1;
	}
	for(int i = 0;i<b;i++)
	{
		a *= a;
	}

	return a;
}

void print_tab(int * tab)
{
	printf("\n");
	for (int j=0;j<12;j++)
	{
		for (int i=0;i<12;i++)
		{
			printf("%d |",tab[i+12*j]);

		}
		printf("\n");
	}
}

int read_world_file(char * src, int* nb_map, int* start_map, int* x, int* y, char* name)
{
	FILE * fp;
	int tab[SIZE];

	fp = fopen(src, "r");
	if (fp==NULL)
	{
		printf("erreur\n");
		return 0;
	}

	char c = fgetc(fp);
	int i = 0;
	while (c>=48 && c<=57)
	{
		tab[i] = char_to_int(c);
		i++;
		c = fgetc(fp);
	}
	for(int j = 0;j<i+1;j++)
	{
		*nb_map += tab[j]*power(10,i-j-1);
	}

	c = fgetc(fp);
	c = fgetc(fp);
	i = 0;
	while (c>=48 && c<=57)
	{
		tab[i] = char_to_int(c);
		i++;
		c = fgetc(fp);
	}
	for(int j = 0;j<i+1;j++)
	{
		*start_map += tab[j]*power(10,i-j-1);
	}

	c = fgetc(fp);
	i = 0;
	while (c>=48 && c<=57)
	{
		tab[i] = char_to_int(c);
		i++;
		c = fgetc(fp);
	}
	for(int j = 0;j<i+1;j++)
	{
		*x += tab[j]*power(10,i-j-1);
	}

	c = fgetc(fp);
	i = 0;
	while (c>=48 && c<=57)
	{
		tab[i] = char_to_int(c);
		i++;
		c = fgetc(fp);
	}
	for(int j = 0;j<i+1;j++)
	{
		*y += tab[j]*power(10,i-j-1);
	}

	c = fgetc(fp);
	fgets(name, 5, fp);

	return 1;
}

int * read_size_map(char * name_map)
{
	int * size;
	size = malloc(2*sizeof(int));
	FILE * fp;

	fp = fopen(name_map, "r");
	if (fp==NULL)
	{
		printf("erreur\n");
		return 0;
	}


	//definir la taille de la map
	int i=0;
	int tab[SIZE];
	char c=fgetc(fp);
	while (c>=48 && c<=57)
	{
		tab[i]=char_to_int(c);
		i+=1;
		c=fgetc(fp);
	}
	if (i==1)
	{
		size[0]=tab[0];	
	}
	if (i==2)
	{
		size[0]=tab[0]*10+tab[1];	
	}
	c=fgetc(fp);
	i=0;
	while (c>=48 && c<=57)
	{
		tab[i]=char_to_int(c);
		i+=1;
		c=fgetc(fp);
	}
	if (i==1)
	{
		size[1]=tab[0];	
	}
	if (i==2)
	{
		size[1]=tab[0]*10+tab[1];	
	}

	return size;
}

int * write_map(char * name_map)
{
	FILE * fp;

	fp = fopen(name_map, "r");
	if (fp==NULL)
	{
		printf("erreur\n");
		return 0;
	}


	//definir la taille de la map
	int i=0;
	int tab[SIZE];
	int compteur=0;
	char c=fgetc(fp);
	int map_height=0;
	int map_width=0;
	while (c>=48 && c<=57)
	{
		tab[i]=char_to_int(c);
		i+=1;
		c=fgetc(fp);
	}
	if (i==1)
	{
		map_width=tab[0];	
	}
	if (i==2)
	{
		map_width=tab[0]*10+tab[1];	
	}
	c=fgetc(fp);
	i=0;
	while (c>=48 && c<=57)
	{
		tab[i]=char_to_int(c);
		i+=1;
		c=fgetc(fp);
	}
	if (i==1)
	{
		map_height=tab[0];	
	}
	if (i==2)
	{
		map_height=tab[0]*10+tab[1];	
	}
	int * themap=malloc(map_height*map_width*sizeof(int));


	//faire le tableau de la map

	while (c != EOF)
	{		
		i=0;
		while (c>=48 && c<=57){
			tab[i]=char_to_int(c);
			i+=1;
			c=fgetc(fp);
		}
		if (i==1)
		{
			themap[compteur]=tab[0];
			tab[0]=0;
			compteur+=1;	
		}
		if (i==2)
		{
			themap[compteur]=tab[0]*10+tab[1];
			tab[0]=0;
			tab[1]=0;
			compteur+=1;		
		}
		if (i==3)
		{
			themap[compteur]=tab[0]*100+tab[1]*10+tab[2];
			tab[0]=0;
			tab[1]=0;	
			tab[2]=0;	
			compteur+=1;		
		}
		c=fgetc(fp);
	}
	
	return themap;
}



/*int main(int argc, char const *argv[])
{
	int * map=write_map("map1.txt");
	print_tab(map);
	return 0;
}
*/













