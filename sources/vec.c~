#include "memsink.h"

void mem_sink_new(struct mem_sink *self, unsigned int cap) {
  self->cap = cap;
  self->len = 0;
  self->data = malloc(self->cap);
}

void mem_sink_drop(struct mem_sink *self) {
  if (self->data)
    free(self->data);
}

void mem_sink_grow(struct mem_sink *self, unsigned int to) {
  self->data = realloc(self->data, to);
}

void mem_sink_push(struct mem_sink *self, const char *item, unsigned int size) {
  if (self->cap < self->len + size)
    mem_sink_grow(self, (self->len + self->len / 2 + size));

  memcpy(self->data + self->len, item, size);
  self->len += size;
}

void mem_sink_clear(struct mem_sink *self, unsigned int cap) {
  if (self->data)
    free(self->data);
  self->data = malloc(cap);
  self->len = 0;
  self->cap = cap;
}
