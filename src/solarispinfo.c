#include "config.h"

#ifdef SOLARIS

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/signal.h>
#include <sys/fault.h>
#include <sys/syscall.h>
#include <sys/procfs.h>

#include <sys/stat.h>

static struct prstatus status;
static struct prpsinfo info;

int pinfo_install()
{
  char buffer[200];
  int fd;
  pid_t pid = getpid();
  sprintf(buffer, "/proc/%d", (int) pid);
  fd = open(buffer, O_RDONLY);
  if(fd==-1) return 0;
  if(ioctl(fd, PIOCSTATUS, &status)==-1) return 0;
  if(ioctl(fd, PIOCPSINFO, &info)==-1) return 0;
  close(fd);
  return 1;
}

double pinfo_seconds()
{
  struct prstatus * s = &status;
  return s -> pr_utime.tv_sec + s -> pr_stime.tv_sec +
         (s -> pr_utime.tv_nsec + s -> pr_stime.tv_nsec) * 1e-9;
}

unsigned int pinfo_size()
{
  struct prstatus * s = &status;
  return s -> pr_brksize + s -> pr_stksize;
}
#endif
