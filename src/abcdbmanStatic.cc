#include "abcdbman.h"

/*---------------------------------------------------------------------------*/

#define resolve(a,str) do { PTR_ ## a = a; } while(0)

/*---------------------------------------------------------------------------*/

void load_abcd_library()
{
  verbose << "libabcd.a is statically linked\n";
  verbose << "resolving symbols ...\n" << inc();

  resolve(ABCD_add_substitution,"ABCD_add_substitution");
  resolve(ABCD_add_variable,"ABCD_add_variable");
  resolve(ABCD_and,"ABCD_and");
  resolve(ABCD_are_equal,"ABCD_are_equal");
  resolve(ABCD_cofactor,"ABCD_cofactor");
  resolve(ABCD_copy, "ABCD_copy");
  resolve(ABCD_compose,"ABCD_compose");
  resolve(ABCD_constant, "ABCD_constant");
  resolve(ABCD_dump, "ABCD_dump");
  resolve(ABCD_equiv,"ABCD_equiv");
  resolve(ABCD_exit,"ABCD_exit");
  resolve(ABCD_exists,"ABCD_exists");
  resolve(ABCD_free, "ABCD_free");
  resolve(ABCD_get_MB_usage, "ABCD_get_MB_usage");
  resolve(ABCD_implies,"ABCD_implies");
  resolve(ABCD_init_MB, "ABCD_init_MB");
  resolve(ABCD_init_take_all_Memory, "ABCD_init_take_all_Memory");
  resolve(ABCD_intersects,"ABCD_intersects");
  resolve(ABCD_ite,"ABCD_ite");
  resolve(ABCD_is_false, "ABCD_is_false");
  resolve(ABCD_is_true, "ABCD_is_true");
  resolve(ABCD_not,"ABCD_not");
  resolve(ABCD_onecube, "ABCD_onecube");
  resolve(ABCD_onsetsize,"ABCD_onsetsize");
  resolve(ABCD_or,"ABCD_or");
  resolve(ABCD_reduce,"ABCD_reduce");
  resolve(ABCD_relprod,"ABCD_relprod");
  resolve(ABCD_size,"ABCD_size");
  resolve(ABCD_statistics,"ABCD_statistics");
  resolve(ABCD_var,"ABCD_var");
  resolve(ABCD_visualize,"ABCD_visualize");

  verbose << dec() << "done.\n";
}
