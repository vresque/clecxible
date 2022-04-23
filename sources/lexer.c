#include <lexer.h>
#include <fs.h>

static const char utility_keywords[][KEYWORD_MAX] = {
  "_Alignas",
  "_Alignof",
  "_Atomic",
  "_Bool",
  "_Complex",
  "_Generic",
  "_Imaginary",
  "_Pragma",
  "_Noreturn",
  "_Static_assert",
  "_Thread_local",
  "_Typeof",
  "_Vector",
  "__cdecl",
  "__stdcall",
  "__declspec",
};

#define UTILITY_KEYWORDS_LEN (sizeof(utility_keywords) / KEYWORD_MAX)

static const char keywords[][KEYWORD_MAX] = {
  "while",
  "break",
  "case",
  "const",
  "continue",
  "volatile",
  "void",
  "unsigned",
  "union",
  "default",
  "do",
  "double",
  "else",
  "if",
  "enum",
  "extern",
  "typedef",
  "switch",
  "struct",
  "static",
  "sizeof",
  "signed",
  "float",
  "for",
  "goto",
  "inline",
  "int",
  "long",
  "register",
  "return",
  "short",
};

#define KEYWORDS_LEN (sizeof(keywords) / KEYWORD_MAX)


struct TokenStream* token_stream_new() {
  struct TokenStream* self = malloc(sizeof(struct TokenStream));
  self->tokens = malloc(sizeof(struct Vec));
  vec_new(self->tokens, TOKEN_INITIAL_CAP);
  self->token_index = 0;
  return self;
}

void token_stream_drop(struct TokenStream* self) {
  vec_drop(self->tokens);
  free(self);
}

enum TokenType get_corresponding_keyword_type(char* keyword, bool* match_found) {
  for (int idx = 0; idx < KEYWORDS_LEN;idx++) {
    if (strcmp(keyword, keywords[idx])) {
      *match_found = true;
      return (idx + 1) + TOKEN_KW_START;
    }
  }
  for (int idx = 0; idx < UTILITY_KEYWORDS_LEN; idx++) {
    if (strcmp(keyword, utility_keywords[idx])) {
      *match_found = true;
      return (idx + 1) + TOKEN_UTIL_START;
    }
  }
  *match_found = false;
  return TOKEN_INVAL;
}

void lexer_new(struct Lexer* self, char* fname) {
  if (self == NULL) {
    self = malloc(sizeof(struct Lexer*));
  }
  self->stream = token_stream_new();
  self->index = 0;
  self->line = 0;
  self->fname = fname;
  self->contents = read_to_string(fname);
}

void lexer_lex(struct Lexer* self) {
  char chr;
  while ( (chr = *self->contents++) ) {
    switch (chr) {

    }
  }
}

void lexer_drop(struct Lexer* self) {
  token_stream_drop(self->stream);
  free(self->fname);
  free(self->contents);
  free(self);
}
