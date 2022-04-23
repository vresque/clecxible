#pragma once
#include <core.h>
#include <location.h>

enum TokenType {
  TOKEN_GT = '>',
  TOKEN_LT = '<',
  TOKEN_PLUS = '+',
  TOKEN_MINUS = '-',
  TOKEN_MUL = '*',
  TOKEN_DIV = '/',
  TOKEN_MOD = '%',
  TOKEN_ASSIGN = '=',
  TOKEN_HASHTAG = '#',
  TOKEN_COLON = ':',
  TOKEN_SEMI = ';',
  TOKEN_LBRACKET = '[',
  TOKEN_RBRACKET = ']',
  TOKEN_LPAREN = '(',
  TOKEN_RPAREN = ')',
  TOKEN_LBRACE = '{',
  TOKEN_RBRACE = '}',
  TOKEN_SINGLE_QUOTE = '\'',
  TOKEN_DOUBLE_QUOTE = '"',
  TOKEN_COMMA = ',',
  TOKEN_DOT = '.',

  TOKEN_AND = '&',
  TOKEN_OR = '|',
  TOKEN_NEG = '~',
  
  TOKEN_TYPE_START = 256,
  TOKEN_INTEGER,
  TOKEN_FLOAT,
  TOKEN_RANGE,
  TOKEN_CHAR,
  TOKEN_STRING,

  TOKEN_IDENTIFIER = 512,
  
  TOKEN_UTIL_START = 1024,
  TOKEN_UT_ALIGNAS,
  TOKEN_UT_ALIGNOF,
  TOKEN_UT_ATOMIC,
  TOKEN_UT_BOOL,
  TOKEN_UT_COMPLEX,
  TOKEN_UT_GENERIC,
  TOKEN_UT_IMAGINARY,
  TOKEN_UT_PRAGMA,
  TOKEN_UT_NORETURN,
  TOKEN_UT_STATIC_ASSERT,
  TOKEN_UT_THREAD_LOCAL,
  TOKEN_UT_TYPEOF,
  TOKEN_UT_VECTOR,
  TOKEN_UT_CDECL,
  TOKEN_UT_STDCALL,
  TOKEN_UT_DECLSPEC,

  TOKEN_KW_START = 2048,
  TOKEN_KW_WHILE,
  TOKEN_KW_BREAK,
  TOKEN_KW_CASE,
  TOKEN_KW_CONST,
  TOKEN_KW_CONTINUE,
  TOKEN_KW_VOLATILE,
  TOKEN_KW_VOID,
  TOKEN_KW_UNSIGNED,
  TOKEN_KW_UNION,
  TOKEN_KW_DEFAULT,
  TOKEN_KW_DO,
  TOKEN_KW_DOUBLE,
  TOKEN_KW_ELSE,
  TOKEN_KW_IF,
  TOKEN_KW_ENUM,
  TOKEN_KW_EXTERN,
  TOKEN_KW_TYPEDEF,
  TOKEN_KW_SWITCH,
  TOKEN_KW_STRUCT,
  TOKEN_KW_STATIC,
  TOKEN_KW_SIZEOF,
  TOKEN_KW_SIGNED,
  TOKEN_KW_FLOAT,
  TOKEN_KW_FOR,
  TOKEN_KW_GOTO,
  TOKEN_KW_INLINE,
  TOKEN_KW_INT,
  TOKEN_KW_LONG,
  TOKEN_KW_REGISTER,
  TOKEN_KW_RETURN,
  TOKEN_KW_SHORT,

  FINAL_TOKEN,
};

// An integer type
// Example:
// signed long long
// ->
// IntegerType {
// .usigned = false,
// .long_a = true,
// .long_b = true,
// }
struct IntegerType {
  bool usigned;
  bool long_a;
  bool long_b;
};

struct Token {
  enum TokenType ty;
  struct Location loc;
  const uchar* start;
  usize token_len;
};


struct TokenStream {
  struct Vec* tokens;
  usize token_index;
};

struct Lexer {
  struct TokenStream* stream;
  usize index;
  usize line;
  const char* fname;
  const char* contents;
};


struct TokenStream* token_stream_new();
void token_stream_drop(struct TokenStream* self);

void lexer_new(struct Lexer* self, char* fname);
void lexer_lex(struct Lexer* self);
