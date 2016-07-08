#ifndef _macros_h_INCLUDED
#define _macros_h_INCLUDED

template<class T> inline
void swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }

template<class T> inline
const T & min(const T & a, const T & b) { return (b<a) ? b : a; }

template<class T> inline
const T & min(const T & a, const T & b, const T & c)
{ return (b<a) ? (c<b ? c : b) : (c<a ? c : a); }

template<class T> inline
void save_delete(T* & ptr)
{
  if(ptr)
    {
      T * tmp = ptr;
      ptr = 0;
      delete tmp;
    }
}

inline bool less(int a, int b) { return a < b; }

template<class T> inline
void reverse_bubblesort(T * a, int sz)
{
  bool changed;

  do {
    changed = false;
    for(int i = 0; i<sz-1; i++)
      {
        if(less(a[i],a[i+1]))
          {
            swap(a[i], a[i+1]);
            changed = true;
          }
      }
  } while(changed);

  // now the largest entry is at a[0]
}

extern int ld_ceiling_table[];

inline int ld_ceiling32(int a)
{
  if(a<0) return -1;
  if(a&0xFFFF0000)
    {
      if(a&0xFF000000) return ld_ceiling_table[a>>24] + 24;
      else return ld_ceiling_table[a>>16] + 16;
    }
  else
    {
      if(a&0x0000FF00) return ld_ceiling_table[a>>8] + 8;
      else return ld_ceiling_table[a];
    }
}

#endif
