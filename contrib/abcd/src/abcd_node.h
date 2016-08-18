/* THIS FILE IS AUTOMICALLY GENERATED -- DO NOT EDIT
 * from `abcd_node.tmpl' on Thu Aug 18 16:20:47 2016
 */

#ifndef _abcd_node_h_INCLUDED
#define _abcd_node_h_INCLUDED

/*****************************************************************************\
(C) 1997 Armin Biere 
$Id: abcd_node.tmpl,v 1.2 2000-05-08 11:33:35 biere Exp $
\*****************************************************************************/

#include "config.h"
#include "abcd_manager.h"
#include "abcd_manager_int.h"
#include "abcd_node_int.h"
#include "abcd_util.h"

/*===========================================================================*/
#ifdef aBCD_compiler_does_bit_allocation
/*===========================================================================*/

#line 16 "abcd_node.tmpl"
#define aBCD_Node_mark( \
  node_ptr) \
do { \
  (node_ptr) -> marked = 1; \
} while(0)

/*---------------------------------------------------------------------------*/

#line 26 "abcd_node.tmpl"
#define aBCD_Node_unmark( \
  node_ptr) \
do { \
  (node_ptr) -> marked = 0; \
} while(0)

/*---------------------------------------------------------------------------*/

#line 36 "abcd_node.tmpl"
#define aBCD_Node_is_marked( \
  node_ptr) \
( \
  (node_ptr) -> marked \
)

/*---------------------------------------------------------------------------*/
#ifdef USE_SHARED
/*---------------------------------------------------------------------------*/

#line 48 "abcd_node.tmpl"
#define aBCD_Node_set_shared( \
  node_ptr) \
do { \
  /* epsilon */ \
} while(0)

/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

#line 60 "abcd_node.tmpl"
#define aBCD_Node_is_shared( \
  node_ptr) \
( \
  (node_ptr) -> shared != 0 \
)

/*---------------------------------------------------------------------------*/

#line 70 "abcd_node.tmpl"
#define aBCD_Node_signed_succ1( \
  node_ptr) \
( \
  (node_ptr) -> right \
)

/*---------------------------------------------------------------------------*/

#line 80 "abcd_node.tmpl"
#define aBCD_Node_var( \
  node_ptr) \
( \
  (node_ptr) -> var \
)

/*---------------------------------------------------------------------------*/
#ifdef aBCD_split_left_successor
/*---------------------------------------------------------------------------*/

#line 92 "abcd_node.tmpl"
#define aBCD_is_empty_Node( \
  node_ptr) \
( \
  (node_ptr) -> right == 0 && \
  (node_ptr) -> left_high == 0 && \
  (node_ptr) -> left_low == 0 \
)

/*---------------------------------------------------------------------------*/

#line 104 "abcd_node.tmpl"
#define aBCD_mk_empty_Node( \
  node_ptr) \
do { \
  /* Make sure that this is equivalent to setting all bits to zero! \
   * (right now this function is just a comment since `memset' is used) \
   */ \
  (node_ptr) -> var = 0; \
  (node_ptr) -> marked = 0; \
  (node_ptr) -> shared = 0; \
  (node_ptr) -> left_high = 0; \
  (node_ptr) -> left_low = 0; \
  (node_ptr) -> right = 0; \
} while(0)

/*---------------------------------------------------------------------------*/

#line 122 "abcd_node.tmpl"
#define aBCD_are_the_same_Node( \
  a, \
  b) \
( \
  (a) -> var == (b) -> var && \
  (a) -> left_low == (b) -> left_low && \
  (a) -> left_high == (b) -> left_high && \
  (a) -> right == (b) -> right \
)

/*---------------------------------------------------------------------------*/

#line 137 "abcd_node.tmpl"
#define aBCD_init_node( \
  v, \
  l, \
  r, \
  node_ptr) \
do { \
  (node_ptr) -> var = (v); \
  (node_ptr) -> marked = 0; \
  (node_ptr) -> shared = 0; \
  (node_ptr) -> left_low = (l) & ~((~0)<<aBCD_NumLeftLowBits); \
  (node_ptr) -> left_high = (l) >> aBCD_NumLeftLowBits; \
  (node_ptr) -> right = (r); \
} while(0)

/*---------------------------------------------------------------------------*/

#line 158 "abcd_node.tmpl"
#define aBCD_Node_signed_succ0( \
  node_ptr) \
( \
  ((node_ptr) -> left_low) | \
  (((node_ptr) -> left_high) << aBCD_NumLeftLowBits) \
)

/*---------------------------------------------------------------------------*/
#else /* aBCD_split_left_successor */
/*---------------------------------------------------------------------------*/

#line 171 "abcd_node.tmpl"
#define aBCD_is_empty_Node( \
  node_ptr) \
( \
  (node_ptr) -> left == 0 && \
  (node_ptr) -> right == 0 \
)

/*---------------------------------------------------------------------------*/

#line 182 "abcd_node.tmpl"
#define aBCD_mk_empty_Node( \
  node_ptr) \
do { \
  /* Make sure that this is equivalent to setting all bits to zero! \
   * (right now this function is just a comment since `memset' is used) \
   */ \
  (node_ptr) -> var = 0; \
  (node_ptr) -> marked = 0; \
  (node_ptr) -> shared = 0; \
  (node_ptr) -> left = 0; \
  (node_ptr) -> right = 0; \
} while(0)

/*---------------------------------------------------------------------------*/

#line 199 "abcd_node.tmpl"
#define aBCD_are_the_same_Node( \
  a, \
  b) \
( \
  (a) -> left == (b) -> left && \
  (a) -> right == (b) -> right && \
  (a) -> var == (b) -> var \
)

/*---------------------------------------------------------------------------*/

#line 213 "abcd_node.tmpl"
#define aBCD_init_node( \
  v, \
  l, \
  r, \
  node_ptr) \
do { \
  (node_ptr) -> var = (v); \
  (node_ptr) -> marked = 0; \
  (node_ptr) -> shared = 0; \
  (node_ptr) -> left = (l); \
  (node_ptr) -> right = (r); \
} while(0)

/*---------------------------------------------------------------------------*/

#line 233 "abcd_node.tmpl"
#define aBCD_Node_signed_succ0( \
  node_ptr) \
( \
  (node_ptr) -> left \
)

/*---------------------------------------------------------------------------*/
#endif /* aBCD_split_left_successor */
/*---------------------------------------------------------------------------*/

#line 245 "abcd_node.tmpl"
#define aBCD_Node_set_forward( \
  manager, \
  node_ptr, \
  destination) \
do { \
  (node_ptr) -> right = (destination); \
} while(0)

/*------------------------------------------------------------------------*/

#line 259 "abcd_node.tmpl"
#define aBCD_Node_get_forward( \
  manager, \
  node_ptr) \
( \
  (node_ptr) -> right \
)

/*===========================================================================*/
#else /* aBCD_compiler_does_bit_allocation */
/*===========================================================================*/

#line 273 "abcd_node.tmpl"
#define aBCD_is_empty_Node( \
  node_ptr) \
( \
  (node_ptr) -> data[0] == 0 && \
  (node_ptr) -> data[1] == 0 \
)

/*---------------------------------------------------------------------------*/

#line 284 "abcd_node.tmpl"
#define aBCD_mk_empty_Node( \
  node_ptr) \
do { \
  /* Make sure that this is equivalent to setting all bits to zero! \
   * (right now this function is just a comment since `memset' is used) \
   */ \
  (node_ptr) -> data[0] = 0; \
  (node_ptr) -> data[1] = 0; \
} while(0)

/*---------------------------------------------------------------------------*/

#line 298 "abcd_node.tmpl"
#define aBCD_are_the_same_Node( \
  a, \
  b) \
( \
  ((a) -> data[0] & ~aBCD_FlagsMask) == ((b) -> data[0] & ~aBCD_FlagsMask) && \
  (a) -> data[1] == (b) -> data[1] \
)

/*---------------------------------------------------------------------------*/

#line 311 "abcd_node.tmpl"
#define aBCD_Node_mark( \
  node_ptr) \
do { \
  (node_ptr) -> data[0] |= aBCD_MarkMask; \
} while(0)

/*---------------------------------------------------------------------------*/

#line 321 "abcd_node.tmpl"
#define aBCD_Node_unmark( \
  node_ptr) \
do { \
  (node_ptr) -> data[0] &= ~aBCD_MarkMask; \
} while(0)

/*---------------------------------------------------------------------------*/

#line 331 "abcd_node.tmpl"
#define aBCD_Node_is_marked( \
  node_ptr) \
( \
 ((node_ptr) -> data[0] & aBCD_MarkMask) != 0 \
)

/*---------------------------------------------------------------------------*/
#ifdef USE_SHARED
/*---------------------------------------------------------------------------*/

#line 343 "abcd_node.tmpl"
#define aBCD_Node_set_shared( \
  node_ptr) \
do { \
  (node_ptr) -> data[0] |= aBCD_SharedMask; \
} while(0)

/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/

#line 355 "abcd_node.tmpl"
#define aBCD_Node_set_shared( \
  node_ptr) \
do { \
  /* epsilon */ \
} while(0)

/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

#line 367 "abcd_node.tmpl"
#define aBCD_Node_is_shared( \
  node_ptr) \
( \
 ((node_ptr) -> data[0] & aBCD_SharedMask) != 0 \
)

/*---------------------------------------------------------------------------*/

#line 377 "abcd_node.tmpl"
#define aBCD_init_node( \
  v, \
  l, \
  r, \
  node_ptr) \
do { \
  aBCD_uint d0, d1; \
 \
  /* The variable is smaller than aBCD_MaxVar (assume 1<<10) and \
   * make up the bits 0..9 of d0. This also clears both the `mark' \
   * and the `shared' flag! \
   */ \
  d0=(v); \
 \
  /* The right index consists of 26 bits (containing the sign) that are \
   * copied to the lowest 26 bits of d0. \
   */ \
  d1=(r); \
 \
  /* The 19 lowest bits of the left idx at position 1..19 together \
   * with the sign at position 0 (which makes 20 bits) are moved to \
   * position 31..12 with a left shift of 12 and masked with the size of \
   * a 32 Bit word => should make this library functional on 64 bit \
   * machines (this is not tested yet). \
   */ \
  d0 |= ((l)<<12) & 0xFFFFFFFF;	/* Hope the compiler optimizes the \
				 * masking away for 32 bit machines. \
                                 */ \
 \
  /* The remaining 26 - 20 = 6 highest bits are the bits at position \
   * 25..20 which means they have to be shifted by 6 positions to \
   * 31..26 and masked with the highest 6 bits in a 32 bit word. \
   */ \
  d1 |= ((l)<<6) & 0xFC000000; \
 \
  (node_ptr) -> data[0] = d0; \
  (node_ptr) -> data[1] = d1; \
} while(0)

/*---------------------------------------------------------------------------*/
/* assume aBCD_MaxVar == (1<<10)
*/
#line 424 "abcd_node.tmpl"
#define aBCD_Node_var( \
  node_ptr) \
( \
  (node_ptr) -> data[0] & 0x3FF \
)

/*---------------------------------------------------------------------------*/

#line 434 "abcd_node.tmpl"
#define aBCD_Node_signed_succ0( \
  node_ptr) \
( \
  ((node_ptr) -> data[0] >> 12) \
  | \
  (((node_ptr) -> data[1] & 0xFC000000) >> 6) \
)

/*---------------------------------------------------------------------------*/

#line 446 "abcd_node.tmpl"
#define aBCD_Node_signed_succ1( \
  node_ptr) \
( \
  (node_ptr) -> data[1] & ~0xFC000000 \
)

/*------------------------------------------------------------------------*/

#line 456 "abcd_node.tmpl"
#define aBCD_Node_set_forward( \
  manager, \
  node_ptr, \
  destination) \
do { \
  (node_ptr) -> data[1] = (destination); \
} while(0)

/*------------------------------------------------------------------------*/

#line 470 "abcd_node.tmpl"
#define aBCD_Node_get_forward( \
  manager, \
  node_ptr) \
( \
  (node_ptr) -> data[1] \
)


/*===========================================================================*/
#endif /* aBCD_compiler_does_bit_allocation */
/*===========================================================================*/

#line 485 "abcd_node.tmpl"
#define aBCD_is_marked( \
  manager, \
  idx) \
( \
  aBCD_Node_is_marked(aBCD_IdxAsNodePtr(manager,idx)) \
)

/*---------------------------------------------------------------------------*/
#ifdef USE_SHARED
/*---------------------------------------------------------------------------*/

#line 499 "abcd_node.tmpl"
#define aBCD_is_shared( \
  manager, \
  idx) \
( \
  aBCD_Node_is_shared(aBCD_IdxAsNodePtr(manager,idx)) \
)

/*---------------------------------------------------------------------------*/
#ifdef aBCD_compiler_does_bit_allocation
/*---------------------------------------------------------------------------*/

#line 513 "abcd_node.tmpl"
#define aBCD_copy( \
  manager, \
  idx) \
( \
  !aBCD_is_shared((manager), (idx)) ?  \
    ((aBCD_IdxAsNodePtr((manager),(idx)) -> shared = 1) , (idx)) : \
    (idx) \
)

/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/

#line 529 "abcd_node.tmpl"
#define aBCD_copy( \
  manager, \
  idx) \
( \
  !aBCD_is_shared((manager), (idx)) ?  \
    ((aBCD_IdxAsNodePtr((manager),(idx))->data[0] |= aBCD_SharedMask), (idx)) \
    : \
    (idx) \
)

/*---------------------------------------------------------------------------*/
#endif /* aBCD_compiler_does_bit_allocation */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/

#line 550 "abcd_node.tmpl"
#define aBCD_is_shared( \
  manager, \
  idx) \
( \
  1 \
)

/*---------------------------------------------------------------------------*/

#line 562 "abcd_node.tmpl"
#define aBCD_copy( \
  manager, \
  idx) \
( \
  idx \
)

/*---------------------------------------------------------------------------*/
#endif /* USE_SHARED */
/*---------------------------------------------------------------------------*/

#line 576 "abcd_node.tmpl"
#define aBCD_Idx_split( \
  signed_idx, \
  unsigned_idx_ptr, \
  sign_ptr) \
do { \
  int tmp; \
 \
  tmp = aBCD_is_negated(signed_idx); \
  *(sign_ptr) = tmp; \
  *(unsigned_idx_ptr) = tmp ? aBCD_toggle_sign(signed_idx) : (signed_idx); \
} while(0)

/*---------------------------------------------------------------------------*/
#ifndef POWER2_SIZE
/*---------------------------------------------------------------------------*/

#line 596 "abcd_node.tmpl"
#define aBCD_hash_Node( \
  manager, \
  node_ptr, \
  result_ptr) \
do { \
  aBCD_uint tmp, var; \
 \
  /* (A) \
   * I experimented for hours to find a better hash function and could not \
   * find one. I bet that it is very dependent of aBCD_MaxIdxBits being 26 ;-) \
   */ \
  tmp = aBCD_Node_signed_succ0(node_ptr); \
  var = aBCD_Node_var(node_ptr); \
  tmp <<= 6; \
  tmp |= var >> 6; \
  tmp = tmp ^ aBCD_Node_signed_succ1(node_ptr); \
  tmp ^= var << 26; \
 \
  *(result_ptr) = tmp % (manager) -> nodes_mod_or_shift; \
} while(0)

/*---------------------------------------------------------------------------*/
#else
/*---------------------------------------------------------------------------*/

#line 625 "abcd_node.tmpl"
#define aBCD_hash_Node( \
  manager, \
  node_ptr, \
  result_ptr) \
do { \
  *(result_ptr) = aBCD_hash3( \
    aBCD_Node_signed_succ0(node_ptr), \
    aBCD_Node_signed_succ1(node_ptr), \
    aBCD_Node_var(node_ptr), \
    manager -> nodes_mod_or_shift); \
} while(0)

/*---------------------------------------------------------------------------*/
#endif
/*---------------------------------------------------------------------------*/

#line 645 "abcd_node.tmpl"
#define aBCD_Idx_var( \
  manager, \
  idx) \
( \
  aBCD_Node_var(aBCD_IdxAsNodePtr(manager,idx)) \
)

/*---------------------------------------------------------------------------*/

#line 657 "abcd_node.tmpl"
#define aBCD_Idx_succ0( \
  manager, \
  idx) \
( \
  aBCD_Node_signed_succ0(aBCD_IdxAsNodePtr(manager,idx)) \
)

/*---------------------------------------------------------------------------*/

#line 669 "abcd_node.tmpl"
#define aBCD_Idx_succ1( \
  manager, \
  idx) \
( \
  aBCD_Node_signed_succ1(aBCD_IdxAsNodePtr(manager,idx)) \
)

/*---------------------------------------------------------------------------*/

#line 681 "abcd_node.tmpl"
#define aBCD_Idx_is_constant( \
  idx) \
( \
	(idx) == 0 || (idx) == 1 \
)

/*---------------------------------------------------------------------------*/

#line 691 "abcd_node.tmpl"
#define aBCD_Idx_is_variable( \
  manager, \
  idx) \
( \
  aBCD_Idx_is_constant(aBCD_Idx_succ0(manager, idx)) \
  && \
  aBCD_Idx_is_constant(aBCD_Idx_succ1(manager, idx)) \
)

/*---------------------------------------------------------------------------*/

#line 705 "abcd_node.tmpl"
#define aBCD_Idx_cofactor1( \
  manager, \
  idx, \
  v, \
  l, \
  r) \
do { \
  struct aBCD_Node * node_ptr; \
 \
  node_ptr = aBCD_IdxAsNodePtr(manager,idx); \
  *(v) = aBCD_Node_var(node_ptr); \
 \
  if(aBCD_is_negated(idx)) \
    { \
      *(l) = aBCD_toggle_sign(aBCD_Node_signed_succ0(node_ptr)); \
      *(r) = aBCD_toggle_sign(aBCD_Node_signed_succ1(node_ptr)); \
    } \
  else \
    { \
      *(l) = aBCD_Node_signed_succ0(node_ptr); \
      *(r) = aBCD_Node_signed_succ1(node_ptr); \
    } \
} while(0)

/*---------------------------------------------------------------------------*/

#line 737 "abcd_node.tmpl"
#define aBCD_Idx_cofactor2( \
  manager, \
  f, \
  g, \
  m, \
  f0, \
  f1, \
  g0, \
  g1) \
do { \
  struct aBCD_Node * f_node_ptr, * g_node_ptr; \
  aBCD_uint f_var, g_var; \
 \
  f_node_ptr = aBCD_IdxAsNodePtr(manager,f); \
  f_var = aBCD_Node_var(f_node_ptr);  \
  g_node_ptr = aBCD_IdxAsNodePtr(manager,g); \
  g_var = aBCD_Node_var(g_node_ptr);  \
 \
  *(m) = aBCD_min2(f_var, g_var); \
 \
  if(f_var==*(m)) \
    { \
      if(aBCD_is_negated(f)) \
        { \
          *(f0) = aBCD_toggle_sign(aBCD_Node_signed_succ0(f_node_ptr)); \
          *(f1) = aBCD_toggle_sign(aBCD_Node_signed_succ1(f_node_ptr)); \
	} \
      else \
        { \
          *(f0) = aBCD_Node_signed_succ0(f_node_ptr); \
          *(f1) = aBCD_Node_signed_succ1(f_node_ptr); \
	} \
    } \
  else \
    { \
      *(f0) = f; \
      *(f1) = f; \
    } \
 \
  if(g_var==*(m)) \
    { \
      if(aBCD_is_negated(g)) \
        { \
          *(g0) = aBCD_toggle_sign(aBCD_Node_signed_succ0(g_node_ptr)); \
          *(g1) = aBCD_toggle_sign(aBCD_Node_signed_succ1(g_node_ptr)); \
	} \
      else \
        { \
          *(g0) = aBCD_Node_signed_succ0(g_node_ptr); \
          *(g1) = aBCD_Node_signed_succ1(g_node_ptr); \
        } \
    } \
  else \
    { \
      *(g0) = g; \
      *(g1) = g; \
    } \
} while(0)

/*---------------------------------------------------------------------------*/

#line 807 "abcd_node.tmpl"
#define aBCD_Idx_cofactor3( \
  manager, \
  f, \
  g, \
  h, \
  m, \
  f0, \
  f1, \
  g0, \
  g1, \
  h0, \
  h1) \
do { \
  struct aBCD_Node * f_node_ptr, * g_node_ptr, * h_node_ptr; \
  aBCD_uint f_var, g_var , h_var; \
 \
  f_node_ptr = aBCD_IdxAsNodePtr(manager,f); \
  f_var = aBCD_Node_var(f_node_ptr);  \
  g_node_ptr = aBCD_IdxAsNodePtr(manager,g); \
  g_var = aBCD_Node_var(g_node_ptr);  \
  h_node_ptr = aBCD_IdxAsNodePtr(manager,h); \
  h_var = aBCD_Node_var(h_node_ptr);  \
 \
  *(m) = aBCD_min3(f_var, g_var, h_var); \
 \
  if(f_var==*(m)) \
    { \
      if(aBCD_is_negated(f)) \
        { \
          *(f0) = aBCD_toggle_sign(aBCD_Node_signed_succ0(f_node_ptr)); \
          *(f1) = aBCD_toggle_sign(aBCD_Node_signed_succ1(f_node_ptr)); \
	} \
      else \
        { \
          *(f0) = aBCD_Node_signed_succ0(f_node_ptr); \
          *(f1) = aBCD_Node_signed_succ1(f_node_ptr); \
	} \
    } \
  else \
    { \
      *(f0) = f; \
      *(f1) = f; \
    } \
 \
  if(g_var==*(m)) \
    { \
      if(aBCD_is_negated(g)) \
        { \
          *(g0) = aBCD_toggle_sign(aBCD_Node_signed_succ0(g_node_ptr)); \
          *(g1) = aBCD_toggle_sign(aBCD_Node_signed_succ1(g_node_ptr)); \
	} \
      else \
        { \
          *(g0) = aBCD_Node_signed_succ0(g_node_ptr); \
          *(g1) = aBCD_Node_signed_succ1(g_node_ptr); \
        } \
    } \
  else \
    { \
      *(g0) = g; \
      *(g1) = g; \
    } \
 \
  if(h_var==*(m)) \
    { \
      if(aBCD_is_negated(h)) \
        { \
          *(h0) = aBCD_toggle_sign(aBCD_Node_signed_succ0(h_node_ptr)); \
          *(h1) = aBCD_toggle_sign(aBCD_Node_signed_succ1(h_node_ptr)); \
	} \
      else \
        { \
          *(h0) = aBCD_Node_signed_succ0(h_node_ptr); \
          *(h1) = aBCD_Node_signed_succ1(h_node_ptr); \
        } \
    } \
  else \
    { \
      *(h0) = h; \
      *(h1) = h; \
    } \
} while(0)

/*---------------------------------------------------------------------------*/
/* do not normalize -> can be used for variable sets, substitutions,
* and for constructing BDDs without negated edges.
*/

#line 907 "abcd_node.tmpl"
#define aBCD_find_Idx_aux( \
  manager, \
  v, \
  l_ptr, \
  r_ptr, \
  result) \
do { \
  struct aBCD_Node node, * p, * last; \
  aBCD_uint hash_val, inc; \
 \
  aBCD_init_node(v,*(l_ptr),*(r_ptr),&node); \
  aBCD_hash_Node(manager, &node, &hash_val); \
  if(hash_val==0) hash_val++;				/* skip constants */ \
  p = &manager -> nodes[hash_val]; \
  last = manager -> nodes + manager -> nodes_size; \
 \
  aBCD_inc_stats(manager, nodes_searches); \
  inc = 0;  \
 \
  for(;;) \
    { \
      if(aBCD_are_the_same_Node(&node, p)) \
        { \
	  aBCD_inc_distribution_visited(manager); \
          aBCD_inc_distribution_hit(manager); \
 \
	  aBCD_inc_stats(manager, nodes_visited); \
	  aBCD_inc_stats(manager, nodes_hits); \
 \
	  if(!aBCD_Node_is_shared(p)) aBCD_Node_set_shared(p); \
          *(result) = aBCD_NodePtrAsIdx(manager, p); \
 \
	  break; \
	} \
      else \
      if(aBCD_is_empty_Node(p)) \
        { \
	  /* Generate a new node but check if we first have to collect \
	   * garbage nodes. Reading an empty node is not considered \
	   * to be a visit of that node. This is questionable because \
	   * for `abcd' we really have to read the whole node before we \
	   * can decide if it is empty. \
	   */ \
 \
          aBCD_inc_distribution_miss(manager); \
 \
	  if(aBCD_check_for_garbage(manager)) \
	    { \
	      /* `result' is assigned by `aBCD_garbage_collection'. Also \
	       * the node count is increased. \
	       */ \
	      aBCD_garbage_collection(manager, v, l_ptr, r_ptr, result); \
	    } \
	  else \
	    { \
	      *p = node; \
              *(result) = aBCD_NodePtrAsIdx(manager, p); \
	      manager -> nodes_count++; \
	    } \
 \
          aBCD_adjust_distribution(manager); \
	   \
	  /* By default the `shared' flag of the result is cleared. \
	   */ \
 \
	  break; \
	} \
      else \
        { \
	  aBCD_inc_distribution_visited(manager); \
	  aBCD_inc_stats(manager, nodes_visited); \
          if(inc == 0)  \
	    { \
	      inc = aBCD_double_hash(manager, v, *(l_ptr), *(r_ptr)); \
	      if(!inc) inc = 1; \
	    } \
	  p += inc; \
	  if(p>=last) p = manager -> nodes + 1;		/* skip constant */ \
	} \
    } \
} while(0)

/*---------------------------------------------------------------------------*/

#line 997 "abcd_node.tmpl"
#define aBCD_find_Idx( \
  manager, \
  v, \
  l_ptr, \
  r_ptr, \
  result) \
do { \
  if(*(l_ptr) != *(r_ptr)) \
    { \
      if(aBCD_is_negated(*(l_ptr))) \
        { \
	  *(l_ptr) = aBCD_toggle_sign(*(l_ptr)); \
	  *(r_ptr) = aBCD_toggle_sign(*(r_ptr)); \
          aBCD_find_Idx_aux(manager,v,l_ptr,r_ptr,result); \
	  *(result) = aBCD_toggle_sign(*result); \
	  *(r_ptr) = aBCD_toggle_sign(*(r_ptr)); \
	  *(l_ptr) = aBCD_toggle_sign(*(l_ptr)); \
	} \
      else aBCD_find_Idx_aux(manager,v,l_ptr,r_ptr,result); \
    } \
  else *(result) = *(l_ptr); \
} while(0)

/*---------------------------------------------------------------------------*/

#endif
