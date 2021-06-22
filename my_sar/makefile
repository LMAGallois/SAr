SYSTEM     = x86-64_linux
LIBFORMAT  = static_pic

#------------------------------------------------------------
#
# When you adapt this makefile to compile your CPLEX programs
# please copy this makefile and set CPLEXDIR and CONCERTDIR to
# the directories where CPLEX and CONCERT are installed.
#
#------------------------------------------------------------

CPLEXDIR      = /home/lgallois/opt/ibm/ILOG/CPLEX_Studio201/cplex
CONCERTDIR    = /home/lgallois/opt/ibm/ILOG/CPLEX_Studio201/concert

# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------

CCC = g++

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

CCOPT = -m64 -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -std=c++14

# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXJARDIR   = $(CPLEXDIR)/lib/cplex.jar
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

# For dynamic linking
CPLEXBINDIR   = $(CPLEXDIR)/bin/$(SYSTEM)
CPLEXLIB      = cplex$(dynamic:yes=2010)

CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR) $(dynamic:yes=-L$(CPLEXBINDIR))
CCLNFLAGS = -lconcert -lilocplex -l$(CPLEXLIB) -lm -lpthread -ldl

all:
	make all_cpp

parse:
	make parse_cpp

solve:
	make solve_cpp

CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) 

CPP_EX = sched_Cband

CPP_PA = parser_alone

CPP_SO = solver_alone

all_cpp: $(CPP_EX)

parse_cpp: $(CPP_PA)

solve_cpp: $(CPP_SO)

clean :
	/bin/rm -rf *.o *~ *.class
	/bin/rm -rf $(CPP_EX)
	/bin/rm -rf *.mps *.ord *.sos *.lp *.sav *.net *.msg *.log *.clp

parser_alone: parser_alone.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o parser_alone parser_alone.o $(CCLNFLAGS)

solver_alone: solver_alone.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o solver_alone solver_alone.o $(CCLNFLAGS)

parser_alone.o: parser_alone.cpp
	$(CCC) -c $(CCFLAGS) parser_alone.cpp -o parser_alone.o

solver_alone.o: solver_alone.cpp
	$(CCC) -c $(CCFLAGS) solver_alone.cpp -o solver_alone.o

sched_Cband: main.o solver.o parser.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS) -o sched_Cband main.o solver.o parser.o $(CCLNFLAGS)

main.o:  main.cpp
	$(CCC) -c $(CCFLAGS) main.cpp -o main.o

parser.o:  parser.cpp
	$(CCC) -c $(CCFLAGS)  parser.cpp -o parser.o

solver.o:  solver.cpp
	$(CCC) -c $(CCFLAGS)  solver.cpp -o solver.o