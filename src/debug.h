#ifndef _debug_h_INCLUDED
#define _debug_h_INCLUDED

#ifdef DEBUG

#define DEBUG_EMPTY_DECL ;
#define DEBUG_ZERO_DECL ;

extern void _failed_assertion(const char *, int);

#define ASSERT(a) { if(!(a)) _failed_assertion(__FILE__,__LINE__); }

#else

#define DEBUG_EMPTY_DECL { }
#define DEBUG_ZERO_DECL { return 0; }

#define ASSERT(a) { }

#endif

#endif
