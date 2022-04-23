#include <fs.h>
#include <stdio.h>
#include <stdlib.h>

char* read_to_string(char* fname) {
  FILE* f = fopen(fname, "rb");
  if (!f) {
    die("failed to open file: %s", fname);
  }
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  rewind(f);
  char* contents = malloc(size + 1);
  fread(contents, size, 1, f);
  fclose(f);
  contents[size] = 0;
  return contents;
}
