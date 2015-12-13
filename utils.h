#ifndef _UTILS_H
#define _UTILS_H

#include "config.h"
#include <pthread.h>

#define DBG_LEVEL_NONE 0
#define DBG_LEVEL_NORM 1
#define DBG_LEVEL_EXT 2
#define DBG_LEVEL_EXTALL 3

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

// 64 bit time + nanoseconds
#define TIME_CHARS 32
#define INT_CHAR_LEN 16

#define min(x, y) ({                \
  typeof(x) _min1 = (x);          \
  typeof(y) _min2 = (y);          \
  (void)(&_min1 == &_min2);      \
  _min1 < _min2 ? _min1 : _min2; })

typedef enum { false, true } bool;

extern int debug_level;
extern pthread_mutex_t dcachemut;
extern pthread_mutexattr_t mutex_attr;

void debugf(int level, char* fmt, ...);
char* get_home_dir();

#endif
