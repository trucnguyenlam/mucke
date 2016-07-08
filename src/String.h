#ifndef _String_h_INCLUDED
#define _String_h_INCLUDED

/* (C) 1996-1997, 1998 Armin Biere, University of Karlsruhe
 */

/*------------------------------------------------------------------------*/

#include "config.h"

/*------------------------------------------------------------------------*/

inline int length(const char * a)
{
  if(!a) return -1;
  int res;

  for(res=0; *a; a++, res++)
    ;

  return res;
}

/*------------------------------------------------------------------------*/

inline void copy(char * dest, const char * src)
{
  if(!src) return;
  while(*src) *dest++ =  *src++;
  *dest = '\0';
}

/*------------------------------------------------------------------------*/

inline char * duplicate(const char * str)
{
  char * res;

  res = new char [ length(str) + 1 ];
  copy(res, str);

  return res;
}

/*------------------------------------------------------------------------*/

inline bool cmp(const char * a, const char * b)
{
  if(!a) return !b;
  if(!b) return false;

  for(; *a && *a==*b; a++, b++)
    ;

  return !*a && !*b ? true : false;
}

/*------------------------------------------------------------------------*/

inline int ATOI(const char * p)
{
  int res;
  for(res=0; *p; p++)
    res = 10*res + *p - '0';
  return res;
}

/*------------------------------------------------------------------------*/

template<class T> inline const char * uITOA(T i)	// unsigned version
{
  const unsigned int size = 40; // big enough for 128 bit machines
  static char buffer[size];

  char * p = buffer+size-1;
  *p-- = '\0';

  if(i==0) { *p = '0'; return p; }

  for(; i!=0; p--)
    {
      *p = '0' + i % 10;
      i /= 10;
    }
  
  return p+1;
}

/*------------------------------------------------------------------------*/

template<class T> inline const char * ITOA(T i)	// signed version
{
  const unsigned int size = 40; // big enough for 128 bit machines
  static char buffer[size];

  char * p = buffer+size-1;
  *p-- = '\0';

  if(i==0) { *p = '0'; return p; }

  bool sign = (i<0);
  if(sign) i = -i;

  for(; i!=0; p--)
    {
      *p = '0' + i % 10;
      i /= 10;
    }
  
  if(sign) 
     {
        *p = '-'; return p;
     }
  else return p+1;
}

/*------------------------------------------------------------------------*/

inline char * append(const char * a, const char * b)
{
  if(!a||!b) return 0;

  char * p, * res = new char [ length(a) + length(b) + 1 ];
  p = res;

  while(*a) *p++ = *a++;
  while(*b) *p++ = *b++;

  *p = '\0';

  return res;
}

#endif
