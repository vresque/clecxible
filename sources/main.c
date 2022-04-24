#include <core.h>
#include <argument.h>
#include <fs.h>
#include <vec.h>

int main(int argc, char** argv) {
  struct Arguments args = parse_args(argc, argv);
  //printf("%s", read_to_string(args.filepath, NULL));
  UNUSED(args);
  Vec(int) my_vec = vec_new(int, 150);
  printf("Still here: %p\n", my_vec);
  for (int i = 0; i < 400; i++) {
    puts("lmao");
    vec_push_one(my_vec, i);
  }
}
