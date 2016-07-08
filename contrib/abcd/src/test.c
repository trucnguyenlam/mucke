/*==========================================================================
 *  (C) 1997-98 Armin Biere 
 *  $Id: test.c,v 1.2 2000-05-08 11:33:37 biere Exp $
 *==========================================================================
 */

#include <stdio.h>

struct Test {
  unsigned v : 10, m : 1, d : 1, l_low : 20, l_high : 6, r : 26;
};

#include "ABCD.h"
#include "abcd_node_int.h"
#include "abcd_cache_int.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/
/* Mark the revision in the code.
 */

char * test_rcsid =
"$Id: test.c,v 1.2 2000-05-08 11:33:37 biere Exp $"
;

/*------------------------------------------------------------------------*/

#if 0

static void test()
{
  ABCD_Manager m;

  printf("sizeof struct Test = %d\n", sizeof(struct Test));

  m = ABCD_init(.25);

  /* ABCD_visualize(m,ABCD_not(m,ABCD_var(m,0))); */
  /* ABCD_visualize(m,ABCD_and(m,ABCD_not(m,ABCD_var(m,0)), ABCD_not(m,ABCD_var(m,1))));
  */
  {
    ABCD v0 = ABCD_var(m,0,"v0");
    ABCD v1 = ABCD_var(m,1,"v0");
    ABCD res = ABCD_and(m,v0, v1);
    ABCD v2 = ABCD_var(m,2,"v2");
    ABCD v3 = ABCD_var(m,3,"v3");
    ABCD v4 = ABCD_var(m,4,"v4");
    ABCD v5 = ABCD_var(m,5,"v5");
    ABCD v6 = ABCD_var(m,6,"v6");
    ABCD v7 = ABCD_var(m,7,"v7");
    ABCD v8 = ABCD_var(m,8,"v8");
    ABCD v9 = ABCD_var(m,9,"v9");
    ABCD v10 = ABCD_var(m,10,"v10");
    ABCD v11 = ABCD_var(m,11,"v11");
    ABCD v13 = ABCD_var(m,13,"v13");
    ABCD v14 = ABCD_var(m,14,"v14");
    ABCD v15 = ABCD_var(m,15,"v15");
    ABCD v16 = ABCD_var(m,16,"v16");
    ABCD v17 = ABCD_var(m,17,"v17");
    ABCD v18 = ABCD_var(m,18,"v18");
    ABCD v19 = ABCD_var(m,19,"v19");
    ABCD v12 = ABCD_var(m,12,"v12");
    ABCD v20 = ABCD_var(m,20,"v20");
    ABCD sub = ABCD_constant(m,1);
    ABCD range;

    printf("onsetsize = %f\n", ABCD_onsetsize(m,ABCD_not(m,res),res));

    /* ABCD_visualize(m,res); */
    res=ABCD_or(m,ABCD_and(m,ABCD_not(m,v0), ABCD_not(m,v1)), ABCD_and(m,v0, v1));
    /* ABCD_visualize(m,res); */
    res =ABCD_exists(m,res, v1);
    /* ABCD_visualize(m,res); */
    res = ABCD_and(m,ABCD_equiv(m,v1, v0), ABCD_equiv(m,v2,v3)); 
    res = ABCD_and(m,ABCD_equiv(m,v0,v1), v3); 
    /* ABCD_visualize(m,res); */
    ABCD_add_substitution(m,sub, 1, ABCD_not(m,v4));
    ABCD_add_substitution(m,sub, 0, ABCD_not(m,v5));
    res = ABCD_compose(m,res, sub);
    range = ABCD_and(m,ABCD_and(m,v2, v3), ABCD_and(m,v4, v5));
    /*
    printf("size      = %d\n", ABCD_size(m,res));
    printf("onsetsize = %f\n", ABCD_onsetsize(m,res,range));
    ABCD_visualize(m,res);
    */
    res = ABCD_exists(m,res, v3);
    printf("size      = %d\n", ABCD_size(m,res));
    printf("onsetsize = %f\n", ABCD_onsetsize(m,res,range));
    res=ABCD_not(m,res);
    /*
    ABCD_visualize(m,res);
    ABCD_visualize(m,ABCD_onecube(m,res, range));
    */
    ABCD_free(m,v7);
    ABCD_free(m,v8);
    ABCD_free(m,v9);
    ABCD_free(m,v10);
    ABCD_free(m,v11);
    ABCD_free(m,v20);
    ABCD_free(m,v12);
    ABCD_free(m,v13);
    ABCD_free(m,v14);
    ABCD_free(m,v15);
    ABCD_free(m,v16);
    ABCD_free(m,v17);
    ABCD_free(m,v18);
    ABCD_free(m,v19);
    ABCD_free(m,v3);
    ABCD_free(m,v0);
    ABCD_free(m,v4);
    ABCD_free(m,res);
    res = ABCD_and(m,ABCD_equiv(m,v1, v2), ABCD_and(m,v5,v6)); 
    ABCD_free(m,v1);
    ABCD_free(m,v2);
    ABCD_free(m,v5);
    ABCD_free(m,v6);
    printf("%s\n", ABCD_statistics(m));
  }
  ABCD_exit(m);
}
#else

#if 0
#define MAX 10

static void test()
{
  ABCD vars[MAX];
  ABCD f,g,h;
  ABCD_Manager m;
  int i;

  m ABCD_init();

  for(i=0; i<MAX; i++) vars[i] = ABCD_var(m,i,0);
  for(i=4; i<MAX; i++) ABCD_free(m,vars[i]);
  
  f=ABCD_equiv(m,vars[0], vars[1]);
  g=ABCD_equiv(m,vars[1], vars[2]);
  h=ABCD_and(m,f,g);

  ABCD_free(m,f);
  ABCD_free(m,g);
  ABCD_free(m,h);
  for(i=0; i<4; i++) ABCD_free(m,vars[i]);

  printf("%s\n", ABCD_statistics(m,));
  ABCD_exit(manager);
}

#else

static void print(char * str) { fputs(str, stdout); }

static void test()
{
  ABCD_Manager manager;
  ABCD v120, v121, f, g;
  struct aBCD_CacheEntry dummy;

  (void) dummy;

  printf("sizeof(struct aBCD_Node) = %d\n" ,
         (int) sizeof(struct aBCD_Node));

  printf("sizeof(struct aBCD_CacheEntry) = %d\n\n" ,
         (int) sizeof(struct aBCD_CacheEntry));

  manager = ABCD_init_take_all_Memory();

# if 1
    v120 = ABCD_new_var(manager, "v120");
    v121 = ABCD_new_var(manager, "v121");
# else
    v120 = ABCD_var(manager, 0, "v120");
    v121 = ABCD_var(manager, 0, "v121");
# endif

  f = ABCD_equiv(manager, v120, v121);
  g = ABCD_unnegate(manager, f);

  printf("negated f (fraction %f):\n-----------------\n\n",
    ABCD_fraction(manager, f));
  ABCD_print(manager, f, stdout);

  printf("\nunnegated f (fraction %f):\n-----------------\n\n",
    ABCD_fraction(manager, g));
  ABCD_print(manager, g, stdout);

  ABCD_dump(manager, f, (char**) 0, print);
  ABCD_dump(manager, g, (char**) 0, print);

  printf("\n%s\n", ABCD_statistics(manager));
  ABCD_exit(manager);
}
#endif

#endif


int
main(int argc, char ** argv)
{
  test();
  exit(0);
  return 0;
}
