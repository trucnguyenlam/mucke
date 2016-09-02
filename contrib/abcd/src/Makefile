TARGETS=../lib/libabcd.a ../bin/abcd
all: dirs ../bin/generate config.h show-flags $(TARGETS)
################################################################
CC=gcc
DEBUG=-g#-pg# do not define -DDEBUG here !
WARNINGS=-Wall -pedantic
#WARNINGS=-W
OPTIMIZE=-O3 -fexpensive-optimizations -fomit-frame-pointer
#OPTIMIZE=-O2
# OPTIMIZE=-O3
BASICCFLAGS=$(DEBUG)
CFLAGS=$(BASICCFLAGS) -fPIC $(WARNINGS) $(OPTIMIZE)
LD=gcc
LDFLAGS=#-pg
#LIBS=-L../lib -lccmalloc -ldl
#LIBS=-lefence
################################################################
OBJS= \
  ../obj/ABCD.o \
  ../obj/abcd.o \
  ../obj/abcd_and.o \
  ../obj/abcd_cache.o \
  ../obj/abcd_cofactor.o \
  ../obj/abcd_compose.o \
  ../obj/abcd_dump.o \
  ../obj/abcd_equiv.o \
  ../obj/abcd_exists.o \
  ../obj/abcd_fraction.o \
  ../obj/abcd_intersects.o \
  ../obj/abcd_ite.o \
  ../obj/abcd_manager.o \
  ../obj/abcd_manager_int.o \
  ../obj/abcd_node.o \
  ../obj/abcd_onecube.o \
  ../obj/abcd_print.o \
  ../obj/abcd_reduce.o \
  ../obj/abcd_relprod.o \
  ../obj/abcd_support.o \
  ../obj/abcd_stats.o \
  ../obj/abcd_undump.o \
  ../obj/abcd_unnegate.o \
  ../obj/abcd_util.o \
  ../obj/abcd_visualize.o
################################################################
../obj/abcd.o: abcd.c
	@echo "[compiling abcd.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd.c
../obj/abcd_and.o: abcd_and.c
	@echo "[compiling abcd_and.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_and.c
../obj/abcd_cache.o: abcd_cache.c
	@echo "[compiling abcd_cache.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_cache.c
../obj/abcd_cofactor.o: abcd_cofactor.c
	@echo "[compiling abcd_cofactor.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_cofactor.c
../obj/abcd_compose.o: abcd_compose.c
	@echo "[compiling abcd_compose.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_compose.c
../obj/abcd_dump.o: abcd_dump.c
	@echo "[compiling abcd_dump.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_dump.c
../obj/abcd_equiv.o: abcd_equiv.c
	@echo "[compiling abcd_equiv.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_equiv.c
../obj/abcd_exists.o: abcd_exists.c
	@echo "[compiling abcd_exists.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_exists.c
../obj/abcd_fraction.o: abcd_fraction.c
	@echo "[compiling abcd_fraction.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_fraction.c
../obj/abcd_intersects.o: abcd_intersects.c
	@echo "[compiling abcd_intersects.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_intersects.c
../obj/abcd_ite.o: abcd_ite.c
	@echo "[compiling abcd_ite.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_ite.c
../obj/abcd_manager.o: abcd_manager.c
	@echo "[compiling abcd_manager.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_manager.c
../obj/abcd_manager_int.o: abcd_manager_int.c
	@echo "[compiling abcd_manager_int.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_manager_int.c
../obj/abcd_node.o: abcd_node.c
	@echo "[compiling abcd_node.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_node.c
../obj/abcd_onecube.o: abcd_onecube.c
	@echo "[compiling abcd_onecube.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_onecube.c
../obj/abcd_print.o: abcd_print.c
	@echo "[compiling abcd_print.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_print.c
../obj/abcd_reduce.o: abcd_reduce.c
	@echo "[compiling abcd_reduce.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_reduce.c
../obj/abcd_relprod.o: abcd_relprod.c
	@echo "[compiling abcd_relprod.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_relprod.c
../obj/abcd_stats.o: abcd_stats.c
	@echo "[compiling abcd_stats.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_stats.c
../obj/abcd_support.o: abcd_support.c
	@echo "[compiling abcd_support.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_support.c
../obj/abcd_undump.o: abcd_undump.c
	@echo "[compiling abcd_undump.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_undump.c
../obj/abcd_unnegate.o: abcd_unnegate.c
	@echo "[compiling abcd_unnegate.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_unnegate.c
../obj/abcd_util.o: abcd_util.c
	@echo "[compiling abcd_util.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_util.c
../obj/abcd_visualize.o: abcd_visualize.c
	@echo "[compiling abcd_visualize.c]"; \
	$(CC) $(CFLAGS) -o $@ -c abcd_visualize.c
../obj/test.o: test.c
	@echo "[compiling test.c]"; \
	$(CC) $(CFLAGS) -o $@ -c test.c
../obj/ABCD.o: ABCD.c
	@echo "[compiling ABCD.c]"; \
	$(CC) $(CFLAGS) -o $@ -c ABCD.c
################################################################
SRCS= \
  ABCD.h \
  abcd.h \
  abcd_cache.h \
  abcd_cache_int.h \
  abcd_manager.h \
  abcd_manager_int.h \
  abcd_node.h \
  abcd_node_int.h \
  abcd_types.h \
  abcd_types_int.h \
  abcd_util.h \
  config.h \
  \
  ABCD.c \
  abcd.c \
  abcd_and.c \
  abcd_cache.c \
  abcd_cofactor.c \
  abcd_compose.c \
  abcd_dump.c \
  abcd_equiv.c \
  abcd_exists.c \
  abcd_fraction.c \
  abcd_intersects.c \
  abcd_ite.c \
  abcd_manager.c \
  abcd_manager_int.c \
  abcd_node.c \
  abcd_onecube.c \
  abcd_print.c \
  abcd_reduce.c \
  abcd_relprod.c \
  abcd_stats.c \
  abcd_support.c \
  abcd_unnegate.c \
  abcd_util.c \
  abcd_visualize.c \
  test.c
################################################################
TEMPLATES= \
  abcd_cache.tmpl \
  abcd_manager.tmpl \
  abcd_node.tmpl
################################################################
.SUFFIXES: .tmpl
.tmpl.h:
	../bin/generate $@
.tmpl.c:
	../bin/generate $@
.c.o:
	@echo "compiling " $*.c; \
	$(CC) $(CFLAGS) -o $@ -c $*.c
dirs:
	@for d in obj bin lib; \
	do \
	  if test ! -d ../$$d; then mkdir ../$$d; fi; \
	done
../bin/generate:
	$(CC) -g -o $@ generate.c
show-flags:
	@echo "CC     = $(CC)"
	@echo "CFLAGS = $(CFLAGS)"
	@sed -e '/[ 	]*#/d' config.cache
	@echo
../bin/abcd: ../lib/libabcd.a ../obj/test.o
	@echo "[linking $@]"; \
	t=`basename $@`; \
	$(LD) -o /tmp/$$t $(LDFLAGS) ../obj/test.o -L../lib -labcd $(LIBS); \
	mv /tmp/$$t $@

#	$(LD) -o $@ $(LDFLAGS) ../obj/test.o -L../lib -labcd $(LIBS)

../lib/libabcd.a: $(OBJS)
	@echo "[generating $@]"
	@ar rc $@ $(OBJS)
	@ranlib $@
../lib/libabcd.so: $(OBJS)
	gcc -shared -Wl,-soname,$@ -o $@ $(OBJS)
config.h:
	./configure
	make depend
depend: ../bin/generate $(SRCS)
	@echo "making dependencies"; \
	./mkdepend $(BASICCFLAGS) $(SRCS)
rm-build-srcs:
	for tmpl in $(TEMPLATES); \
	do \
	  base=`basename $$tmpl .tmpl`; \
	  rm -f $${base}.c $${base}.h; \
	done
clean: rm-build-srcs
	@echo "removing objects, libraries, and targets"; \
	rm -f ../obj/*.o
	rm -f ../lib/libabcd.a ../lib/libabcd.so
	rm -f ../bin/abcd
	rm -f abcd*.lg abcd*.ps
	rm -f ../bin/generate
distclean: clean
	makedepend 1>/dev/null 2>/dev/null
	rm -f config.h config.cache ../bin/generate
	rm -f *~ Makefile.bak
# DO NOT DELETE
