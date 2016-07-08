#include "repr.h"
#include "io.h"

void teststatistics()
{
  output << PredicateManager::instance() -> stats() << '\n';
}
