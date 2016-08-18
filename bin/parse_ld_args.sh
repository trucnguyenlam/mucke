#!/bin/bash

arg=$*
target=$1
libs=""
stdlibs=""
flags=""
dirs=""
shift

while test ! x"$1" = x
do
  case $1 in
    -L*)
       dir=`echo $1 | sed -e 's,-L,,'`
       if test ! -d $dir
       then
         echo "*** $0: '$dir' is no directory" 2>&1
	 exit 1
       fi
       dirs="$dirs $dir"
       ;;
    -l*)
       lib=`echo $1 | sed -e 's,-l,lib,' -e 's,$,.a,'`
       found=false
       for d in $dir
       do
         if test -f ${d}/$lib
         then
           libs="$libs ${d}/$lib"
	   found=true
	   break
         fi
       done
       if test $found = false
       then
         stdlibs="$stdlibs $lib"
       fi
       ;;
    -*)
       flags="$flags $1"
       ;;
    *)
       objs="$objs $1"
       ;;
  esac
  shift
done

if test x"$DEBUG" = x1
then
  echo \
"----------------------------------------------------------------------------"
  echo "target  = $target"
  echo "libs    =$libs"
  echo "stdlibs =$stdlibs"
  echo "flags   =$flags"
  echo "dirs    =$dirs"
  echo "objs    =$objs" | \
    sed \
      -e 's,\.\./obj/LINK/,,g' \
      -e 's,\.o,,g' | \
    fmt
  echo \
"----------------------------------------------------------------------------"
fi
