/* THIS FILE IS AUTOMICALLY GENERATED -- DO NOT EDIT
 * from `abcd_manager.tmpl' on Thu Aug 18 16:20:47 2016
 */

#ifndef _abcd_manager_h_INCLUDED
#define _abcd_manager_h_INCLUDED

/*****************************************************************************\
(C) 1997 Armin Biere 
$Id: abcd_manager.tmpl,v 1.2 2000-05-08 11:33:35 biere Exp $
\*****************************************************************************/

#include "abcd_manager_int.h"
#include "abcd_node_int.h"
#include "abcd_types.h"
#include "abcd_util.h"

/*---------------------------------------------------------------------------*/

#line 13 "abcd_manager.tmpl"
#define _aBCD_IdxAsNodePtr( \
  base, \
  idx) \
( \
 &base[(idx) >> 1] \
)

/*---------------------------------------------------------------------------*/

#line 25 "abcd_manager.tmpl"
#define aBCD_IdxAsNodePtr( \
  manager, \
  idx) \
( \
  _aBCD_IdxAsNodePtr((manager)->nodes,(idx)) \
)

/*---------------------------------------------------------------------------*/

#line 37 "abcd_manager.tmpl"
#define _aBCD_NodePtrAsIdx( \
  base, \
  n) \
( \
  ((n) - (base)) << 1 \
)

/*---------------------------------------------------------------------------*/

#line 49 "abcd_manager.tmpl"
#define aBCD_NodePtrAsIdx( \
  manager, \
  n) \
( \
  _aBCD_NodePtrAsIdx((manager) -> nodes, n) \
)

/*---------------------------------------------------------------------------*/

#line 61 "abcd_manager.tmpl"
#define aBCD_push( \
  manager, \
  i) \
do { \
  assert((manager) -> sp < (manager) -> top_of_stack, "stack overflow"); \
  *(manager) -> sp++ = (aBCD_uint) (i); \
} while(0)

/*---------------------------------------------------------------------------*/

#line 74 "abcd_manager.tmpl"
#define aBCD_pop( \
  manager, \
  void_ptr) \
do { \
  assert((manager) -> sp > (manager) -> stack, "stack underflow"); \
  *(aBCD_uint *)(void_ptr) = *--(manager) -> sp; \
} while(0)

/*---------------------------------------------------------------------------*/

#line 87 "abcd_manager.tmpl"
#define aBCD_top( \
  manager, \
  void_ptr) \
do { \
  assert((manager) -> sp > (manager) -> stack, "stack underflow"); \
  *(aBCD_uint*)(void_ptr) = (manager) -> sp[-1]; \
} while(0)

/*---------------------------------------------------------------------------*/

#line 100 "abcd_manager.tmpl"
#define aBCD_check_for_garbage( \
  manager) \
( \
  manager -> nodes_count > manager -> gc_threshold \
)

/*---------------------------------------------------------------------------*/

#endif
