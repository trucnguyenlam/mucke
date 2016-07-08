#ifndef _configurator_h_INCLUDED
#define _configurator_h_INCLUDED

class AvoidNastyWarning;

class Configurator
{
  friend class AvoidNastyWarning;

  Configurator() { }
  ~Configurator() { }

  static Configurator * _instance;

  void usage();

public:

  static Configurator * instance() { return _instance; }

  void commandLineArgs(int argc, char ** argv);
};

#endif
