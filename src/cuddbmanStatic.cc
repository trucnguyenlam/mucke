#include "cuddbman.h"

extern "C" {
#include <math.h>
#include <stdio.h>
#include "cudd.h"
};

/*------------------------------------------------------------------------*/

#define resolve(a, b, str) do { PTR_ ## a = b; } while(0)

/*------------------------------------------------------------------------*/

void load_cudd_bdd_library()
{
  verbose << "libcudd.a is statically linked\n";
  verbose << "resolving symbols ...\n" << inc();

  resolve(cudd_and, Cudd_bddAnd, "Cudd_bddAnd");
  resolve(cudd_cofactor, Cudd_Cofactor, "Cudd_Cofactor");
  resolve(cudd_exists, Cudd_bddExistAbstract, "Cudd_bddExistAbstract");
  resolve(cudd_forall, Cudd_bddUnivAbstract, "Cudd_bddUnivAbstract");
  resolve(cudd_recursive_free, Cudd_RecursiveDeref, "Cudd_RecursiveDeref");
  resolve(cudd_free, Cudd_Deref, "Cudd_Deref");
  resolve(cudd_init, Cudd_Init, "Cudd_Init");
  resolve(cudd_ite, Cudd_bddIte, "Cudd_bddIte");
  resolve(cudd_new_var_last, Cudd_bddNewVar, "Cudd_bddNewVar");
  resolve(cudd_var, Cudd_bddIthVar, "Cudd_bddIthVar");
  resolve(cudd_one, Cudd_ReadOne, "Cudd_ReadOne");
  resolve(cudd_or, Cudd_bddOr, "Cudd_bddOr");
  resolve(cudd_quit, Cudd_Quit, "Cudd_Quit");
  resolve(cudd_minimize, Cudd_bddMinimize, "Cudd_bddMinimize");
  resolve(cudd_constrain, Cudd_bddConstrain, "Cudd_bddConstrain");
  resolve(cudd_restrict, Cudd_bddRestrict, "Cudd_bddRestrict");
  resolve(cudd_compact, Cudd_bddLICompaction, "Cudd_bddLICompaction");
  resolve(cudd_rel_prod, Cudd_bddAndAbstract, "Cudd_bddAndAbstract");
  resolve(cudd_intersect, Cudd_bddIntersect, "Cudd_bddIntersect");
  resolve(cudd_size, Cudd_DagSize, "Cudd_DagSize");
  resolve(cudd_compose, Cudd_bddCompose, "Cudd_bddCompose");
  resolve(cudd_vector_compose, Cudd_bddVectorCompose, "Cudd_bddVectorCompose");
  resolve(cudd_unfree, Cudd_Ref, "Cudd_Ref");
  resolve(cudd_xnor, Cudd_bddXnor, "Cudd_bddXnor");
  resolve(cudd_xor, Cudd_bddXor, "Cudd_bddXor");
  resolve(cudd_zero, Cudd_ReadLogicZero, "Cudd_ReadLogicZero");
  resolve(cudd_indices_to_cube, Cudd_IndicesToCube, "Cudd_IndicesToCube");
  resolve(cudd_make_prime, Cudd_bddMakePrime, "Cudd_bddMakePrime");
  resolve(cudd_vars, Cudd_ReadSize, "Cudd_ReadSize");
  resolve(cudd_count_min_term, Cudd_CountMinterm, "Cudd_CountMinterm");
  resolve(cudd_countpathstononzero, Cudd_CountPathsToNonZero, "Cudd_CountPathsToNonZero");
  resolve(cudd_dumpdot, Cudd_DumpDot, "Cudd_DumpDot");
  resolve(cudd_computecube, Cudd_bddComputeCube, "Cudd_bddComputeCube");

  verbose <<  dec() << "done.\n";
}
