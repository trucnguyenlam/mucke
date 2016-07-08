#ifndef _dyn_arr_h_INCLUDED
#define _dyn_arr_h_INCLUDED

template <class T>
class DynArray
{
  long _size;
  T* _array;

  void grow(long index)
    {
      T* newarray;
      long newsize,i;

      for (newsize=_size;index >= newsize; newsize+=_size);
      
      newarray = new T [newsize];
      for (i=0;i<_size;i++)
	newarray[i] = _array[i];
      for (i=_size;i<newsize;i++)
	newarray[i] = 0;

      delete [] _array;
      _array = newarray;
      _size = newsize;
    }

public:
  
  DynArray(int s) : _size(s), _array(new T [s])
    {
      long i;
      
      for (i=0;i<_size;i++)
	_array[i] = 0;
    }
  
  T& operator[] (long index)
    {
      if (index>=_size)
	grow(index);

      return _array[index];
    }

  long length() { return _size; }
  
  ~DynArray()
    {
      delete [] _array;
    }

};

#endif
