#pragma once

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNUSED(val) (void)val;
#define STRINGIFY(val) #val
#define RANGE(a, b) a... b
#define verify(return_value, msg, ...)                                         \
  if ((return_value)) {                                                        \
    fprintf(stderr,                                                            \
            "program crashed during runtime.\nerrorcode: %d\n "                \
            "error:\n%s\nerrno: %d\nstringified errno: %s\n"                   \
            "location of crash: file " __FILE__                                \
            "; line %d; function " __func__,                                   \
            return_value, strerror(return_value), errno, strerror(errno),      \
            __LINE__);                                                         \
    die("" msg, ##__VA_ARGS__);                                                \
  }

#define assert(cond, msg, ...)                                                 \
  if (!(cond)) {                                                               \
    fprintf(stderr,                                                            \
            "assertion failed: " STRINGIFY(                                    \
                cond) "\nlocation of failure: file " __FILE__                  \
                      "; line %d; function %s\n",                              \
            __LINE__, __func__);                                               \
    die(msg, ##__VA_ARGS__);                                                   \
  }

int die(const char *s, ...);

typedef signed char i8;
typedef short int i16;
typedef int i32;
#if __WORDSIZE == 64
typedef long int i64;
#else
typedef long long int i64;
#endif

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
#if __WORDSIZE == 64
typedef unsigned long int u64;
#else
typedef unsigned long long int u64;
#endif

typedef signed char ifast8;
#if __WORDSIZE == 64
typedef long int ifast16;
typedef long int ifast32;
typedef long int ifast64;
#else
typedef int ifast16;
typedef int ifast32;
typedef long long int ifast64;
#endif

typedef unsigned char ufast8;
#if __WORDSIZE == 64
typedef unsigned long int ufast16;
typedef unsigned long int ufast32;
typedef unsigned long int ufast64;
#else
typedef unsigned int ufast16;
typedef unsigned int ufast32;
typedef unsigned long long int ufast64;
#endif

#if __WORDSIZE == 64
typedef long int iptr;
typedef unsigned long int uptr;
#else
typedef int iptr;
typedef unsigned long long int uptr;
#endif

#include <stddef.h>
#undef size_t

typedef long unsigned int usize;
typedef long isize;

typedef _Bool bool;
#define true 1
#define false 0

typedef unsigned char uchar;
typedef char ichar;

#include <limits.h>
typedef uchar path[PATH_MAX];

#define do_once(body)                                                          \
  do {                                                                         \
    body                                                                       \
  } while (false)