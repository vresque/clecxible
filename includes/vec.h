#pragma once

#include <string.h>
#include <core.h>

struct Vec {
  u8 *data;
  usize len;
  usize cap;
};

void vec_new(struct Vec *self, usize cap);
void vec_drop(struct Vec *self);
void vec_grow(struct Vec *self, usize to);
void vec_push(struct Vec *self, const u8 *item, usize size);
void vec_clear(struct Vec *self, usize cap);
