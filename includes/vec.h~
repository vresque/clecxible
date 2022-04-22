#pragma once

#include "util.h"
#include <string.h>

struct mem_sink {
  char *data;
  unsigned int len;
  unsigned int cap;
};

void mem_sink_new(struct mem_sink *self, unsigned int cap);
void mem_sink_drop(struct mem_sink *self);
void mem_sink_grow(struct mem_sink *self, unsigned int to);
void mem_sink_push(struct mem_sink *self, const char *item, unsigned int size);
void mem_sink_clear(struct mem_sink *self, unsigned int cap);
