#include <vec.h>
#include <stdlib.h>

void vec_new(struct vec *self, usize cap) {
  self->cap = cap;
  self->len = 0;
  self->data = malloc(self->cap);
}

void vec_drop(struct vec *self) {
  if (self->data)
    free(self->data);
}

void vec_grow(struct vec *self, usize to) {
  self->data = realloc(self->data, to);
}

void vec_push(struct vec *self, const u8 *item, usize size) {
  if (self->cap < self->len + size)
    vec_grow(self, (self->len + self->len / 2 + size));

  memcpy(self->data + self->len, item, size);
  self->len += size;
}

void vec_clear(struct vec *self, usize cap) {
  if (self->data)
    free(self->data);
  self->data = malloc(cap);
  self->len = 0;
  self->cap = cap;
}
