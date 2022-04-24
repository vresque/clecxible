#pragma once
#include <core.h>

// SAFETY: A NULL may be passed to length if you do not care about it
char* read_to_string(char* fname, u64* length);
