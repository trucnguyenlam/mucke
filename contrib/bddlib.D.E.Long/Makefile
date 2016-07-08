# This is a make file for Linux + Solaris to make a shared
# version of the bdd library of D.E. Long. 
#
# (1) Type ./configure <dir> where <dir> is the directory where you
# unpacked the bddlib.tar.gz archive.
#
# (2) Link your binary files directory (e.g. `../../bin/linux-intel') to
# `../../bin/LINK'
#
# (3) make
#
# You can get the archive from http://www.cs.cmu.edu/~modelcheck.
# This library comes in a tar-archive `bddlib.tar' and with
# `tar xf bddlib.tar' you will get
# 
#   mem/dist.tar
#   bddlib/dist.tar
#   boole/dist.tar
#   trlib/dist.tar
#
# Untar these archives in the respective directories.

CC=gcc
INCL=-I.#-I../mem
DEFS=#-DUSE_MALLOC_FREE

#CFLAGS = $(DEFS) $(INCL) -fPIC -g
CFLAGS = $(DEFS) $(INCL) -fPIC -O2 -fomit-frame-pointer
CFLAGS = $(DEFS) $(INCL) -fPIC -O3

OBJS=bdd.o bddapply.o bddassoc.o bddblk.o bddcache.o bddcmp.o bddcomp.o \
 bdddump.o bddhash.o bddmisc.o bddprimes.o bddprint.o bddprprofile.o \
 bddqnt.o bddreduce.o bddrelprod.o bddreorder.o bddsat.o bddsize.o \
 bddsupport.o bddswap.o bddunique.o bddwarn.o mtbdd.o memblock.o memrec.o

all:
	make $(OBJS)
	../../bin/ldshared \
	../../bin/`../../bin/barch`/bddlib.D.E.Long.so $(OBJS)

../../bin/LINK/bddlib.D.E.Long.so: $(OBJS)
	../../bin/ldshared $@ $(OBJS)

../../bin/LINK/libbdd.a: $(OBJS)
	ar cr $@ $(OBJS)
	ranlib $@
clean:
	rm -f $(OBJS)

distclean: clean
	rm -f ../../bin/LINK/bddlib.D.E.Long.so
	rm -f ../../bin/LINK/libbdd.a
	rm -f bdd.c bddapply.c bddassoc.c bddblk.c bddcache.c bddcmp.c \
	bddcomp.c bdddump.c bddhash.c bddmisc.c bddprimes.c bddprint.c \
	bddprprofile.c bddqnt.c bddreduce.c bddrelprod.c bddreorder.c \
	bddsat.c bddsize.c bddsupport.c bddswap.c bddunique.c bddwarn.c \
	mtbdd.c memblock.c memrec.c memuser.h memint.h bdduser.h bddint.h

.PHONY: all clean distclean
