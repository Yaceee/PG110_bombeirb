#ifndef LECTURE_H_
#define LECTURE_H_

#include <stdlib.h>
#include <stdio.h>

int char_to_int(char c);
void print_tab(int * tab);
int read_world_file(char * src, int* nb_map, int* start_map, int* x, int* y, char* name);
int * read_size_map(char * name_map);
int * write_map(char * name_map);

#endif