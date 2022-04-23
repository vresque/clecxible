#pragma once
#include <core.h>
#include <limits.h>
#include <spec.h>

enum OptLevel {
  O1,
  O2,
  O3,
  OSIZE,
  O0,
};

struct Arguments {
  char* filepath; // <cmd> <fname>
  enum Target target; // <cmd> --target <x86_64>
  enum OutputFormat format; // <cmd> --format <elf|mach-o| 
  enum Platform plat; // <cmd> --platform <windows|unix>
  enum OptLevel optimization; // <cmd> --optimize|-O <0,1,2,3,size>
};

struct Arguments parse_args(i32 argc, ichar** argv);
void usage();
