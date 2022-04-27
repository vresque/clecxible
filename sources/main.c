#include <argument.h>
#include <core.h>
#include <fs.h>
#include <lexer.h>

int main(int argc, char **argv) {
  struct Arguments args = parse_args(argc, argv);
  struct Lexer lexer = {0};
  lexer_new(&lexer, args.filepath);
  lexer_lex(&lexer);
  lexer_dump(&lexer);
}
