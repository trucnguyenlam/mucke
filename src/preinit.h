extern void basicTypePreInitializer();
extern void useBooleReprPreInit();
extern void symbolPreInitializer();
extern void termPreInitializer();
extern void debugPreInitializer();
extern void loadBManPreInit();
extern void memDebugInit();
extern void linkBManPreInit();
extern void install_linuxpinfo();
extern void evaluationManagerPreInitializer();
extern void booleManPreInitializer();
extern void IOStream_PreInitializer();
extern void InstallAtExit();
struct PreInitBucket {
  void (*f)();
  const char * comment;
}; 
static PreInitBucket preinitbuckets [] = {
  {basicTypePreInitializer, " PreInitializer of Basic Types"},
  {useBooleReprPreInit, " PreInitializer for using Boole Representation"},
  {symbolPreInitializer, " PreInitializer for Symbol Class"},
  {termPreInitializer, " PreInitializer for Term Class"},
  {debugPreInitializer, " debugging PreInitializer"},
  {loadBManPreInit, " PreInitializer for loading Boole::manager"},
  {memDebugInit, " PreInitializer for memory debugging"},
  {linkBManPreInit, " PreInitializer for linking Boole::manager"},
  {install_linuxpinfo, " print vsize initializer"},
  {evaluationManagerPreInitializer, " Evaluation::manager PreInitializer"},
  {booleManPreInitializer, " Boole::manager PreInitializer"},
  {IOStream_PreInitializer, " PreInitializer of IOStream"},
  {InstallAtExit, " register PreCleanUp"},
  {0, 0}
}; 
