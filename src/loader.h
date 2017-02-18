#ifndef LOADER
#define LOADER

#include <stdio.h>

void load_from_paths(char* file_x6502, char* file_out);

void load_from_files(FILE* fp_x6502, FILE* fp_out);

#endif