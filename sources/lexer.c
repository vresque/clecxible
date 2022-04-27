#include <lexer.h>
#include <fs.h>
#include <ctype.h>
#include <location.h>

#define tpush_simple(tok, str) tpush((struct Token){ .ty = tok, .loc = this_loc(), .start = str, .token_len = 1})
#define tpush(tok) vec_push_one(self->stream->tokens, tok)
#define tpush_str(tok, str, len) tpush((struct Token){ .ty = tok, .loc = this_loc(), .start = str, .token_len = len})

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

static const char token_str[][TOKEN_STR_MAX] = {
  [TOKEN_GT] = ">",
  [TOKEN_LT] = "<",
  [TOKEN_PLUS] = "+",
  [TOKEN_MINUS] = "-",
  [TOKEN_MUL] = "*",
  [TOKEN_DIV] = "/",
  [TOKEN_MOD] = "%",
  [TOKEN_ASSIGN] = "=",
  [TOKEN_HASHTAG] = "#",
  [TOKEN_COLON] = ":",
  [TOKEN_SEMI] = ";",
  [TOKEN_LBRACKET] = "[",
  [TOKEN_RBRACKET] = "]",
  [TOKEN_LPAREN] = "(",
  [TOKEN_RPAREN] = ")",
  [TOKEN_LBRACE] = "{",
  [TOKEN_RBRACE] = "}",
  [TOKEN_SINGLE_QUOTE] = "\'",
  [TOKEN_DOUBLE_QUOTE] = "\"",
  [TOKEN_COMMA] = ",",
  [TOKEN_DOT] = ".",

  [TOKEN_AND] = "&",
  [TOKEN_OR] = "|",
  [TOKEN_BNEG] = "~",
  
  [TOKEN_TYPE_START] = "<type_start>",
  [TOKEN_INTEGER] = "<int>",
  [TOKEN_FLOAT] = "<float>",
  [TOKEN_RANGE] = "<range>",
  [TOKEN_CHAR] = "<char>",
  [TOKEN_STRING] = "<string>",

  [TOKEN_IDENTIFIER] = "<Ident>",
  
  [TOKEN_UTIL_START] = "<UtilStart>",
  [TOKEN_UT_ALIGNAS] = "_Alignas",
  [TOKEN_UT_ALIGNOF] = "_Alignof",
  [TOKEN_UT_ATOMIC] = "_Atomic",
  [TOKEN_UT_BOOL] = "_Bool",
  [TOKEN_UT_COMPLEX] = "_Complex",
  [TOKEN_UT_GENERIC] = "_Generic",
  [TOKEN_UT_IMAGINARY] = "_Imaginary",
  [TOKEN_UT_PRAGMA] = "_Pragma",
  [TOKEN_UT_NORETURN] = "_Noreturn",
  [TOKEN_UT_STATIC_ASSERT] = "_Static_assert",
  [TOKEN_UT_THREAD_LOCAL] = "_Thread_local",
  [TOKEN_UT_TYPEOF] = "_Typeof",
  [TOKEN_UT_VECTOR] = "_Vector",
  [TOKEN_UT_CDECL] = "__cdecl",
  [TOKEN_UT_STDCALL] = "__stdcall",
  [TOKEN_UT_DECLSPEC] = "__declspec",

  [TOKEN_KW_START] = "<KwStart>",
  [TOKEN_KW_WHILE] = "while",
  [TOKEN_KW_BREAK] = "break",
  [TOKEN_KW_CASE] = "case",
  [TOKEN_KW_CONST] = "const",
  [TOKEN_KW_CONTINUE] = "continue",
  [TOKEN_KW_VOLATILE] = "volatile",
  [TOKEN_KW_VOID] = "void",
  [TOKEN_KW_UNSIGNED] = "unsigned",
  [TOKEN_KW_UNION] = "union",
  [TOKEN_KW_DEFAULT] = "default",
  [TOKEN_KW_DO] = "do",
  [TOKEN_KW_DOUBLE] = "double",
  [TOKEN_KW_ELSE] = "else",
  [TOKEN_KW_IF] = "if",
  [TOKEN_KW_ENUM] = "enum",
  [TOKEN_KW_EXTERN] = "extern",
  [TOKEN_KW_TYPEDEF] = "typedef",
  [TOKEN_KW_SWITCH] = "switch",
  [TOKEN_KW_STRUCT] = "struct",
  [TOKEN_KW_STATIC] = "static",
  [TOKEN_KW_SIZEOF] = "sizeof",
  [TOKEN_KW_SIGNED] = "signed",
  [TOKEN_KW_FLOAT] = "float",
  [TOKEN_KW_FOR] = "for",
  [TOKEN_KW_GOTO] = "goto",
  [TOKEN_KW_INLINE] = "inline",
  [TOKEN_KW_INT] = "int",
  [TOKEN_KW_LONG] = "long",
  [TOKEN_KW_REGISTER] = "register",
  [TOKEN_KW_RETURN] = "return",
  [TOKEN_KW_SHORT] = "short",

  [TOKEN_INVAL] = "<Invalid>",
  [FINAL_TOKEN] = "<Final>",
};

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
    ['0' ... '9'] = TOKEN_INTEGER,
    [ '\n' ] = TOKEN_NEWLINE,
    [ '\r' ] = TOKEN_DISCARD,
};
  void advance(struct Lexer* self) {
    self->index++;
    self->col++;
  }

bool is_valid_ident(const char* ident) {
  char c;
  while ( (c = *ident++) ) {
    if (!(is_valid_ident_char(c))) return false;
  }
  return true;
}

bool is_valid_ident_char(const char c) {
    bool cond_a = (c >= 'A' && c <= 'Z');
    bool cond_b = (c >= 'a' && c <= 'z');
    bool specials = (c == '$' || c == '_');
    return (cond_a || cond_b || specials);
}

char next(struct Lexer* self) {
  advance(self);
  if (self->index < self->content_len) {
    return self->contents[self->index];
  }
  return 0;
}

char peek(struct Lexer* self) {
  if (self->index + 1 < self->content_len) {
    return self->contents[self->index + 1];
  }
  return 0;
}

char this(struct Lexer* self) {
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
    .column_number = self->col, // TODO: Change
  };
}

struct TokenStream* token_stream_new() {
  struct TokenStream* self = malloc(sizeof(struct TokenStream));
  self->tokens = vec_new(struct Token, TOKEN_INITIAL_CAP);
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
  self->stream = token_stream_new();
  self->index = 0;
  self->line = 0;
  self->fname = fname;
  self->contents = read_to_string(fname, &self->content_len);
}

void skip_whitespace(struct Lexer* self) {
  if (isspace(this(self))) {
    advance(self);
  }
  while (true) {
  char chr = peek(self);
  if (chr && isspace(chr)) {
    advance(self);
  } else { break; }
  }
}

struct Token lex_ident(struct Lexer* self) {
  printf("hel\n");
  Vec(char) final_string = vec_new(char, 10);
  vec_push_one(final_string, this(self));
  char chr;
  while ( (chr = next(self)) && is_valid_ident_char(chr)) {
      vec_push_one(final_string, chr);
  }
  // Next will be called *always*, therefore, it will be called
  // One too many times in case of an invalid condition
  self->index--;

  vec_push_one(final_string, 0);
  printf("Found lexed ident: %s\n", final_string);

  struct Token t = (struct Token) {
    .loc = this_loc(self),
    .start = final_string,
    .token_len = vec_len(final_string),
    .ty = TOKEN_IDENTIFIER
  };
  printf("here\n");
  return t;
}

void lexer_lex(struct Lexer* self) {
  skip_whitespace(self);
  char chr;
  while ( (chr = next(self)) ) {
    enum TokenType type = token_types[(usize)chr];
    switch (type) {
    case TOKEN_IDENTIFIER: {
      tpush(lex_ident(self));
      break;
    }
    case TOKEN_NEWLINE: {
      self->line++;
      self->col = 0;
      break;
    }
    case TOKEN_INTEGER:
    case TOKEN_SINGLE_QUOTE:
    case TOKEN_DOUBLE_QUOTE:
    default: {
      printf("def %s\n", token_str[type]);
      struct Token tok =  {
        .ty = type,
        .loc = this_loc(self),
        .start =&self->contents[self->index],
        .token_len = 1,
	    };
      tpush(tok);
    }
  }
    skip_whitespace(self);
}
}

void lexer_drop(struct Lexer* self) {
  token_stream_drop(self->stream);
  free(self->fname);
  free(self->contents);
  free(self);
}

void lexer_dump(struct Lexer* self) {
  printf("Lexer: File %s\n", self->fname);
  FOR_EACH_VEC(self->stream->tokens, it, {
      printf("Token {\n\t[%s]: \n\t Len %lu;\n\t Line %d;\n\t Column %d;\n\t Payload: %.*s\n}\n", token_str[it.ty], it.token_len, it.loc.line_number, it.loc.column_number, (int) it.token_len, it.start);
  });
}
