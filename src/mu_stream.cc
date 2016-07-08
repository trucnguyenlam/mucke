#include "mu_stream.h"
#include "io.h"
#include "String.h"

extern "C" {
#include <string.h>
};

MuStreamOpener * MuStreamOpener::_instance = 0;
MuStreamOpener::MuStreamOpener() { }

bool MuStreamOpener::push(IOStream & io, char * name)
{
  if(io.push(IOStream::Read, name)) return true;

  int l = strlen(name);
  if(l > 3 && cmp(name + l - 3, ".mu")) return false;

  char * buf = new char [strlen(name) + 1 + 3];

  strcpy(buf, name);
  strcat(buf, ".mu");

  return io.push(IOStream::Read, buf);

  delete buf;
}
