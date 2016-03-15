all: bin/vfunc bin/ddispatch

VFUNC_SDIR=src/vfunc/
DD_SDIR=src/ddispatch/

bin/vfunc: $(VFUNC_SDIR)main.cpp \
	$(VFUNC_SDIR)shapes.cpp $(VFUNC_SDIR)shapes.h
	g++ -Wall -Werror -std=c++11 $(wordlist 1,2,$^) -o $@

bin/ddispatch: $(DD_SDIR)/main.cpp $(DD_SDIR)shapes.cpp \
	$(DD_SDIR)/shapes.h $(DD_SDIR)/double_dispatcher.h
	g++ -Wall -Werror -std=c++11 $(wordlist 1,2,$^) -o $@

clean:
	rm -rf bin/*
