all:
	@if [ ! -f .configuration ]; \
	then \
	  echo "trying to run:  ./configure" 2>&1; \
	  ./configure || exit 1; \
	fi
	@make depend
	@cd src; make all

clean:
	cd src; make clean
	cd contrib/abcd/src; make clean
	cd contrib/bddlib.D.E.Long; make clean

depend:
	cd src; make depend
	cd contrib/abcd/src; make depend

distclean:
	./bin/distclean
	cd src; make distclean
	cd contrib/bddlib.D.E.Long; make distclean
	cd contrib/abcd/src; make distclean

realclean: distclean

.PHONY: all clean config depend distclean realclean
