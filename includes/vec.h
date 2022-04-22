#pragma once

#include <string.h>
#include <core.h>

struct vec {
  u8 *data;
  usize len;
  usize cap;
};

void vec_new(struct vec *self, usize cap);
void vec_drop(struct vec *self);
void vec_grow(struct vec *self, usize to);
void vec_push(struct vec *self, const u8 *item, usize size);
void vec_clear(struct vec *self, usize cap);
