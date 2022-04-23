#include <lexer.h>





struct TokenStream* token_stream_new() {
  return NULL;

}
void token_stream_drop(struct TokenStream* self) {


}

void lexer_new(struct Lexer* self, char* fname);
void lexer_lex(struct Lexer* self);
