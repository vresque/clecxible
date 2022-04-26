#include <core.h>
#include <argument.h>
#include <fs.h>
#include <lexer.h>

int main(int argc, char** argv) {
  printf("abc\n");
  struct Arguments args = parse_args(argc, argv);
  //printf("%s", read_to_string(args.filepath, NULL));
  struct Lexer lexer = {0};
  lexer_new(&lexer, args.filepath);
  lexer_lex(&lexer);
  printf("yere\n");
  lexer_dump(&lexer);
  
}
