/*==========================================================================
 *  (C) 1997-98 Armin Biere 
 *  $Id: abcd_types_int.h,v 1.2 2000-05-08 11:33:36 biere Exp $
 *==========================================================================
 */

#ifndef _abcd_types_int_h_INCLUDED
#define _abcd_types_int_h_INCLUDED

#include "abcd_types.h"

#define aBCD_sign(i) ((i) & 1)
#define aBCD_toggle_sign(i) ((i) ^ 1)
#define aBCD_is_negated(i) (((i) & 1) != 0)

#endif
