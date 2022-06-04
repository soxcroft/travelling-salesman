#
# Makefile for travelling salesman
#

# compiler flags
DEBUG    = -ggdb
OPTIMISE = -O2
WARNINGS = -Wall -Wextra -Wno-variadic-macros -Wno-overlength-strings -pedantic
CFLAGS   = $(DEBUG) $(OPTIMISE) $(WARNINGS)
DFLAGS   = -DDEBUG

CC       = clang
MPICC    = mpicc
RM       = rm -f
#COMPILE  = $(CC) $(CFLAGS) $(DFLAGS)
COMPILE  = $(MPICC) $(CFLAGS) $(DFLAGS)
INSTALL  = install

# files
EXES = tsp testgraph teststack

BINDIR = ../bin

# RULES

tsp: tsp.c stack.o graph.o | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^

teststack: teststack.c stack.o | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^

testgraph: testgraph.c graph.o | $(BINDIR)
	$(COMPILE) -o $(BINDIR)/$@ $^

# units

stack.o: stack.c stack.h
	$(COMPILE) -c $<

graph.o: graph.c graph.h
	$(COMPILE) -c $<

# PHONY TARGETS

clean:
	$(RM) $(foreach EXEFILE, $(EXES), $(BINDIR)/$(EXEFILE))
	$(RM) *.o
	$(RM) -rf $(BINDIR)/*.dSYM
