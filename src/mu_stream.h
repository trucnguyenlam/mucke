#ifndef _mu_stream_INCLUDED
#define _mu_stream_INCLUDED

class IOStream;

class MuStreamOpener
{
  static MuStreamOpener * _instance;

  MuStreamOpener();

public:

  static MuStreamOpener * instance()
  {
    if(!_instance) _instance = new MuStreamOpener;
    return _instance;
  }

  bool push(IOStream &, char * n);
};

#endif
