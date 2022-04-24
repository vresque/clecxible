#include <vec.h>
#include <stdlib.h>


#define GET_INFO(this) GET_VEC_INFO(this)
void* _vec_new(usize elem_size, usize cap) {
  puts("lo");
  struct VecInfo* info = calloc(1, sizeof(struct VecInfo) + (elem_size * cap));
  if (!info) { die("failed to allocate new vector"); }
  info->elem_size = elem_size;
  info->cap = cap;
  info->is_genesis = true;
  return &info->data[0];
}


void _vec_drop(void* self) {
  struct VecInfo* info = GET_INFO(self);
  free(info->data);
  free(info);
  free(self); // Should be NULL
}

struct VecInfo* vec_grow(void* self, usize to) {
  struct VecInfo* info = GET_INFO(self);
  struct VecInfo* ret = realloc(info, sizeof(struct VecInfo) + to);
  return ret;
}

void* _vec_prep_push(void* self, usize count) {
  struct VecInfo* info = GET_INFO(self);
  if ((info->cap * info->elem_size) < (info->len * info->elem_size) + (count * info->elem_size)) {
    struct VecInfo* new = vec_grow(self, ((info->len * info->elem_size) + (info->len * info->elem_size) / 2 + (count * info->elem_size)));
    return &new->data[0];
  }
  return self;
}

void _vec_clear(void** self, usize cap) {
   struct VecInfo* info = GET_INFO(self);
   *self = _vec_new(info->elem_size, cap);
}

usize vec_len(void* self) {
  return GET_INFO(self)->len;
}