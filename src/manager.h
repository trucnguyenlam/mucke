#ifndef _manager_h_INCLUDED
#define _manager_h_INCLUDED

template<class Manager>
class Managed
{
  Manager * _manager;

public:

  Managed(Manager* M) : _manager(M) { }
  const Manager * manager() { return _manager; }
};

#endif
