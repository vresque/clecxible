#pragma once
#include <core.h>

struct Location {
  const uchar* file_name;
  const uchar* line_as_string;
  usize line_length;
  u32 line_number;
  u8 column_number;
};
  
