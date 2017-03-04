#include <string.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include "../src/loader.h"

#define MAX_FILES 100
#define MAX_STR_BUFFER 1024

/*
* My dummy program to help run a few of my files in a row
*/
int main(int argc, char *argv[]) {
  DIR* dir;
  struct dirent *ent;
  char* file_name;
  char* file_ext;
  char sys_command[MAX_STR_BUFFER];
  char o65_name[MAX_STR_BUFFER];
  char o65_file[MAX_STR_BUFFER];
  char* file_list[MAX_FILES];
  int file_list_index = 0;

  // Get all the assembly files in test
  if ((dir = opendir("./test")) != NULL) {
    while ((ent = readdir(dir)) != NULL) {

      if (ent->d_type == DT_REG) {
        file_ext = strstr(ent->d_name, ".s");
        if (file_ext != NULL && strlen(file_ext) == 2 && strlen(ent->d_name) < MAX_STR_BUFFER) {
          file_list[file_list_index] = malloc(strlen(ent->d_name));
          strcpy(file_list[file_list_index], ent->d_name);
          file_list_index++;
        }
      }
    }
    closedir(dir);
  }

  int i = 0;
  int valid;
  FILE* fp_x6502;
  FILE* fp_out = fopen("./test/results.txt", "w");
  while (i < file_list_index) {
    file_name = file_list[i++];

    strncpy(o65_name, file_name, strlen(file_name) - 2);

    valid = snprintf(sys_command, MAX_STR_BUFFER, "xa ./test/%s -o ./test/%s.o65", file_name, o65_name);
    if (valid == MAX_STR_BUFFER || valid < 0) {
      return 1;
    }

    system(sys_command);

    snprintf(o65_file, MAX_STR_BUFFER, "./test/%s.o65", o65_name);
    fp_x6502 = fopen(o65_file, "r");

    fprintf(fp_out, "%s\n", file_name);
    load_from_files(fp_x6502, fp_out);
    fprintf(fp_out, "\n");
    fclose(fp_x6502);
  }
  fclose(fp_out);

  system("rm ./test/*.o65");
  system("cmp --silent ./test/results.txt ./test/expected.txt || echo \"Warning! Unexpected test output\"");
  return 0;
}