extern void resetTerminal();
extern void deinstall_linuxpinfo();
struct PreCleanupBucket {
  void (*f)();
  const char * comment;
}; 
static PreCleanupBucket precleanupbuckets [] = {
  {resetTerminal, " Reset Terminal"},
  {deinstall_linuxpinfo, " print vsize initializer"},
  {0, 0}
}; 
