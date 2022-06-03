#
# Makefile for travelling salesman
#

# compiler flags
DEBUG    = -ggdb
OPTIMISE = -O2
WARNINGS = -Wall -Wextra -Wno-variadic-macros -Wno-overlength-strings -pedantic
CFLAGS   = $(DEBUG) $(OPTIMISE) $(WARNINGS)
#DFLAGS = -TODO

CC       = clang
RM       = rm -f
COMPILE  = $(CC) $(CFLAGS) $(DFLAGS)
INSTALL  = install

# files
EXES = testgraph teststack

BINDIR = ../bin

# RULES

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
