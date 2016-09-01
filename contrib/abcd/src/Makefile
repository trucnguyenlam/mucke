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

../obj/ABCD.o: abcd_types.h /usr/include/stdio.h /usr/include/sys/cdefs.h
../obj/ABCD.o: /usr/include/sys/_symbol_aliasing.h
../obj/ABCD.o: /usr/include/sys/_posix_availability.h /usr/include/Availability.h
../obj/ABCD.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/ABCD.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
../obj/ABCD.o: /usr/include/i386/_types.h /usr/include/sys/_pthread/_pthread_types.h
../obj/ABCD.o: /usr/include/sys/_types/_va_list.h /usr/include/sys/_types/_size_t.h
../obj/ABCD.o: /usr/include/sys/_types/_null.h /usr/include/sys/stdio.h
../obj/ABCD.o: /usr/include/sys/_types/_off_t.h /usr/include/sys/_types/_ssize_t.h
../obj/ABCD.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
../obj/abcd.o: abcd_types.h /usr/include/stdio.h /usr/include/sys/cdefs.h
../obj/abcd.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd.o: /usr/include/sys/_posix_availability.h /usr/include/Availability.h
../obj/abcd.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
../obj/abcd.o: /usr/include/i386/_types.h /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd.o: /usr/include/sys/_types/_va_list.h /usr/include/sys/_types/_size_t.h
../obj/abcd.o: /usr/include/sys/_types/_null.h /usr/include/sys/stdio.h
../obj/abcd.o: /usr/include/sys/_types/_off_t.h /usr/include/sys/_types/_ssize_t.h
../obj/abcd.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
../obj/abcd_cache.o: abcd_cache_int.h abcd_types_int.h abcd_types.h abcd_manager.h
../obj/abcd_cache.o: abcd_manager_int.h config.h abcd_node.h abcd_node_int.h
../obj/abcd_cache.o: abcd_util.h /usr/include/stdlib.h /usr/include/Availability.h
../obj/abcd_cache.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_cache.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../obj/abcd_cache.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_cache.o: /usr/include/sys/_posix_availability.h
../obj/abcd_cache.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_cache.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_cache.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_cache.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_cache.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_cache.o: /usr/include/i386/_mcontext.h /usr/include/mach/i386/_structs.h
../obj/abcd_cache.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_cache.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_cache.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_cache.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_cache.o: /usr/include/_types/_uint8_t.h /usr/include/_types/_uint16_t.h
../obj/abcd_cache.o: /usr/include/_types/_uint32_t.h /usr/include/_types/_uint64_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_cache.o: /usr/include/_types/_intmax_t.h
../obj/abcd_cache.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_cache.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_cache.o: /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h
../obj/abcd_cache.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
../obj/abcd_cache.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
../obj/abcd_cache.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_mode_t.h
../obj/abcd_cache_int.o: abcd_types_int.h abcd_types.h
../obj/abcd_manager.o: abcd_manager_int.h config.h abcd_node.h abcd_manager.h
../obj/abcd_manager.o: abcd_node_int.h abcd_types_int.h abcd_types.h abcd_util.h
../obj/abcd_manager.o: /usr/include/stdlib.h /usr/include/Availability.h
../obj/abcd_manager.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_manager.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../obj/abcd_manager.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_manager.o: /usr/include/sys/_posix_availability.h
../obj/abcd_manager.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_manager.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_manager.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_manager.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_manager.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_manager.o: /usr/include/i386/_mcontext.h
../obj/abcd_manager.o: /usr/include/mach/i386/_structs.h
../obj/abcd_manager.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_manager.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_manager.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_manager.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_manager.o: /usr/include/_types/_uint8_t.h
../obj/abcd_manager.o: /usr/include/_types/_uint16_t.h
../obj/abcd_manager.o: /usr/include/_types/_uint32_t.h
../obj/abcd_manager.o: /usr/include/_types/_uint64_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_manager.o: /usr/include/_types/_intmax_t.h
../obj/abcd_manager.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_manager.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_manager.o: /usr/include/sys/_endian.h
../obj/abcd_manager.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_manager.o: /usr/include/libkern/i386/_OSByteOrder.h
../obj/abcd_manager.o: /usr/include/alloca.h /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
../obj/abcd_manager.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_mode_t.h
../obj/abcd_manager_int.o: config.h abcd_node.h abcd_manager.h abcd_manager_int.h
../obj/abcd_manager_int.o: abcd_types_int.h abcd_types.h abcd_node_int.h abcd_util.h
../obj/abcd_manager_int.o: /usr/include/stdlib.h /usr/include/Availability.h
../obj/abcd_manager_int.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_manager_int.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../obj/abcd_manager_int.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_manager_int.o: /usr/include/sys/_posix_availability.h
../obj/abcd_manager_int.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_manager_int.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_manager_int.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_manager_int.o: /usr/include/sys/appleapiopts.h
../obj/abcd_manager_int.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
../obj/abcd_manager_int.o: /usr/include/machine/_mcontext.h
../obj/abcd_manager_int.o: /usr/include/i386/_mcontext.h
../obj/abcd_manager_int.o: /usr/include/mach/i386/_structs.h
../obj/abcd_manager_int.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_uid_t.h
../obj/abcd_manager_int.o: /usr/include/sys/resource.h /usr/include/stdint.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_int8_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_manager_int.o: /usr/include/_types/_uint8_t.h
../obj/abcd_manager_int.o: /usr/include/_types/_uint16_t.h
../obj/abcd_manager_int.o: /usr/include/_types/_uint32_t.h
../obj/abcd_manager_int.o: /usr/include/_types/_uint64_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_manager_int.o: /usr/include/_types/_intmax_t.h
../obj/abcd_manager_int.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_manager_int.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_manager_int.o: /usr/include/sys/_endian.h
../obj/abcd_manager_int.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_manager_int.o: /usr/include/libkern/i386/_OSByteOrder.h
../obj/abcd_manager_int.o: /usr/include/alloca.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_null.h
../obj/abcd_manager_int.o: /usr/include/machine/types.h /usr/include/i386/types.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_dev_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_mode_t.h
../obj/abcd_node.o: config.h abcd_manager.h abcd_manager_int.h abcd_node.h
../obj/abcd_node.o: abcd_node_int.h abcd_types_int.h abcd_types.h abcd_util.h
../obj/abcd_node.o: /usr/include/stdlib.h /usr/include/Availability.h
../obj/abcd_node.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_node.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../obj/abcd_node.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_node.o: /usr/include/sys/_posix_availability.h
../obj/abcd_node.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_node.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_node.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_node.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_node.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_node.o: /usr/include/i386/_mcontext.h /usr/include/mach/i386/_structs.h
../obj/abcd_node.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_node.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_node.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_node.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_node.o: /usr/include/_types/_uint8_t.h /usr/include/_types/_uint16_t.h
../obj/abcd_node.o: /usr/include/_types/_uint32_t.h /usr/include/_types/_uint64_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_node.o: /usr/include/_types/_intmax_t.h /usr/include/_types/_uintmax_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_timeval.h /usr/include/machine/endian.h
../obj/abcd_node.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
../obj/abcd_node.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_node.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
../obj/abcd_node.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
../obj/abcd_node.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_mode_t.h
../obj/abcd_node_int.o: config.h abcd_types_int.h abcd_types.h
../obj/abcd_types_int.o: abcd_types.h
../obj/abcd_util.o: /usr/include/stdlib.h /usr/include/Availability.h
../obj/abcd_util.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_util.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../obj/abcd_util.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_util.o: /usr/include/sys/_posix_availability.h
../obj/abcd_util.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_util.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_util.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_util.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_util.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_util.o: /usr/include/i386/_mcontext.h /usr/include/mach/i386/_structs.h
../obj/abcd_util.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_util.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_util.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_util.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_util.o: /usr/include/_types/_uint8_t.h /usr/include/_types/_uint16_t.h
../obj/abcd_util.o: /usr/include/_types/_uint32_t.h /usr/include/_types/_uint64_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_util.o: /usr/include/_types/_intmax_t.h /usr/include/_types/_uintmax_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_timeval.h /usr/include/machine/endian.h
../obj/abcd_util.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
../obj/abcd_util.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_util.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
../obj/abcd_util.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
../obj/abcd_util.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_mode_t.h config.h abcd_types.h
../obj/ABCD.o: config.h abcd.h abcd_types.h /usr/include/stdio.h
../obj/ABCD.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
../obj/ABCD.o: /usr/include/sys/_posix_availability.h /usr/include/Availability.h
../obj/ABCD.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/ABCD.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
../obj/ABCD.o: /usr/include/i386/_types.h /usr/include/sys/_pthread/_pthread_types.h
../obj/ABCD.o: /usr/include/sys/_types/_va_list.h /usr/include/sys/_types/_size_t.h
../obj/ABCD.o: /usr/include/sys/_types/_null.h /usr/include/sys/stdio.h
../obj/ABCD.o: /usr/include/sys/_types/_off_t.h /usr/include/sys/_types/_ssize_t.h
../obj/ABCD.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
../obj/ABCD.o: abcd_types_int.h abcd_manager.h abcd_manager_int.h abcd_node.h
../obj/ABCD.o: abcd_node_int.h abcd_util.h /usr/include/stdlib.h
../obj/ABCD.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/ABCD.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/ABCD.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/ABCD.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/ABCD.o: /usr/include/i386/_mcontext.h /usr/include/mach/i386/_structs.h
../obj/ABCD.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/ABCD.o: /usr/include/sys/_types/_sigaltstack.h
../obj/ABCD.o: /usr/include/sys/_types/_ucontext.h
../obj/ABCD.o: /usr/include/sys/_types/_sigset_t.h /usr/include/sys/_types/_uid_t.h
../obj/ABCD.o: /usr/include/sys/resource.h /usr/include/stdint.h
../obj/ABCD.o: /usr/include/sys/_types/_int8_t.h /usr/include/sys/_types/_int16_t.h
../obj/ABCD.o: /usr/include/sys/_types/_int32_t.h /usr/include/sys/_types/_int64_t.h
../obj/ABCD.o: /usr/include/_types/_uint8_t.h /usr/include/_types/_uint16_t.h
../obj/ABCD.o: /usr/include/_types/_uint32_t.h /usr/include/_types/_uint64_t.h
../obj/ABCD.o: /usr/include/sys/_types/_intptr_t.h
../obj/ABCD.o: /usr/include/sys/_types/_uintptr_t.h /usr/include/_types/_intmax_t.h
../obj/ABCD.o: /usr/include/_types/_uintmax_t.h /usr/include/sys/_types/_timeval.h
../obj/ABCD.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/ABCD.o: /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h
../obj/ABCD.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
../obj/ABCD.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/ABCD.o: /usr/include/sys/_types/_rune_t.h /usr/include/sys/_types/_wchar_t.h
../obj/ABCD.o: /usr/include/machine/types.h /usr/include/i386/types.h
../obj/ABCD.o: /usr/include/sys/_types/_dev_t.h /usr/include/sys/_types/_mode_t.h
../obj/ABCD.o: /usr/include/string.h /usr/include/sys/_types/_rsize_t.h
../obj/ABCD.o: /usr/include/sys/_types/_errno_t.h /usr/include/strings.h
../obj/ABCD.o: /usr/include/secure/_string.h
../obj/abcd.o: config.h abcd.h abcd_types.h /usr/include/stdio.h
../obj/abcd.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
../obj/abcd.o: /usr/include/sys/_posix_availability.h /usr/include/Availability.h
../obj/abcd.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
../obj/abcd.o: /usr/include/i386/_types.h /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd.o: /usr/include/sys/_types/_va_list.h /usr/include/sys/_types/_size_t.h
../obj/abcd.o: /usr/include/sys/_types/_null.h /usr/include/sys/stdio.h
../obj/abcd.o: /usr/include/sys/_types/_off_t.h /usr/include/sys/_types/_ssize_t.h
../obj/abcd.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
../obj/abcd.o: abcd_types_int.h abcd_manager.h abcd_manager_int.h abcd_node.h
../obj/abcd.o: abcd_node_int.h abcd_util.h /usr/include/stdlib.h
../obj/abcd.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd.o: /usr/include/i386/_mcontext.h /usr/include/mach/i386/_structs.h
../obj/abcd.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd.o: /usr/include/sys/_types/_sigset_t.h /usr/include/sys/_types/_uid_t.h
../obj/abcd.o: /usr/include/sys/resource.h /usr/include/stdint.h
../obj/abcd.o: /usr/include/sys/_types/_int8_t.h /usr/include/sys/_types/_int16_t.h
../obj/abcd.o: /usr/include/sys/_types/_int32_t.h /usr/include/sys/_types/_int64_t.h
../obj/abcd.o: /usr/include/_types/_uint8_t.h /usr/include/_types/_uint16_t.h
../obj/abcd.o: /usr/include/_types/_uint32_t.h /usr/include/_types/_uint64_t.h
../obj/abcd.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd.o: /usr/include/sys/_types/_uintptr_t.h /usr/include/_types/_intmax_t.h
../obj/abcd.o: /usr/include/_types/_uintmax_t.h /usr/include/sys/_types/_timeval.h
../obj/abcd.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd.o: /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h
../obj/abcd.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
../obj/abcd.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd.o: /usr/include/sys/_types/_rune_t.h /usr/include/sys/_types/_wchar_t.h
../obj/abcd.o: /usr/include/machine/types.h /usr/include/i386/types.h
../obj/abcd.o: /usr/include/sys/_types/_dev_t.h /usr/include/sys/_types/_mode_t.h
../obj/abcd.o: /usr/include/string.h /usr/include/sys/_types/_rsize_t.h
../obj/abcd.o: /usr/include/sys/_types/_errno_t.h /usr/include/strings.h
../obj/abcd.o: /usr/include/secure/_string.h
../obj/abcd_and.o: abcd_node.h config.h abcd_manager.h abcd_manager_int.h
../obj/abcd_and.o: abcd_types_int.h abcd_types.h abcd_node_int.h abcd_util.h
../obj/abcd_and.o: /usr/include/stdlib.h /usr/include/Availability.h
../obj/abcd_and.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_and.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../obj/abcd_and.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_and.o: /usr/include/sys/_posix_availability.h
../obj/abcd_and.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_and.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_and.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_and.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_and.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_and.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_and.o: /usr/include/i386/_mcontext.h /usr/include/mach/i386/_structs.h
../obj/abcd_and.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_and.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_and.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_and.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_and.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_and.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_and.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_and.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_and.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_and.o: /usr/include/sys/_types/_int64_t.h /usr/include/_types/_uint8_t.h
../obj/abcd_and.o: /usr/include/_types/_uint16_t.h /usr/include/_types/_uint32_t.h
../obj/abcd_and.o: /usr/include/_types/_uint64_t.h
../obj/abcd_and.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_and.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_and.o: /usr/include/_types/_intmax_t.h /usr/include/_types/_uintmax_t.h
../obj/abcd_and.o: /usr/include/sys/_types/_timeval.h /usr/include/machine/endian.h
../obj/abcd_and.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
../obj/abcd_and.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_and.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
../obj/abcd_and.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_and.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_and.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_and.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
../obj/abcd_and.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_and.o: /usr/include/sys/_types/_mode_t.h abcd_cache.h abcd_cache_int.h
../obj/abcd_cache.o: abcd_cache.h abcd_cache_int.h abcd_types_int.h abcd_types.h
../obj/abcd_cache.o: abcd_manager.h abcd_manager_int.h config.h abcd_node.h
../obj/abcd_cache.o: abcd_node_int.h abcd_util.h /usr/include/stdlib.h
../obj/abcd_cache.o: /usr/include/Availability.h /usr/include/AvailabilityInternal.h
../obj/abcd_cache.o: /usr/include/_types.h /usr/include/sys/_types.h
../obj/abcd_cache.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
../obj/abcd_cache.o: /usr/include/sys/_posix_availability.h
../obj/abcd_cache.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_cache.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_cache.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_cache.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_cache.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_cache.o: /usr/include/i386/_mcontext.h /usr/include/mach/i386/_structs.h
../obj/abcd_cache.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_cache.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_cache.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_cache.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_cache.o: /usr/include/_types/_uint8_t.h /usr/include/_types/_uint16_t.h
../obj/abcd_cache.o: /usr/include/_types/_uint32_t.h /usr/include/_types/_uint64_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_cache.o: /usr/include/_types/_intmax_t.h
../obj/abcd_cache.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_cache.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_cache.o: /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h
../obj/abcd_cache.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
../obj/abcd_cache.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
../obj/abcd_cache.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_cache.o: /usr/include/sys/_types/_mode_t.h
../obj/abcd_cofactor.o: abcd_node.h config.h abcd_manager.h abcd_manager_int.h
../obj/abcd_cofactor.o: abcd_types_int.h abcd_types.h abcd_node_int.h abcd_util.h
../obj/abcd_cofactor.o: /usr/include/stdlib.h /usr/include/Availability.h
../obj/abcd_cofactor.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_cofactor.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../obj/abcd_cofactor.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_cofactor.o: /usr/include/sys/_posix_availability.h
../obj/abcd_cofactor.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_cofactor.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_cofactor.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_cofactor.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_cofactor.o: /usr/include/sys/appleapiopts.h
../obj/abcd_cofactor.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
../obj/abcd_cofactor.o: /usr/include/machine/_mcontext.h
../obj/abcd_cofactor.o: /usr/include/i386/_mcontext.h
../obj/abcd_cofactor.o: /usr/include/mach/i386/_structs.h
../obj/abcd_cofactor.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_cofactor.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_cofactor.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_cofactor.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_cofactor.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_cofactor.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_cofactor.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_cofactor.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_cofactor.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_cofactor.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_cofactor.o: /usr/include/_types/_uint8_t.h
../obj/abcd_cofactor.o: /usr/include/_types/_uint16_t.h
../obj/abcd_cofactor.o: /usr/include/_types/_uint32_t.h
../obj/abcd_cofactor.o: /usr/include/_types/_uint64_t.h
../obj/abcd_cofactor.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_cofactor.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_cofactor.o: /usr/include/_types/_intmax_t.h
../obj/abcd_cofactor.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_cofactor.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_cofactor.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_cofactor.o: /usr/include/sys/_endian.h
../obj/abcd_cofactor.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_cofactor.o: /usr/include/libkern/i386/_OSByteOrder.h
../obj/abcd_cofactor.o: /usr/include/alloca.h /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_cofactor.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_cofactor.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_cofactor.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
../obj/abcd_cofactor.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_cofactor.o: /usr/include/sys/_types/_mode_t.h abcd_cache.h
../obj/abcd_cofactor.o: abcd_cache_int.h
../obj/abcd_compose.o: abcd.h abcd_types.h /usr/include/stdio.h
../obj/abcd_compose.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
../obj/abcd_compose.o: /usr/include/sys/_posix_availability.h
../obj/abcd_compose.o: /usr/include/Availability.h
../obj/abcd_compose.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_compose.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
../obj/abcd_compose.o: /usr/include/i386/_types.h
../obj/abcd_compose.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_compose.o: /usr/include/sys/_types/_va_list.h
../obj/abcd_compose.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_compose.o: /usr/include/sys/_types/_null.h /usr/include/sys/stdio.h
../obj/abcd_compose.o: /usr/include/sys/_types/_off_t.h
../obj/abcd_compose.o: /usr/include/sys/_types/_ssize_t.h
../obj/abcd_compose.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
../obj/abcd_compose.o: abcd_node.h config.h abcd_manager.h abcd_manager_int.h
../obj/abcd_compose.o: abcd_types_int.h abcd_node_int.h abcd_util.h
../obj/abcd_compose.o: /usr/include/stdlib.h /usr/include/sys/wait.h
../obj/abcd_compose.o: /usr/include/sys/_types/_pid_t.h
../obj/abcd_compose.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_compose.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_compose.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_compose.o: /usr/include/i386/_mcontext.h
../obj/abcd_compose.o: /usr/include/mach/i386/_structs.h
../obj/abcd_compose.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_compose.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_compose.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_compose.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_compose.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_compose.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_compose.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_compose.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_compose.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_compose.o: /usr/include/_types/_uint8_t.h
../obj/abcd_compose.o: /usr/include/_types/_uint16_t.h
../obj/abcd_compose.o: /usr/include/_types/_uint32_t.h
../obj/abcd_compose.o: /usr/include/_types/_uint64_t.h
../obj/abcd_compose.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_compose.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_compose.o: /usr/include/_types/_intmax_t.h
../obj/abcd_compose.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_compose.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_compose.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_compose.o: /usr/include/sys/_endian.h
../obj/abcd_compose.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_compose.o: /usr/include/libkern/i386/_OSByteOrder.h
../obj/abcd_compose.o: /usr/include/alloca.h /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_compose.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_compose.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_compose.o: /usr/include/machine/types.h /usr/include/i386/types.h
../obj/abcd_compose.o: /usr/include/sys/_types/_dev_t.h
../obj/abcd_compose.o: /usr/include/sys/_types/_mode_t.h abcd_cache.h
../obj/abcd_compose.o: abcd_cache_int.h
../obj/abcd_dump.o: /usr/include/stdlib.h /usr/include/Availability.h
../obj/abcd_dump.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_dump.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../obj/abcd_dump.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_dump.o: /usr/include/sys/_posix_availability.h
../obj/abcd_dump.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_dump.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_dump.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_dump.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_dump.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_dump.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_dump.o: /usr/include/i386/_mcontext.h /usr/include/mach/i386/_structs.h
../obj/abcd_dump.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_dump.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_dump.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_dump.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_dump.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_dump.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_dump.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_dump.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_dump.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_dump.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_dump.o: /usr/include/_types/_uint8_t.h /usr/include/_types/_uint16_t.h
../obj/abcd_dump.o: /usr/include/_types/_uint32_t.h /usr/include/_types/_uint64_t.h
../obj/abcd_dump.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_dump.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_dump.o: /usr/include/_types/_intmax_t.h /usr/include/_types/_uintmax_t.h
../obj/abcd_dump.o: /usr/include/sys/_types/_timeval.h /usr/include/machine/endian.h
../obj/abcd_dump.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
../obj/abcd_dump.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_dump.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
../obj/abcd_dump.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_dump.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_dump.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_dump.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
../obj/abcd_dump.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_dump.o: /usr/include/sys/_types/_mode_t.h /usr/include/stdio.h
../obj/abcd_dump.o: /usr/include/sys/_types/_va_list.h /usr/include/sys/stdio.h
../obj/abcd_dump.o: /usr/include/sys/_types/_off_t.h
../obj/abcd_dump.o: /usr/include/sys/_types/_ssize_t.h /usr/include/secure/_stdio.h
../obj/abcd_dump.o: /usr/include/secure/_common.h abcd_node.h config.h
../obj/abcd_dump.o: abcd_manager.h abcd_manager_int.h abcd_types_int.h abcd_types.h
../obj/abcd_dump.o: abcd_node_int.h abcd_util.h
../obj/abcd_equiv.o: abcd_node.h config.h abcd_manager.h abcd_manager_int.h
../obj/abcd_equiv.o: abcd_types_int.h abcd_types.h abcd_node_int.h abcd_util.h
../obj/abcd_equiv.o: /usr/include/stdlib.h /usr/include/Availability.h
../obj/abcd_equiv.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_equiv.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../obj/abcd_equiv.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_equiv.o: /usr/include/sys/_posix_availability.h
../obj/abcd_equiv.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_equiv.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_equiv.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_equiv.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_equiv.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_equiv.o: /usr/include/i386/_mcontext.h /usr/include/mach/i386/_structs.h
../obj/abcd_equiv.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_equiv.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_equiv.o: /usr/include/_types/_uint8_t.h /usr/include/_types/_uint16_t.h
../obj/abcd_equiv.o: /usr/include/_types/_uint32_t.h /usr/include/_types/_uint64_t.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_equiv.o: /usr/include/_types/_intmax_t.h
../obj/abcd_equiv.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_equiv.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_equiv.o: /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h
../obj/abcd_equiv.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
../obj/abcd_equiv.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_equiv.o: /usr/include/sys/_types/_mode_t.h abcd_cache.h abcd_cache_int.h
../obj/abcd_exists.o: config.h abcd.h abcd_types.h /usr/include/stdio.h
../obj/abcd_exists.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
../obj/abcd_exists.o: /usr/include/sys/_posix_availability.h
../obj/abcd_exists.o: /usr/include/Availability.h
../obj/abcd_exists.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_exists.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
../obj/abcd_exists.o: /usr/include/i386/_types.h
../obj/abcd_exists.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_exists.o: /usr/include/sys/_types/_va_list.h
../obj/abcd_exists.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_exists.o: /usr/include/sys/_types/_null.h /usr/include/sys/stdio.h
../obj/abcd_exists.o: /usr/include/sys/_types/_off_t.h
../obj/abcd_exists.o: /usr/include/sys/_types/_ssize_t.h
../obj/abcd_exists.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
../obj/abcd_exists.o: abcd_node.h abcd_manager.h abcd_manager_int.h abcd_types_int.h
../obj/abcd_exists.o: abcd_node_int.h abcd_util.h /usr/include/stdlib.h
../obj/abcd_exists.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_exists.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_exists.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_exists.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_exists.o: /usr/include/i386/_mcontext.h
../obj/abcd_exists.o: /usr/include/mach/i386/_structs.h
../obj/abcd_exists.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_exists.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_exists.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_exists.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_exists.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_exists.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_exists.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_exists.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_exists.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_exists.o: /usr/include/_types/_uint8_t.h /usr/include/_types/_uint16_t.h
../obj/abcd_exists.o: /usr/include/_types/_uint32_t.h
../obj/abcd_exists.o: /usr/include/_types/_uint64_t.h
../obj/abcd_exists.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_exists.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_exists.o: /usr/include/_types/_intmax_t.h
../obj/abcd_exists.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_exists.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_exists.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_exists.o: /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h
../obj/abcd_exists.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
../obj/abcd_exists.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_exists.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_exists.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_exists.o: /usr/include/machine/types.h /usr/include/i386/types.h
../obj/abcd_exists.o: /usr/include/sys/_types/_dev_t.h
../obj/abcd_exists.o: /usr/include/sys/_types/_mode_t.h abcd_cache.h
../obj/abcd_exists.o: abcd_cache_int.h
../obj/abcd_fraction.o: abcd_node.h config.h abcd_manager.h abcd_manager_int.h
../obj/abcd_fraction.o: abcd_types_int.h abcd_types.h abcd_node_int.h abcd_util.h
../obj/abcd_fraction.o: /usr/include/stdlib.h /usr/include/Availability.h
../obj/abcd_fraction.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_fraction.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../obj/abcd_fraction.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_fraction.o: /usr/include/sys/_posix_availability.h
../obj/abcd_fraction.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_fraction.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_fraction.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_fraction.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_fraction.o: /usr/include/sys/appleapiopts.h
../obj/abcd_fraction.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
../obj/abcd_fraction.o: /usr/include/machine/_mcontext.h
../obj/abcd_fraction.o: /usr/include/i386/_mcontext.h
../obj/abcd_fraction.o: /usr/include/mach/i386/_structs.h
../obj/abcd_fraction.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_fraction.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_fraction.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_fraction.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_fraction.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_fraction.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_fraction.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_fraction.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_fraction.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_fraction.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_fraction.o: /usr/include/_types/_uint8_t.h
../obj/abcd_fraction.o: /usr/include/_types/_uint16_t.h
../obj/abcd_fraction.o: /usr/include/_types/_uint32_t.h
../obj/abcd_fraction.o: /usr/include/_types/_uint64_t.h
../obj/abcd_fraction.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_fraction.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_fraction.o: /usr/include/_types/_intmax_t.h
../obj/abcd_fraction.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_fraction.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_fraction.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_fraction.o: /usr/include/sys/_endian.h
../obj/abcd_fraction.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_fraction.o: /usr/include/libkern/i386/_OSByteOrder.h
../obj/abcd_fraction.o: /usr/include/alloca.h /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_fraction.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_fraction.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_fraction.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
../obj/abcd_fraction.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_fraction.o: /usr/include/sys/_types/_mode_t.h abcd_cache.h
../obj/abcd_fraction.o: abcd_cache_int.h
../obj/abcd_intersects.o: abcd.h abcd_types.h /usr/include/stdio.h
../obj/abcd_intersects.o: /usr/include/sys/cdefs.h
../obj/abcd_intersects.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_intersects.o: /usr/include/sys/_posix_availability.h
../obj/abcd_intersects.o: /usr/include/Availability.h
../obj/abcd_intersects.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_intersects.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
../obj/abcd_intersects.o: /usr/include/i386/_types.h
../obj/abcd_intersects.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_va_list.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_null.h /usr/include/sys/stdio.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_off_t.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_ssize_t.h
../obj/abcd_intersects.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
../obj/abcd_intersects.o: abcd_cache.h abcd_cache_int.h abcd_types_int.h
../obj/abcd_intersects.o: abcd_manager.h abcd_manager_int.h config.h abcd_node.h
../obj/abcd_intersects.o: abcd_node_int.h abcd_util.h /usr/include/stdlib.h
../obj/abcd_intersects.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_intersects.o: /usr/include/sys/appleapiopts.h
../obj/abcd_intersects.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
../obj/abcd_intersects.o: /usr/include/machine/_mcontext.h
../obj/abcd_intersects.o: /usr/include/i386/_mcontext.h
../obj/abcd_intersects.o: /usr/include/mach/i386/_structs.h
../obj/abcd_intersects.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_uid_t.h
../obj/abcd_intersects.o: /usr/include/sys/resource.h /usr/include/stdint.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_int8_t.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_intersects.o: /usr/include/_types/_uint8_t.h
../obj/abcd_intersects.o: /usr/include/_types/_uint16_t.h
../obj/abcd_intersects.o: /usr/include/_types/_uint32_t.h
../obj/abcd_intersects.o: /usr/include/_types/_uint64_t.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_intersects.o: /usr/include/_types/_intmax_t.h
../obj/abcd_intersects.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_intersects.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_intersects.o: /usr/include/sys/_endian.h
../obj/abcd_intersects.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_intersects.o: /usr/include/libkern/i386/_OSByteOrder.h
../obj/abcd_intersects.o: /usr/include/alloca.h /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_intersects.o: /usr/include/machine/types.h /usr/include/i386/types.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_dev_t.h
../obj/abcd_intersects.o: /usr/include/sys/_types/_mode_t.h
../obj/abcd_ite.o: abcd.h abcd_types.h /usr/include/stdio.h /usr/include/sys/cdefs.h
../obj/abcd_ite.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_ite.o: /usr/include/sys/_posix_availability.h
../obj/abcd_ite.o: /usr/include/Availability.h /usr/include/AvailabilityInternal.h
../obj/abcd_ite.o: /usr/include/_types.h /usr/include/sys/_types.h
../obj/abcd_ite.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_ite.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_ite.o: /usr/include/sys/_types/_va_list.h
../obj/abcd_ite.o: /usr/include/sys/_types/_size_t.h /usr/include/sys/_types/_null.h
../obj/abcd_ite.o: /usr/include/sys/stdio.h /usr/include/sys/_types/_off_t.h
../obj/abcd_ite.o: /usr/include/sys/_types/_ssize_t.h /usr/include/secure/_stdio.h
../obj/abcd_ite.o: /usr/include/secure/_common.h abcd_node.h config.h abcd_manager.h
../obj/abcd_ite.o: abcd_manager_int.h abcd_types_int.h abcd_node_int.h abcd_util.h
../obj/abcd_ite.o: /usr/include/stdlib.h /usr/include/sys/wait.h
../obj/abcd_ite.o: /usr/include/sys/_types/_pid_t.h /usr/include/sys/_types/_id_t.h
../obj/abcd_ite.o: /usr/include/sys/signal.h /usr/include/sys/appleapiopts.h
../obj/abcd_ite.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
../obj/abcd_ite.o: /usr/include/machine/_mcontext.h /usr/include/i386/_mcontext.h
../obj/abcd_ite.o: /usr/include/mach/i386/_structs.h
../obj/abcd_ite.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_ite.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_ite.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_ite.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_ite.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_ite.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_ite.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_ite.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_ite.o: /usr/include/sys/_types/_int64_t.h /usr/include/_types/_uint8_t.h
../obj/abcd_ite.o: /usr/include/_types/_uint16_t.h /usr/include/_types/_uint32_t.h
../obj/abcd_ite.o: /usr/include/_types/_uint64_t.h
../obj/abcd_ite.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_ite.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_ite.o: /usr/include/_types/_intmax_t.h /usr/include/_types/_uintmax_t.h
../obj/abcd_ite.o: /usr/include/sys/_types/_timeval.h /usr/include/machine/endian.h
../obj/abcd_ite.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
../obj/abcd_ite.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_ite.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
../obj/abcd_ite.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_ite.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_ite.o: /usr/include/sys/_types/_wchar_t.h /usr/include/machine/types.h
../obj/abcd_ite.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_ite.o: /usr/include/sys/_types/_mode_t.h abcd_cache.h abcd_cache_int.h
../obj/abcd_manager.o: abcd_manager_int.h config.h abcd_node.h abcd_manager.h
../obj/abcd_manager.o: abcd_node_int.h abcd_types_int.h abcd_types.h abcd_util.h
../obj/abcd_manager.o: /usr/include/stdlib.h /usr/include/Availability.h
../obj/abcd_manager.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_manager.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../obj/abcd_manager.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_manager.o: /usr/include/sys/_posix_availability.h
../obj/abcd_manager.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_manager.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_manager.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_manager.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_manager.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_manager.o: /usr/include/i386/_mcontext.h
../obj/abcd_manager.o: /usr/include/mach/i386/_structs.h
../obj/abcd_manager.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_manager.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_manager.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_manager.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_manager.o: /usr/include/_types/_uint8_t.h
../obj/abcd_manager.o: /usr/include/_types/_uint16_t.h
../obj/abcd_manager.o: /usr/include/_types/_uint32_t.h
../obj/abcd_manager.o: /usr/include/_types/_uint64_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_manager.o: /usr/include/_types/_intmax_t.h
../obj/abcd_manager.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_manager.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_manager.o: /usr/include/sys/_endian.h
../obj/abcd_manager.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_manager.o: /usr/include/libkern/i386/_OSByteOrder.h
../obj/abcd_manager.o: /usr/include/alloca.h /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
../obj/abcd_manager.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_manager.o: /usr/include/sys/_types/_mode_t.h
../obj/abcd_manager_int.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
../obj/abcd_manager_int.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_manager_int.o: /usr/include/sys/_posix_availability.h
../obj/abcd_manager_int.o: /usr/include/Availability.h
../obj/abcd_manager_int.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_manager_int.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
../obj/abcd_manager_int.o: /usr/include/i386/_types.h
../obj/abcd_manager_int.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_va_list.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_null.h /usr/include/sys/stdio.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_off_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_ssize_t.h
../obj/abcd_manager_int.o: /usr/include/secure/_stdio.h
../obj/abcd_manager_int.o: /usr/include/secure/_common.h /usr/include/string.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_rsize_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_errno_t.h /usr/include/strings.h
../obj/abcd_manager_int.o: /usr/include/secure/_string.h config.h abcd_cache.h
../obj/abcd_manager_int.o: abcd_cache_int.h abcd_types_int.h abcd_types.h
../obj/abcd_manager_int.o: abcd_manager.h abcd_manager_int.h abcd_node.h
../obj/abcd_manager_int.o: abcd_node_int.h abcd_util.h /usr/include/stdlib.h
../obj/abcd_manager_int.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_manager_int.o: /usr/include/sys/appleapiopts.h
../obj/abcd_manager_int.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
../obj/abcd_manager_int.o: /usr/include/machine/_mcontext.h
../obj/abcd_manager_int.o: /usr/include/i386/_mcontext.h
../obj/abcd_manager_int.o: /usr/include/mach/i386/_structs.h
../obj/abcd_manager_int.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_uid_t.h
../obj/abcd_manager_int.o: /usr/include/sys/resource.h /usr/include/stdint.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_int8_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_manager_int.o: /usr/include/_types/_uint8_t.h
../obj/abcd_manager_int.o: /usr/include/_types/_uint16_t.h
../obj/abcd_manager_int.o: /usr/include/_types/_uint32_t.h
../obj/abcd_manager_int.o: /usr/include/_types/_uint64_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_manager_int.o: /usr/include/_types/_intmax_t.h
../obj/abcd_manager_int.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_manager_int.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_manager_int.o: /usr/include/sys/_endian.h
../obj/abcd_manager_int.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_manager_int.o: /usr/include/libkern/i386/_OSByteOrder.h
../obj/abcd_manager_int.o: /usr/include/alloca.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_manager_int.o: /usr/include/machine/types.h /usr/include/i386/types.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_dev_t.h
../obj/abcd_manager_int.o: /usr/include/sys/_types/_mode_t.h
../obj/abcd_node.o: config.h abcd_manager.h abcd_manager_int.h abcd_node.h
../obj/abcd_node.o: abcd_node_int.h abcd_types_int.h abcd_types.h abcd_util.h
../obj/abcd_node.o: /usr/include/stdlib.h /usr/include/Availability.h
../obj/abcd_node.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_node.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../obj/abcd_node.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_node.o: /usr/include/sys/_posix_availability.h
../obj/abcd_node.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_node.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_node.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_node.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_node.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_node.o: /usr/include/i386/_mcontext.h /usr/include/mach/i386/_structs.h
../obj/abcd_node.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_node.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_node.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_node.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_node.o: /usr/include/_types/_uint8_t.h /usr/include/_types/_uint16_t.h
../obj/abcd_node.o: /usr/include/_types/_uint32_t.h /usr/include/_types/_uint64_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_node.o: /usr/include/_types/_intmax_t.h /usr/include/_types/_uintmax_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_timeval.h /usr/include/machine/endian.h
../obj/abcd_node.o: /usr/include/i386/endian.h /usr/include/sys/_endian.h
../obj/abcd_node.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_node.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
../obj/abcd_node.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
../obj/abcd_node.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_node.o: /usr/include/sys/_types/_mode_t.h
../obj/abcd_onecube.o: abcd_node.h config.h abcd_manager.h abcd_manager_int.h
../obj/abcd_onecube.o: abcd_types_int.h abcd_types.h abcd_node_int.h abcd_util.h
../obj/abcd_onecube.o: /usr/include/stdlib.h /usr/include/Availability.h
../obj/abcd_onecube.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_onecube.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../obj/abcd_onecube.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_onecube.o: /usr/include/sys/_posix_availability.h
../obj/abcd_onecube.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_onecube.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_onecube.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_onecube.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_onecube.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_onecube.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_onecube.o: /usr/include/i386/_mcontext.h
../obj/abcd_onecube.o: /usr/include/mach/i386/_structs.h
../obj/abcd_onecube.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_onecube.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_onecube.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_onecube.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_onecube.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_onecube.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_onecube.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_onecube.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_onecube.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_onecube.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_onecube.o: /usr/include/_types/_uint8_t.h
../obj/abcd_onecube.o: /usr/include/_types/_uint16_t.h
../obj/abcd_onecube.o: /usr/include/_types/_uint32_t.h
../obj/abcd_onecube.o: /usr/include/_types/_uint64_t.h
../obj/abcd_onecube.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_onecube.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_onecube.o: /usr/include/_types/_intmax_t.h
../obj/abcd_onecube.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_onecube.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_onecube.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_onecube.o: /usr/include/sys/_endian.h
../obj/abcd_onecube.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_onecube.o: /usr/include/libkern/i386/_OSByteOrder.h
../obj/abcd_onecube.o: /usr/include/alloca.h /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_onecube.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_onecube.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_onecube.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
../obj/abcd_onecube.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_onecube.o: /usr/include/sys/_types/_mode_t.h
../obj/abcd_print.o: abcd_node.h config.h abcd_manager.h abcd_manager_int.h
../obj/abcd_print.o: abcd_types_int.h abcd_types.h abcd_node_int.h abcd_util.h
../obj/abcd_print.o: /usr/include/stdlib.h /usr/include/Availability.h
../obj/abcd_print.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_print.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../obj/abcd_print.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_print.o: /usr/include/sys/_posix_availability.h
../obj/abcd_print.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_print.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_print.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_print.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_print.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_print.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_print.o: /usr/include/i386/_mcontext.h /usr/include/mach/i386/_structs.h
../obj/abcd_print.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_print.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_print.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_print.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_print.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_print.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_print.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_print.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_print.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_print.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_print.o: /usr/include/_types/_uint8_t.h /usr/include/_types/_uint16_t.h
../obj/abcd_print.o: /usr/include/_types/_uint32_t.h /usr/include/_types/_uint64_t.h
../obj/abcd_print.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_print.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_print.o: /usr/include/_types/_intmax_t.h
../obj/abcd_print.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_print.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_print.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_print.o: /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h
../obj/abcd_print.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
../obj/abcd_print.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_print.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_print.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_print.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
../obj/abcd_print.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_print.o: /usr/include/sys/_types/_mode_t.h abcd_cache.h abcd_cache_int.h
../obj/abcd_print.o: /usr/include/stdio.h /usr/include/sys/_types/_va_list.h
../obj/abcd_print.o: /usr/include/sys/stdio.h /usr/include/sys/_types/_off_t.h
../obj/abcd_print.o: /usr/include/sys/_types/_ssize_t.h /usr/include/secure/_stdio.h
../obj/abcd_print.o: /usr/include/secure/_common.h
../obj/abcd_reduce.o: abcd.h abcd_types.h /usr/include/stdio.h
../obj/abcd_reduce.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
../obj/abcd_reduce.o: /usr/include/sys/_posix_availability.h
../obj/abcd_reduce.o: /usr/include/Availability.h
../obj/abcd_reduce.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_reduce.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
../obj/abcd_reduce.o: /usr/include/i386/_types.h
../obj/abcd_reduce.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_va_list.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_null.h /usr/include/sys/stdio.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_off_t.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_ssize_t.h
../obj/abcd_reduce.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
../obj/abcd_reduce.o: abcd_cache.h abcd_cache_int.h abcd_types_int.h abcd_manager.h
../obj/abcd_reduce.o: abcd_manager_int.h config.h abcd_node.h abcd_node_int.h
../obj/abcd_reduce.o: abcd_util.h /usr/include/stdlib.h /usr/include/sys/wait.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_pid_t.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_reduce.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_reduce.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_reduce.o: /usr/include/i386/_mcontext.h
../obj/abcd_reduce.o: /usr/include/mach/i386/_structs.h
../obj/abcd_reduce.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_reduce.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_reduce.o: /usr/include/_types/_uint8_t.h /usr/include/_types/_uint16_t.h
../obj/abcd_reduce.o: /usr/include/_types/_uint32_t.h
../obj/abcd_reduce.o: /usr/include/_types/_uint64_t.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_reduce.o: /usr/include/_types/_intmax_t.h
../obj/abcd_reduce.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_reduce.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_reduce.o: /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h
../obj/abcd_reduce.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_reduce.o: /usr/include/machine/types.h /usr/include/i386/types.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_dev_t.h
../obj/abcd_reduce.o: /usr/include/sys/_types/_mode_t.h
../obj/abcd_relprod.o: abcd.h abcd_types.h /usr/include/stdio.h
../obj/abcd_relprod.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
../obj/abcd_relprod.o: /usr/include/sys/_posix_availability.h
../obj/abcd_relprod.o: /usr/include/Availability.h
../obj/abcd_relprod.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_relprod.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
../obj/abcd_relprod.o: /usr/include/i386/_types.h
../obj/abcd_relprod.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_va_list.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_null.h /usr/include/sys/stdio.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_off_t.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_ssize_t.h
../obj/abcd_relprod.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
../obj/abcd_relprod.o: abcd_node.h config.h abcd_manager.h abcd_manager_int.h
../obj/abcd_relprod.o: abcd_types_int.h abcd_node_int.h abcd_util.h
../obj/abcd_relprod.o: /usr/include/stdlib.h /usr/include/sys/wait.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_pid_t.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_relprod.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_relprod.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_relprod.o: /usr/include/i386/_mcontext.h
../obj/abcd_relprod.o: /usr/include/mach/i386/_structs.h
../obj/abcd_relprod.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_relprod.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_relprod.o: /usr/include/_types/_uint8_t.h
../obj/abcd_relprod.o: /usr/include/_types/_uint16_t.h
../obj/abcd_relprod.o: /usr/include/_types/_uint32_t.h
../obj/abcd_relprod.o: /usr/include/_types/_uint64_t.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_relprod.o: /usr/include/_types/_intmax_t.h
../obj/abcd_relprod.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_relprod.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_relprod.o: /usr/include/sys/_endian.h
../obj/abcd_relprod.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_relprod.o: /usr/include/libkern/i386/_OSByteOrder.h
../obj/abcd_relprod.o: /usr/include/alloca.h /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_relprod.o: /usr/include/machine/types.h /usr/include/i386/types.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_dev_t.h
../obj/abcd_relprod.o: /usr/include/sys/_types/_mode_t.h abcd_cache.h
../obj/abcd_relprod.o: abcd_cache_int.h
../obj/abcd_stats.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
../obj/abcd_stats.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_stats.o: /usr/include/sys/_posix_availability.h
../obj/abcd_stats.o: /usr/include/Availability.h /usr/include/AvailabilityInternal.h
../obj/abcd_stats.o: /usr/include/_types.h /usr/include/sys/_types.h
../obj/abcd_stats.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_stats.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_stats.o: /usr/include/sys/_types/_va_list.h
../obj/abcd_stats.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_stats.o: /usr/include/sys/_types/_null.h /usr/include/sys/stdio.h
../obj/abcd_stats.o: /usr/include/sys/_types/_off_t.h
../obj/abcd_stats.o: /usr/include/sys/_types/_ssize_t.h /usr/include/secure/_stdio.h
../obj/abcd_stats.o: /usr/include/secure/_common.h /usr/include/string.h
../obj/abcd_stats.o: /usr/include/sys/_types/_rsize_t.h
../obj/abcd_stats.o: /usr/include/sys/_types/_errno_t.h /usr/include/strings.h
../obj/abcd_stats.o: /usr/include/secure/_string.h config.h abcd_cache.h
../obj/abcd_stats.o: abcd_cache_int.h abcd_types_int.h abcd_types.h abcd_manager.h
../obj/abcd_stats.o: abcd_manager_int.h abcd_node.h abcd_node_int.h abcd_util.h
../obj/abcd_stats.o: /usr/include/stdlib.h /usr/include/sys/wait.h
../obj/abcd_stats.o: /usr/include/sys/_types/_pid_t.h
../obj/abcd_stats.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_stats.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_stats.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_stats.o: /usr/include/i386/_mcontext.h /usr/include/mach/i386/_structs.h
../obj/abcd_stats.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_stats.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_stats.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_stats.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_stats.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_stats.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_stats.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_stats.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_stats.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_stats.o: /usr/include/_types/_uint8_t.h /usr/include/_types/_uint16_t.h
../obj/abcd_stats.o: /usr/include/_types/_uint32_t.h /usr/include/_types/_uint64_t.h
../obj/abcd_stats.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_stats.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_stats.o: /usr/include/_types/_intmax_t.h
../obj/abcd_stats.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_stats.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_stats.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_stats.o: /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h
../obj/abcd_stats.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
../obj/abcd_stats.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_stats.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_stats.o: /usr/include/sys/_types/_wchar_t.h /usr/include/machine/types.h
../obj/abcd_stats.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_stats.o: /usr/include/sys/_types/_mode_t.h
../obj/abcd_support.o: /usr/include/stdlib.h /usr/include/Availability.h
../obj/abcd_support.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_support.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../obj/abcd_support.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_support.o: /usr/include/sys/_posix_availability.h
../obj/abcd_support.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_support.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_support.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_support.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_support.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/abcd_support.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/abcd_support.o: /usr/include/i386/_mcontext.h
../obj/abcd_support.o: /usr/include/mach/i386/_structs.h
../obj/abcd_support.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_support.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_support.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_support.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_support.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_support.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_support.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_support.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_support.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_support.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_support.o: /usr/include/_types/_uint8_t.h
../obj/abcd_support.o: /usr/include/_types/_uint16_t.h
../obj/abcd_support.o: /usr/include/_types/_uint32_t.h
../obj/abcd_support.o: /usr/include/_types/_uint64_t.h
../obj/abcd_support.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_support.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_support.o: /usr/include/_types/_intmax_t.h
../obj/abcd_support.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_support.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_support.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_support.o: /usr/include/sys/_endian.h
../obj/abcd_support.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_support.o: /usr/include/libkern/i386/_OSByteOrder.h
../obj/abcd_support.o: /usr/include/alloca.h /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_support.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_support.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_support.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
../obj/abcd_support.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_support.o: /usr/include/sys/_types/_mode_t.h abcd.h abcd_types.h
../obj/abcd_support.o: /usr/include/stdio.h /usr/include/sys/_types/_va_list.h
../obj/abcd_support.o: /usr/include/sys/stdio.h /usr/include/sys/_types/_off_t.h
../obj/abcd_support.o: /usr/include/sys/_types/_ssize_t.h
../obj/abcd_support.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
../obj/abcd_support.o: abcd_node.h config.h abcd_manager.h abcd_manager_int.h
../obj/abcd_support.o: abcd_types_int.h abcd_node_int.h abcd_util.h
../obj/abcd_unnegate.o: abcd_node.h config.h abcd_manager.h abcd_manager_int.h
../obj/abcd_unnegate.o: abcd_types_int.h abcd_types.h abcd_node_int.h abcd_util.h
../obj/abcd_unnegate.o: /usr/include/stdlib.h /usr/include/Availability.h
../obj/abcd_unnegate.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_unnegate.o: /usr/include/sys/_types.h /usr/include/sys/cdefs.h
../obj/abcd_unnegate.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_unnegate.o: /usr/include/sys/_posix_availability.h
../obj/abcd_unnegate.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
../obj/abcd_unnegate.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_unnegate.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/abcd_unnegate.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_unnegate.o: /usr/include/sys/appleapiopts.h
../obj/abcd_unnegate.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
../obj/abcd_unnegate.o: /usr/include/machine/_mcontext.h
../obj/abcd_unnegate.o: /usr/include/i386/_mcontext.h
../obj/abcd_unnegate.o: /usr/include/mach/i386/_structs.h
../obj/abcd_unnegate.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_unnegate.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_unnegate.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_unnegate.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_unnegate.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_unnegate.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/resource.h
../obj/abcd_unnegate.o: /usr/include/stdint.h /usr/include/sys/_types/_int8_t.h
../obj/abcd_unnegate.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_unnegate.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_unnegate.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_unnegate.o: /usr/include/_types/_uint8_t.h
../obj/abcd_unnegate.o: /usr/include/_types/_uint16_t.h
../obj/abcd_unnegate.o: /usr/include/_types/_uint32_t.h
../obj/abcd_unnegate.o: /usr/include/_types/_uint64_t.h
../obj/abcd_unnegate.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_unnegate.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_unnegate.o: /usr/include/_types/_intmax_t.h
../obj/abcd_unnegate.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_unnegate.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_unnegate.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_unnegate.o: /usr/include/sys/_endian.h
../obj/abcd_unnegate.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_unnegate.o: /usr/include/libkern/i386/_OSByteOrder.h
../obj/abcd_unnegate.o: /usr/include/alloca.h /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_unnegate.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_unnegate.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_unnegate.o: /usr/include/sys/_types/_null.h /usr/include/machine/types.h
../obj/abcd_unnegate.o: /usr/include/i386/types.h /usr/include/sys/_types/_dev_t.h
../obj/abcd_unnegate.o: /usr/include/sys/_types/_mode_t.h abcd_cache.h
../obj/abcd_unnegate.o: abcd_cache_int.h
../obj/abcd_util.o: /usr/include/signal.h /usr/include/sys/cdefs.h
../obj/abcd_util.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_util.o: /usr/include/sys/_posix_availability.h /usr/include/_types.h
../obj/abcd_util.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
../obj/abcd_util.o: /usr/include/i386/_types.h
../obj/abcd_util.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_util.o: /usr/include/sys/signal.h /usr/include/sys/appleapiopts.h
../obj/abcd_util.o: /usr/include/Availability.h /usr/include/AvailabilityInternal.h
../obj/abcd_util.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
../obj/abcd_util.o: /usr/include/machine/_mcontext.h /usr/include/i386/_mcontext.h
../obj/abcd_util.o: /usr/include/mach/i386/_structs.h
../obj/abcd_util.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_util.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_util.o: /usr/include/sys/_types/_pid_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_uid_t.h
../obj/abcd_util.o: /usr/include/sys/_pthread/_pthread_t.h /usr/include/sys/time.h
../obj/abcd_util.o: /usr/include/sys/_types/_fd_def.h
../obj/abcd_util.o: /usr/include/sys/_types/_timespec.h
../obj/abcd_util.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_util.o: /usr/include/sys/_types/_timeval64.h
../obj/abcd_util.o: /usr/include/sys/_types/_time_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_suseconds_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_fd_setsize.h
../obj/abcd_util.o: /usr/include/sys/_types/_fd_set.h
../obj/abcd_util.o: /usr/include/sys/_types/_fd_clr.h
../obj/abcd_util.o: /usr/include/sys/_types/_fd_isset.h
../obj/abcd_util.o: /usr/include/sys/_types/_fd_zero.h
../obj/abcd_util.o: /usr/include/sys/_types/_fd_copy.h /usr/include/time.h
../obj/abcd_util.o: /usr/include/sys/_types/_clock_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_null.h /usr/include/sys/_select.h
../obj/abcd_util.o: /usr/include/sys/resource.h /usr/include/stdint.h
../obj/abcd_util.o: /usr/include/sys/_types/_int8_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_util.o: /usr/include/_types/_uint8_t.h /usr/include/_types/_uint16_t.h
../obj/abcd_util.o: /usr/include/_types/_uint32_t.h /usr/include/_types/_uint64_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_util.o: /usr/include/_types/_intmax_t.h /usr/include/_types/_uintmax_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/types.h
../obj/abcd_util.o: /usr/include/machine/types.h /usr/include/i386/types.h
../obj/abcd_util.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_util.o: /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h
../obj/abcd_util.o: /usr/include/libkern/i386/_OSByteOrder.h
../obj/abcd_util.o: /usr/include/sys/_types/_dev_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_blkcnt_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_blksize_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_gid_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_in_addr_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_in_port_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_ino_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_ino64_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_key_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_mode_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_nlink_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_off_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_ssize_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_useconds_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_rsize_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_errno_t.h
../obj/abcd_util.o: /usr/include/sys/_pthread/_pthread_cond_t.h
../obj/abcd_util.o: /usr/include/sys/_pthread/_pthread_condattr_t.h
../obj/abcd_util.o: /usr/include/sys/_pthread/_pthread_mutex_t.h
../obj/abcd_util.o: /usr/include/sys/_pthread/_pthread_mutexattr_t.h
../obj/abcd_util.o: /usr/include/sys/_pthread/_pthread_once_t.h
../obj/abcd_util.o: /usr/include/sys/_pthread/_pthread_rwlock_t.h
../obj/abcd_util.o: /usr/include/sys/_pthread/_pthread_rwlockattr_t.h
../obj/abcd_util.o: /usr/include/sys/_pthread/_pthread_key_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_fsblkcnt_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_fsfilcnt_t.h /usr/include/stdio.h
../obj/abcd_util.o: /usr/include/sys/_types/_va_list.h /usr/include/sys/stdio.h
../obj/abcd_util.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
../obj/abcd_util.o: /usr/include/stdlib.h /usr/include/sys/wait.h
../obj/abcd_util.o: /usr/include/alloca.h /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_util.o: /usr/include/sys/_types/_wchar_t.h /usr/include/unistd.h
../obj/abcd_util.o: /usr/include/sys/unistd.h
../obj/abcd_util.o: /usr/include/sys/_types/_posix_vdisable.h
../obj/abcd_util.o: /usr/include/sys/_types/_seek_set.h /usr/include/sys/select.h
../obj/abcd_util.o: /usr/include/sys/_types/_uuid_t.h /usr/include/gethostuuid.h
../obj/abcd_util.o: abcd_types.h abcd_manager.h abcd_manager_int.h config.h
../obj/abcd_util.o: abcd_node.h abcd_node_int.h abcd_types_int.h abcd_util.h
../obj/abcd_visualize.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
../obj/abcd_visualize.o: /usr/include/sys/_symbol_aliasing.h
../obj/abcd_visualize.o: /usr/include/sys/_posix_availability.h
../obj/abcd_visualize.o: /usr/include/Availability.h
../obj/abcd_visualize.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/abcd_visualize.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
../obj/abcd_visualize.o: /usr/include/i386/_types.h
../obj/abcd_visualize.o: /usr/include/sys/_pthread/_pthread_types.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_va_list.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_size_t.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_null.h /usr/include/sys/stdio.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_off_t.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_ssize_t.h
../obj/abcd_visualize.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h
../obj/abcd_visualize.o: /usr/include/stdlib.h /usr/include/sys/wait.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_pid_t.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/abcd_visualize.o: /usr/include/sys/appleapiopts.h
../obj/abcd_visualize.o: /usr/include/machine/signal.h /usr/include/i386/signal.h
../obj/abcd_visualize.o: /usr/include/machine/_mcontext.h
../obj/abcd_visualize.o: /usr/include/i386/_mcontext.h
../obj/abcd_visualize.o: /usr/include/mach/i386/_structs.h
../obj/abcd_visualize.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_sigaltstack.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_ucontext.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_sigset_t.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_uid_t.h
../obj/abcd_visualize.o: /usr/include/sys/resource.h /usr/include/stdint.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_int8_t.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_int16_t.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_int32_t.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_int64_t.h
../obj/abcd_visualize.o: /usr/include/_types/_uint8_t.h
../obj/abcd_visualize.o: /usr/include/_types/_uint16_t.h
../obj/abcd_visualize.o: /usr/include/_types/_uint32_t.h
../obj/abcd_visualize.o: /usr/include/_types/_uint64_t.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_intptr_t.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_uintptr_t.h
../obj/abcd_visualize.o: /usr/include/_types/_intmax_t.h
../obj/abcd_visualize.o: /usr/include/_types/_uintmax_t.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_timeval.h
../obj/abcd_visualize.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/abcd_visualize.o: /usr/include/sys/_endian.h
../obj/abcd_visualize.o: /usr/include/libkern/_OSByteOrder.h
../obj/abcd_visualize.o: /usr/include/libkern/i386/_OSByteOrder.h
../obj/abcd_visualize.o: /usr/include/alloca.h /usr/include/sys/_types/_ct_rune_t.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_rune_t.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_wchar_t.h
../obj/abcd_visualize.o: /usr/include/machine/types.h /usr/include/i386/types.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_dev_t.h
../obj/abcd_visualize.o: /usr/include/sys/_types/_mode_t.h abcd_cache.h
../obj/abcd_visualize.o: abcd_cache_int.h abcd_types_int.h abcd_types.h
../obj/abcd_visualize.o: abcd_manager.h abcd_manager_int.h config.h abcd_node.h
../obj/abcd_visualize.o: abcd_node_int.h abcd_util.h
../obj/test.o: /usr/include/stdio.h /usr/include/sys/cdefs.h
../obj/test.o: /usr/include/sys/_symbol_aliasing.h
../obj/test.o: /usr/include/sys/_posix_availability.h /usr/include/Availability.h
../obj/test.o: /usr/include/AvailabilityInternal.h /usr/include/_types.h
../obj/test.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
../obj/test.o: /usr/include/i386/_types.h /usr/include/sys/_pthread/_pthread_types.h
../obj/test.o: /usr/include/sys/_types/_va_list.h /usr/include/sys/_types/_size_t.h
../obj/test.o: /usr/include/sys/_types/_null.h /usr/include/sys/stdio.h
../obj/test.o: /usr/include/sys/_types/_off_t.h /usr/include/sys/_types/_ssize_t.h
../obj/test.o: /usr/include/secure/_stdio.h /usr/include/secure/_common.h ABCD.h
../obj/test.o: abcd_types.h abcd_node_int.h config.h abcd_types_int.h
../obj/test.o: abcd_cache_int.h abcd_util.h /usr/include/stdlib.h
../obj/test.o: /usr/include/sys/wait.h /usr/include/sys/_types/_pid_t.h
../obj/test.o: /usr/include/sys/_types/_id_t.h /usr/include/sys/signal.h
../obj/test.o: /usr/include/sys/appleapiopts.h /usr/include/machine/signal.h
../obj/test.o: /usr/include/i386/signal.h /usr/include/machine/_mcontext.h
../obj/test.o: /usr/include/i386/_mcontext.h /usr/include/mach/i386/_structs.h
../obj/test.o: /usr/include/sys/_pthread/_pthread_attr_t.h
../obj/test.o: /usr/include/sys/_types/_sigaltstack.h
../obj/test.o: /usr/include/sys/_types/_ucontext.h
../obj/test.o: /usr/include/sys/_types/_sigset_t.h /usr/include/sys/_types/_uid_t.h
../obj/test.o: /usr/include/sys/resource.h /usr/include/stdint.h
../obj/test.o: /usr/include/sys/_types/_int8_t.h /usr/include/sys/_types/_int16_t.h
../obj/test.o: /usr/include/sys/_types/_int32_t.h /usr/include/sys/_types/_int64_t.h
../obj/test.o: /usr/include/_types/_uint8_t.h /usr/include/_types/_uint16_t.h
../obj/test.o: /usr/include/_types/_uint32_t.h /usr/include/_types/_uint64_t.h
../obj/test.o: /usr/include/sys/_types/_intptr_t.h
../obj/test.o: /usr/include/sys/_types/_uintptr_t.h /usr/include/_types/_intmax_t.h
../obj/test.o: /usr/include/_types/_uintmax_t.h /usr/include/sys/_types/_timeval.h
../obj/test.o: /usr/include/machine/endian.h /usr/include/i386/endian.h
../obj/test.o: /usr/include/sys/_endian.h /usr/include/libkern/_OSByteOrder.h
../obj/test.o: /usr/include/libkern/i386/_OSByteOrder.h /usr/include/alloca.h
../obj/test.o: /usr/include/sys/_types/_ct_rune_t.h
../obj/test.o: /usr/include/sys/_types/_rune_t.h /usr/include/sys/_types/_wchar_t.h
../obj/test.o: /usr/include/machine/types.h /usr/include/i386/types.h
../obj/test.o: /usr/include/sys/_types/_dev_t.h /usr/include/sys/_types/_mode_t.h
