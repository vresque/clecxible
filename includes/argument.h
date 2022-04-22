#pragma once
#include <core.h>
#include <limits.h>
#include <spec.h>

enum opt_level {
  O1,
  O2,
  O3,
  OSIZE,
  O0,
};

struct arguments {
  path filepath; // <cmd> <fname>
  enum target target; // <cmd> --target <x86_64>
  enum output_format format; // <cmd> --format <elf|mach-o| 
  enum platform plat; // <cmd> --platform <windows|unix>
  enum opt_level optimization; // <cmd> --optimize|-O <0,1,2,3,size>
};

struct arguments parse_args(i32 argc, ichar** argv);
void usage();
