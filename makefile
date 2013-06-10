# Generic GNUMakefile
 
# Just a snippet to stop executing under other make(1) commands
# that won't understand these lines
ifneq (,)
This makefile requires GNU Make.
endif
 
PROGRAM = mesitype-tester
CPP_FILES := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(CPP_FILES))
CC = g++
CPPFLAGS = -Wall -pedantic -std=c++11
LDFLAGS =
 
all: $(PROGRAM) version
 
$(PROGRAM): .depend $(OBJS)
	$(CC) $(CPPFLAGS) $(OBJS) $(LDFLAGS) -o $(PROGRAM)
 
depend: .depend
 
.depend: cmd = g++ -MM -MF depend $(var); cat depend >> .depend;
.depend:
	@echo "Generating dependencies..."
	@$(foreach var, $(CPP_FILES), $(cmd))
	@rm -f depend
 
-include .depend
 
version:
	echo "#define VERSION \"` date +%s `\"" > version.inc

# These are the pattern matching rules. In addition to the automatic
# variables used here, the variable $* that matches whatever % stands for
# can be useful in special cases.
%.o: %.cpp
	$(CC) $(CPPFLAGS) -c $< -o $@
 
%: %.cpp
	$(CC) $(CPPFLAGS) -o $@ $<
 
clean:
	rm -f .depend *.o
 
.PHONY: clean depend
