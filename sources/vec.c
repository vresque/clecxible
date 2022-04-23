#include <vec.h>
#include <stdlib.h>

void vec_new(struct Vec *self, usize cap) {
  if (!self) self = malloc(sizeof(struct Vec));
  self->cap = cap;
  self->len = 0;
  self->data = malloc(self->cap);
}

void vec_drop(struct Vec *self) {
  if (self->data)
    free(self->data);
  free(self);
}

void vec_grow(struct Vec *self, usize to) {
  self->data = realloc(self->data, to);
}

void vec_push(struct Vec *self, const u8 *item, usize size) {
  if (self->cap < self->len + size)
    vec_grow(self, (self->len + self->len / 2 + size));

  memcpy(self->data + self->len, item, size);
  self->len += size;
}

void vec_clear(struct Vec *self, usize cap) {
  if (self->data)
    free(self->data);
  self->data = malloc(cap);
  self->len = 0;
  self->cap = cap;
}
