#include <core.h>
#include <argument.h>
#include <fs.h>

int main(int argc, char** argv) {
  struct Arguments args = parse_args(argc, argv);
  printf("%s", read_to_string(args.filepath));
  UNUSED(args);
}
