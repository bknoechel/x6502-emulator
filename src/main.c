#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

#include "loader.h"

int main(int argc, char *argv[]) {
  // Hardout output files
  load_from_paths("a.o65", NULL);

  printf("\n");
  return 0;
}