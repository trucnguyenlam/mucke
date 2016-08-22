#------------------------------------------------------------------------

FLAGS=-g -fPIC -fno-strict-aliasing -Wno-write-strings
CXXFLAGS=-g -fPIC -fno-strict-aliasing -fpermissive -Wno-write-strings

#------------------------------------------------------------------------

CXX=g++ $(CXXFLAGS) -Wall
CC=gcc $(FLAGS) -Wall
CCNOWARN=gcc $(FLAGS) -W
LD=g++
YACC=yacc

#------------------------------------------------------------------------

all: config.h
	@. ../.configuration; \
	if [ $$STATIC = true ]; \
	then \
	  echo "[STATIC]"; \
	  make static; \
	else \
	  echo "[DYNAMIC]"; \
	  make dynamic; \
	fi

#------------------------------------------------------------------------

BIN=../bin/LINK
OBJ=../obj/LINK
INCL=-I../contrib/bddlib.D.E.Long -I../contrib/abcd/src -I../contrib/cudd/install/include

#------------------------------------------------------------------------

static: $(BIN)/mucke_static_bman

#------------------------------------------------------------------------

dynamic: $(BIN)/simplebman.so $(BIN)/abcdbman.so $(BIN)/mucke_dynamic_bman
	@. ../.configuration; \
	if [ ! x"$$LONGBDD" = x ]; \
	then \
	  echo "[generating longbman.so]"; \
	  make $(BIN)/longbman.so; \
	  cd ../contrib/bddlib.D.E.Long; \
	  make ../../bin/LINK/bddlib.D.E.Long.so; \
	fi
	if [ ! x"$$CUDDBDD" = x ]; \
	then \
	  echo "[generating libcudd.a]"; \
	  make $(BIN)/cuddbman.so;
	fi
	@echo "[generating abcdbman.so]"; \
	cd ../contrib/abcd/src; make ../lib/libabcd.so
	cp ../contrib/abcd/lib/libabcd.so $(BIN)

#------------------------------------------------------------------------

SIMPLEBMANOBJS=$(OBJ)/simplebman.o $(OBJ)/BDDsimple.o

$(BIN)/simplebman.so:  $(SIMPLEBMANOBJS)
	../bin/ldshared $@ $(SIMPLEBMANOBJS)

#------------------------------------------------------------------------

LONGBMANOBJS=$(OBJ)/longbman.o $(OBJ)/longbddstats.o

$(BIN)/longbman.so: $(LONGBMANOBJS) $(OBJ)/longbmanDynamic.o
	../bin/ldshared $@ $(LONGBMANOBJS) $(OBJ)/longbmanDynamic.o

#------------------------------------------------------------------------
#------------------------------------------------------------------------

CUDDBMANOBJS=$(OBJ)/cuddbman.o $(OBJ)/cuddbddstats.o

$(BIN)/cuddbman.so: $(CUDDBMANOBJS) $(OBJ)/cuddbmanDynamic.o
	../bin/ldshared $@ $(CUDDBMANOBJS) $(OBJ)/cuddbmanDynamic.o

#------------------------------------------------------------------------

$(BIN)/abcdbman.so: $(OBJ)/abcdbman.o $(OBJ)/abcdbmanDynamic.o
	../bin/ldshared $@ $(OBJ)/abcdbman.o $(OBJ)/abcdbmanDynamic.o

#------------------------------------------------------------------------

config.h:
	@echo "*** run the configure script first" 1>&2; exit 1

#------------------------------------------------------------------------

OBJ=../obj/LINK

depend: Tokens Keywords.cc Operators.cc y.tab.c y.tab.h preinit.h  \
  TokenStrings precleanup.h
	makedepend $(INCL) -p $(OBJ)/ *.cc *.c 2>/dev/null 1>/dev/null

#------------------------------------------------------------------------

OBJS=$(OBJ)/alloc.o $(OBJ)/alloccs.o $(OBJ)/analyzer.o $(OBJ)/arr_stor.o \
  $(OBJ)/banner.o $(OBJ)/bitmatrix.o $(OBJ)/boole.o $(OBJ)/booleman.o \
  $(OBJ)/brepr.o $(OBJ)/cache.o $(OBJ)/cexbuild.o $(OBJ)/cHash.o \
  $(OBJ)/cleanup.o $(OBJ)/configurator.o $(OBJ)/context.o $(OBJ)/debug.o \
  $(OBJ)/eval.o $(OBJ)/evalalldef.o $(OBJ)/evalalloc.o $(OBJ)/evalallocone.o \
  $(OBJ)/evalalloccs.o $(OBJ)/evaldpnd.o $(OBJ)/evalfundefdep.o \
  $(OBJ)/evalint.o $(OBJ)/evalmmc.o $(OBJ)/evalmon.o $(OBJ)/evalpred.o \
  $(OBJ)/evalscc.o $(OBJ)/evalshowmapper.o $(OBJ)/evaltermmapper.o \
  $(OBJ)/evaltnc.o $(OBJ)/evalwitcs.o $(OBJ)/evalwitexp.o $(OBJ)/except.o \
  $(OBJ)/file.o $(OBJ)/forallbd.o $(OBJ)/graph.o $(OBJ)/idx.o \
  $(OBJ)/InitFileReader.o $(OBJ)/io.o $(OBJ)/iterator.o $(OBJ)/iter_vec.o \
  $(OBJ)/linkBMan.o $(OBJ)/loadBMan.o $(OBJ)/loader.o $(OBJ)/macros.o \
  $(OBJ)/main.o $(OBJ)/mem.o $(OBJ)/mu_stream.o $(OBJ)/npnftab.o \
  $(OBJ)/Parser.o $(OBJ)/pinfo.o $(OBJ)/prepare.o  $(OBJ)/Primes.o \
  $(OBJ)/print_ev.o $(OBJ)/pterm.o $(OBJ)/repr.o $(OBJ)/reprint.o \
  $(OBJ)/Scanner.o $(OBJ)/scc.o $(OBJ)/solarispinfo.o $(OBJ)/storage.o \
  $(OBJ)/Symbol.o $(OBJ)/term.o $(OBJ)/terminal.o $(OBJ)/termint.o \
  $(OBJ)/testcex.o $(OBJ)/testdump.o $(OBJ)/testons.o $(OBJ)/testPredEval.o \
  $(OBJ)/testreset.o $(OBJ)/testsize.o $(OBJ)/teststats.o $(OBJ)/testvis.o \
  $(OBJ)/Timer.o $(OBJ)/tnlabels.o $(OBJ)/Token.o $(OBJ)/useBRepr.o \
  $(OBJ)/y.tab.o $(OBJ)/init.o

#------------------------------------------------------------------------

LIBS=-lm -ldl

#------------------------------------------------------------------------

DOBJS=$(OBJ)/loadBManDynamic.o $(OBJ)/linkBManDynamic.o

#------------------------------------------------------------------------

SOBJS=$(OBJ)/loadBManStatic.o $(OBJ)/linkBManStatic.o \
  $(OBJ)/BDDsimple.o $(OBJ)/simplebman.o \
  $(OBJ)/abcdbman.o $(OBJ)/abcdbmanStatic.o

#$(OBJ)/longbman.o $(OBJ)/longbmanStatic.o \

#------------------------------------------------------------------------

$(BIN)/mucke_static_bman: $(OBJS) $(SOBJS)
	@libs="-L../bin/LINK"; \
	. ../.configuration; \
	if [ ! x"$$LONGBDD" = x ]; \
	then \
	  echo "[generating longbman objects]"; \
	  make \
	    $(OBJ)/longbman.o \
	    $(OBJ)/longbddstats.o \
	    $(OBJ)/longbmanStatic.o; \
	  libs="$$libs \
	    $(OBJ)/longbman.o \
	    $(OBJ)/longbddstats.o \
	    $(OBJ)/longbmanStatic.o"; \
	  cd ../contrib/bddlib.D.E.Long; \
	  make ../../bin/LINK/libbdd.a; \
	  libs="$$libs -lbdd"; \
	  cd ../../src; \
	fi; \
	if [ ! x"$$CUDDBDD" = x ]; \
	then \
	  echo "[generating cuddbman objects]"; \
	  make \
	    $(OBJ)/cuddbman.o \
	    $(OBJ)/cuddbddstats.o \
	    $(OBJ)/cuddbmanStatic.o; \
	  libs="$$libs \
	    $(OBJ)/cuddbman.o \
	    $(OBJ)/cuddbddstats.o \
	    $(OBJ)/cuddbmanStatic.o"; \
	  cd ../contrib/cudd/install/lib; \
	  cp libcudd.a ../../../../bin/LINK; \
	  libs="$$libs -lcudd"; \
	  cd ../../../../src; \
	fi; \
	echo "[generating abcdbman objects]"; \
	cd ../contrib/abcd/src; make ../lib/libabcd.a; cd ../../../src; \
	cp ../contrib/abcd/lib/libabcd.a $(BIN); \
	libs="$$libs -labcd"; \
	echo "[linking mucke_static_bman]"; \
	LD=$(LD) LDFLAGS=$(LDFLAGS) ../bin/ldstatic \
	  $@ $(OBJS) $(SOBJS) $$libs $(LIBS)

#------------------------------------------------------------------------

$(BIN)/mucke_dynamic_bman: $(OBJS) $(DOBJS)
	@LD="$(LD)" LDFLAGS="$(LDFLAGS)" \
	../bin/lddynamic $@ $(OBJS) $(DOBJS) $(LIBS)

#------------------------------------------------------------------------

clean:
	rm -f $(OBJS) $(SOBJS) $(DOBJS)
	rm -f preinit.h precleanup.h
	rm -f banner.c
	rm -f Tokens TokenStrings Keywords.cc Operators.cc

clean-exe:
	rm -f $(BIN)/mucke_static_bman $(BIN)/mucke_dynamic_bman

#------------------------------------------------------------------------

distclean: clean
	rm -f $(BIN)/mucke_static_bman $(BIN)/mucke_dynamic_bman
	rm -f $(BIN)/simplebman.so
	rm -f $(BIN)/longbman.a $(BIN)/bddlib.D.E.Long.so $(BIN)/libbdd.a
	rm -f $(BIN)/abcdbman.so $(BIN)/libabcd.so $(BIN)/libabcd.a
	rm -f $(BIN)/keywords
	rm -f y.tab.c y.tab.h
	makedepend 1>/dev/null 2>/dev/null

#------------------------------------------------------------------------

banner.o: banner.tmpl
banner.c: banner.tmpl
	@@echo "[generating $@]"; \
	version=`cat ../VERSION`; \
	dateStr=`sed -e '/^DATE_STR/!d' -e 's/^DATE_STR \(.*\)/\1/' config.h` ; \
	echo '  using as argument for date ' $$dateStr > /dev/null; \
	sed \
	  -e "s,<DATE>,`date "$$dateStr"`,"\
	  -e "s,<VERSION>,$$version,"\
	  -e "s,<HOST>,`uname -s`-`uname -r`-`uname -m`,"\
	  banner.tmpl > $@; \

#------------------------------------------------------------------------

INIT=booleman.cc cleanup.cc evalint.cc io.cc linkBMan.cc loadBMan.cc \
  main.cc mem.cc pinfo.cc Symbol.cc term.cc useBRepr.cc

preinit.h: $(INIT)
	../bin/genpreinit $(INIT)

init.o: preinit.h

#------------------------------------------------------------------------

CLEANUP=pinfo.cc terminal.cc

precleanup.h: $(CLEANUP)
	../bin/genprecleanup $(CLEANUP)

cleanup.o: precleanup.h

#------------------------------------------------------------------------

y.tab.c y.tab.h: mu.y
	$(YACC) -d mu.y

Tokens: y.tab.h mu.y
	@@echo "[generating $@]"; \
	( sed \
	    -e '/YY/d'\
	    -e '/#[ 	]*define/!d'\
	    -e 's/#[ 	]*define[ 	]*\([^	 ]*\)[	]*\([0-9]*\)/\1 \2/'\
	    -e 's/ / = /'\
	    -e 's/$$/,/'\
	  y.tab.h; \
	echo 'End = 0' \
	) > $@.new; \
	if [ -f $@ ];\
	then\
	  if diff $@ $@.new 1>/dev/null 2>/dev/null; \
	  then\
	    rm $@.new;\
	  else\
	    mv $@.new $@;\
	  fi;\
	else\
	  mv $@.new $@;\
	fi

#------------------------------------------------------------------------

TokenStrings: Operators.kw Keywords.kw
	@@echo "[generating $@]"; \
	gawk \
	'{if($$2!="")a[$$2]=$$1} \
	 END{for(s in a)print "token_strings[" s "] = \"" a[s] "\";"}' \
	Keywords.kw Operators.kw | \
	sed -e 's,\\,\\\\,g' > $@ || rm $@

Token.o: Keywords.cc Operators.cc TokenStrings Tokens

$(BIN)/keywords: keywords.c
	$(CC) -o $@ keywords.c

Keywords.cc: $(BIN)/keywords Keywords.kw
	$(BIN)/keywords -s str -r Token::Identifier -o $@ Keywords.kw

Operators.cc: $(BIN)/keywords Operators.kw
	$(BIN)/keywords -s str -r Token::Undefined -o $@ Operators.kw

#------------------------------------------------------------------------

$(OBJ)/abcdbman.o: abcdbman.cc
	$(CXX) -c -o $@ abcdbman.cc
$(OBJ)/abcdbmanDynamic.o: abcdbmanDynamic.cc
	$(CXX) -c -o $@ abcdbmanDynamic.cc
$(OBJ)/abcdbmanStatic.o: abcdbmanStatic.cc
	$(CXX) -c -o $@ abcdbmanStatic.cc
$(OBJ)/alloc.o: alloc.cc
	$(CXX) -c -o $@ alloc.cc
$(OBJ)/alloccs.o: alloccs.cc
	$(CXX) -c -o $@ alloccs.cc
$(OBJ)/analyzer.o: analyzer.cc
	$(CXX) -c -o $@ analyzer.cc
$(OBJ)/arr_stor.o: arr_stor.cc
	$(CXX) -c -o $@ arr_stor.cc
$(OBJ)/banner.o: banner.c
	$(CC) -c -o $@ banner.c
$(OBJ)/BDDsimple.o: BDDsimple.cc
	$(CXX) -c -o $@ BDDsimple.cc
$(OBJ)/bitmatrix.o: bitmatrix.cc
	$(CXX) -c -o $@ bitmatrix.cc
$(OBJ)/boole.o: boole.cc
	$(CXX) -c -o $@ boole.cc
$(OBJ)/booleman.o: booleman.cc
	$(CXX) -c -o $@ booleman.cc
$(OBJ)/brepr.o: brepr.cc
	$(CXX) -c -o $@ brepr.cc
$(OBJ)/cache.o: cache.cc
	$(CXX) -c -o $@ cache.cc
$(OBJ)/cexbuild.o: cexbuild.cc
	$(CXX) -c -o $@ cexbuild.cc
$(OBJ)/cHash.o: cHash.cc
	$(CXX) -c -o $@ cHash.cc
$(OBJ)/cleanup.o: cleanup.cc
	$(CXX) -c -o $@ cleanup.cc
$(OBJ)/configurator.o: configurator.cc
	$(CXX) -c -o $@ configurator.cc
$(OBJ)/context.o: context.cc
	$(CXX) -c -o $@ context.cc
$(OBJ)/debug.o: debug.cc
	$(CXX) -c -o $@ debug.cc
$(OBJ)/eval.o: eval.cc
	$(CXX) -c -o $@ eval.cc
$(OBJ)/evalalldef.o: evalalldef.cc
	$(CXX) -c -o $@ evalalldef.cc
$(OBJ)/evalalloc.o: evalalloc.cc
	$(CXX) -c -o $@ evalalloc.cc
$(OBJ)/evalallocone.o: evalallocone.cc
	$(CXX) -c -o $@ evalallocone.cc
$(OBJ)/evalalloccs.o: evalalloccs.cc
	$(CXX) -c -o $@ evalalloccs.cc
$(OBJ)/evaldpnd.o: evaldpnd.cc
	$(CXX) -c -o $@ evaldpnd.cc
$(OBJ)/evalfundefdep.o: evalfundefdep.cc
	$(CXX) -c -o $@ evalfundefdep.cc
$(OBJ)/evalint.o: evalint.cc
	$(CXX) -c -o $@ evalint.cc
$(OBJ)/evalmmc.o: evalmmc.cc
	$(CXX) -c -o $@ evalmmc.cc
$(OBJ)/evalmon.o: evalmon.cc
	$(CXX) -c -o $@ evalmon.cc
$(OBJ)/evalpred.o: evalpred.cc
	$(CXX) -c -o $@ evalpred.cc
$(OBJ)/evalscc.o: evalscc.cc
	$(CXX) -c -o $@ evalscc.cc
$(OBJ)/evalshowmapper.o: evalshowmapper.cc
	$(CXX) -c -o $@ evalshowmapper.cc
$(OBJ)/evaltermmapper.o: evaltermmapper.cc
	$(CXX) -c -o $@ evaltermmapper.cc
$(OBJ)/evaltnc.o: evaltnc.cc
	$(CXX) -c -o $@ evaltnc.cc
$(OBJ)/evalwitcs.o: evalwitcs.cc
	$(CXX) -c -o $@ evalwitcs.cc
$(OBJ)/evalwitexp.o: evalwitexp.cc
	$(CXX) -c -o $@ evalwitexp.cc
$(OBJ)/except.o: except.cc
	$(CXX) -c -o $@ except.cc
$(OBJ)/file.o: file.cc
	$(CXX) -c -o $@ file.cc
$(OBJ)/forallbd.o: forallbd.cc
	$(CXX) -c -o $@ forallbd.cc
$(OBJ)/graph.o: graph.cc
	$(CXX) -c -o $@ graph.cc
$(OBJ)/idx.o: idx.cc
	$(CXX) -c -o $@ idx.cc
$(OBJ)/init.o: init.cc
	$(CXX) -c -o $@ init.cc
$(OBJ)/InitFileReader.o: InitFileReader.cc
	$(CXX) -c -o $@ InitFileReader.cc
$(OBJ)/io.o: io.cc
	$(CXX) -c -o $@ io.cc
$(OBJ)/iterator.o: iterator.cc
	$(CXX) -c -o $@ iterator.cc
$(OBJ)/iter_vec.o: iter_vec.cc
	$(CXX) -c -o $@ iter_vec.cc
$(OBJ)/linkBMan.o: linkBMan.cc
	$(CXX) -c -o $@ linkBMan.cc
$(OBJ)/linkBManDynamic.o: linkBManDynamic.cc
	$(CXX) -c -o $@ linkBManDynamic.cc
$(OBJ)/linkBManStatic.o: linkBManStatic.cc
	$(CXX) -c -o $@ linkBManStatic.cc
$(OBJ)/loadBMan.o: loadBMan.cc
	$(CXX) -c -o $@ loadBMan.cc
$(OBJ)/loadBManDynamic.o: loadBManDynamic.cc
	$(CXX) -c -o $@ loadBManDynamic.cc
$(OBJ)/loadBManStatic.o: loadBManStatic.cc
	$(CXX) -c -o $@ loadBManStatic.cc
$(OBJ)/loader.o: loader.cc
	$(CXX) -c -o $@ loader.cc
$(OBJ)/longbman.o: longbman.cc
	$(CXX) -c -o $@ longbman.cc
$(OBJ)/longbddstats.o: longbddstats.c
	$(CC) $(INCL) -c -o $@ longbddstats.c
$(OBJ)/longbmanDynamic.o: longbmanDynamic.cc
	$(CXX) -c -o $@ longbmanDynamic.cc
$(OBJ)/longbmanStatic.o: longbmanStatic.cc
	$(CXX) $(INCL) -c -o $@ longbmanStatic.cc
$(OBJ)/cuddbman.o: cuddbman.cc
	$(CXX) -c -o $@ cuddbman.cc
$(OBJ)/cuddbddstats.o: cuddbddstats.c
	$(CC) $(INCL) -c -o $@ cuddbddstats.c
$(OBJ)/cuddbmanDynamic.o: cuddbmanDynamic.cc
	$(CXX) -c -o $@ cuddbmanDynamic.cc
$(OBJ)/cuddbmanStatic.o: cuddbmanStatic.cc
	$(CXX) $(INCL) -c -o $@ cuddbmanStatic.cc
$(OBJ)/macros.o: macros.cc
	$(CXX) -c -o $@ macros.cc
$(OBJ)/main.o: main.cc
	$(CXX) -c -o $@ main.cc
$(OBJ)/mem.o: mem.cc
	$(CXX) -c -o $@ mem.cc
$(OBJ)/mu_stream.o: mu_stream.cc
	$(CXX) -c -o $@ mu_stream.cc
$(OBJ)/npnftab.o: npnftab.cc
	$(CXX) -c -o $@ npnftab.cc
$(OBJ)/Parser.o: Parser.cc
	$(CXX) -c -o $@ Parser.cc
$(OBJ)/pinfo.o: pinfo.cc
	$(CXX) -c -o $@ pinfo.cc
$(OBJ)/prepare.o: prepare.cc
	$(CXX) -c -o $@ prepare.cc
$(OBJ)/Primes.o: Primes.cc
	$(CXX) -c -o $@ Primes.cc
$(OBJ)/print_ev.o: print_ev.cc
	$(CXX) -c -o $@ print_ev.cc
$(OBJ)/pterm.o: pterm.c
	$(CC) -c -o $@ pterm.c
$(OBJ)/repr.o: repr.cc
	$(CXX) -c -o $@ repr.cc
$(OBJ)/reprint.o: reprint.cc
	$(CXX) -c -o $@ reprint.cc
$(OBJ)/Scanner.o: Scanner.cc
	$(CXX) -c -o $@ Scanner.cc
$(OBJ)/scc.o: scc.cc
	$(CXX) -c -o $@ scc.cc
$(OBJ)/simplebman.o: simplebman.cc
	$(CXX) -c -o $@ simplebman.cc
$(OBJ)/solarispinfo.o: solarispinfo.c
	$(CC) -c -o $@ solarispinfo.c
$(OBJ)/storage.o: storage.cc
	$(CXX) -c -o $@ storage.cc
$(OBJ)/Symbol.o: Symbol.cc
	$(CXX) -c -o $@ Symbol.cc
$(OBJ)/term.o: term.cc
	$(CXX) -c -o $@ term.cc
$(OBJ)/terminal.o: terminal.cc
	$(CXX) -c -o $@ terminal.cc
$(OBJ)/termint.o: termint.cc
	$(CXX) -c -o $@ termint.cc
$(OBJ)/testcex.o: testcex.cc
	$(CXX) -c -o $@ testcex.cc
$(OBJ)/testdump.o: testdump.cc
	$(CXX) -c -o $@ testdump.cc
$(OBJ)/testons.o: testons.cc
	$(CXX) -c -o $@ testons.cc
$(OBJ)/testPredEval.o: testPredEval.cc
	$(CXX) -c -o $@ testPredEval.cc
$(OBJ)/testreset.o: testreset.cc
	$(CXX) -c -o $@ testreset.cc
$(OBJ)/testsize.o: testsize.cc
	$(CXX) -c -o $@ testsize.cc
$(OBJ)/teststats.o: teststats.cc
	$(CXX) -c -o $@ teststats.cc
$(OBJ)/testvis.o: testvis.cc
	$(CXX) -c -o $@ testvis.cc
$(OBJ)/Timer.o: Timer.cc
	$(CXX) -c -o $@ Timer.cc
$(OBJ)/tnlabels.o: tnlabels.cc
	$(CXX) -c -o $@ tnlabels.cc
$(OBJ)/Token.o: Token.cc
	$(CXX) -c -o $@ Token.cc
$(OBJ)/useBRepr.o: useBRepr.cc
	$(CXX) -c -o $@ useBRepr.cc
$(OBJ)/y.tab.o: y.tab.c
	$(CCNOWARN) -c -o $@ y.tab.c

#------------------------------------------------------------------------

.PHONY: all clean distclean dynamic static
# DO NOT DELETE
