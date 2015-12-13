#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include "utils.h"

int debug_level;

void debugf(int level, char* fmt, ...)
{
  if (debug_level)
  {
    if (level <= debug_level)
    {
#ifdef SYS_gettid
      pid_t thread_id = syscall(SYS_gettid);
#else
      int thread_id = 0;
#error "SYS_gettid unavailable on this system"
#endif
      va_list args;
      char prefix[] = "==DBG %d [%s]:%d==";
      char line[4096];
      char time_str[TIME_CHARS];
      get_time_now_as_str(time_str, sizeof(time_str));
      sprintf(line, prefix, level, time_str, thread_id);
      fputs(line, stderr);
      va_start(args, fmt);
      vfprintf(stderr, fmt, args);
      va_end(args);
      fputs(KNRM, stderr);
      putc('\n', stderr);
      putc('\r', stderr);
    }
  }
}

char* get_home_dir()
{
  char* home;
  if ((home = getenv("HOME")) && !access(home, R_OK))
    return home;
  struct passwd* pwd = getpwuid(geteuid());
  if ((home = pwd->pw_dir) && !access(home, R_OK))
    return home;
  return "~";
}
