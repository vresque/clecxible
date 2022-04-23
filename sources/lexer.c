#include <lexer.h>
#include <fs.h>
#include <location.h>

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

enum TokenType token_types[] = {
  ['>'] = TOKEN_GT,
    ['<'] = TOKEN_LT,
    ['+'] = TOKEN_PLUS,
    ['-'] = TOKEN_MINUS,
    ['*'] = TOKEN_MUL,
    ['/'] = TOKEN_DIV,
    ['%'] = TOKEN_MOD,
    ['='] = TOKEN_ASSIGN,
    ['#'] = TOKEN_HASHTAG,
    [':'] = TOKEN_COLON,
    [';'] = TOKEN_SEMI,
    ['['] = TOKEN_LBRACKET,
    [']'] = TOKEN_RBRACKET,
    ['('] = TOKEN_LPAREN,
    [')'] = TOKEN_RPAREN,
    ['{'] = TOKEN_LBRACE,
    ['}'] = TOKEN_RBRACE,
    ['\''] = TOKEN_SINGLE_QUOTE,
    ['"'] = TOKEN_DOUBLE_QUOTE,
    [','] = TOKEN_COMMA,
    ['.'] = TOKEN_DOT,
    ['&'] = TOKEN_AND,
    ['|'] = TOKEN_OR,
    ['~'] = TOKEN_BNEG,
    [ 'A' ... 'Z'] = TOKEN_IDENTIFIER,
    [ 'a' ... 'z'] = TOKEN_IDENTIFIER,
    ['_'] = TOKEN_IDENTIFIER,
    ['$'] = TOKEN_IDENTIFIER,
    ['0' ... '9'] = TOKEN_INTEGER
};
  void advance(struct Lexer* self) {
    self->index++;
  }

char next(struct Lexer* self) {
  advance(self);
  if (self->index < self->content_len) {
    return self->contents[self->index];
  }
  return 0;
}

struct Location this_loc(struct Lexer* self) {
  return (struct Location){
    .file_name = self->fname,
    .line_as_string = &self->contents[self->index],
    .line_length = 10, // TODO: Change
    .line_number = self->line,
    .column_number = (self->index / self->line),
  };
}

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
  read_to_string(fname, &self->content_len);
}

void lexer_lex(struct Lexer* self) {
  char chr;
  while ( (chr = next(self)) ) {
    enum TokenType type = token_types[(usize)chr];
    switch (type) {
    case TOKEN_IDENTIFIER:
    case TOKEN_INTEGER:
    case TOKEN_SINGLE_QUOTE:
    case TOKEN_DOUBLE_QUOTE:
    default: {
      struct Token tok =  {
	.ty = type,
	.loc = this_loc(self),
	.start = self->contents + self->index,
	.token_len = 1,
	  };
      UNUSED(tok)
    }
  }
}
}

void lexer_drop(struct Lexer* self) {
  token_stream_drop(self->stream);
  free(self->fname);
  free(self->contents);
  free(self);
}
