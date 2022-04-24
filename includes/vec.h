#pragma once

#include <string.h>
#include <core.h>

#define Vec(T) T*

struct VecInfo {
  bool is_genesis;
  usize len;
  usize cap;
  usize elem_size;
  u8 data[];
};

void* _vec_new(usize elem_size, usize cap);
void _vec_drop(void* self);
void* _vec_prep_push(void* self, usize count);
struct VecInfo* vec_grow(void* self, usize to);
void* _vec_prep_push(void* self, usize count);
void _vec_clear(void** self, usize cap);
usize vec_len(void* self);

void vec_display(void* self);

#define vec_new(T, cap) _vec_new(sizeof(T), cap)

#define vec_drop(name) _vec_drop(name)
#define vec_push_one(name, it) vec_push_counted(name, it, 1)
#define vec_push_counted(name, it, count) do_once({ \
  name = _vec_prep_push((void*)name, count); \
  name[(GET_VEC_INFO(name)->len++)] = it; \
})

#define GET_VEC_INFO(this) ( ( (struct VecInfo*) this) - 1 )
