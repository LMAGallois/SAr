SYSTEM     = x86-64_osx
LIBFORMAT  = static_pic

#------------------------------------------------------------
#
# When you adapt this makefile to compile your CPLEX programs
# please copy this makefile and set CPLEXDIR and CONCERTDIR to
# the directories where CPLEX and CONCERT are installed.
#
#------------------------------------------------------------

CPLEXDIR      = /Users/lhoussin/CPLEX_Studio129/cplex
CONCERTDIR    = /Users/lhoussin/CPLEX_Studio129/concert

# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------

CCC = g++ -O0


# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

CCOPT = -m64 -O -fPIC -fexceptions -DNDEBUG -DIL_STD -stdlib=libc++


# ---------------------------------------------------------------------
# Link options and libraries
# ---------------------------------------------------------------------

CPLEXBINDIR   = $(CPLEXDIR)/bin/$(BINDIST)
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)

# For dynamic linking
CPLEXBINDIR   = $(CPLEXDIR)/bin/$(SYSTEM)
CPLEXLIB      = cplex$(dynamic:yes=1290)
run           = $(dynamic:yes=DYLD_LIBRARY_PATH=$(CPLEXBINDIR))

CCLNDIRS  = -L$(CPLEXLIBDIR) -L$(CONCERTLIBDIR) $(dynamic:yes=-L$(CPLEXBINDIR))
CCLNFLAGS = -lconcert -lilocplex -l$(CPLEXLIB) -m64 -lm -lpthread  -stdlib=libc++



CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include

CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR) 




#-----------------------------------------------------------------

sched_Cband : main.o User.o Contact.o read_data.o CAntennaSite.o Satellite.o ContactSat.o MIP.o ImproveGraph.o
	$(CCC) $(CCFLAGS) $(CCLNDIRS)  -o sched_Cband main.o User.o Contact.o read_data.o CAntennaSite.o Satellite.o  ContactSat.o MIP.o ImproveGraph.o $(CCLNFLAGS)



main.o : main.cpp
	$(CCC) -c $(CCFLAGS)  -c main.cpp 
User.o : User.cpp
	g++ -c User.cpp
Contact.o : Contact.cpp
	g++ -c Contact.cpp
read_data.o : read_data.cpp
	g++ -c read_data.cpp
CAntennaSite.o : CAntennaSite.cpp
	g++ -c CAntennaSite.cpp
Satellite.o : Satellite.cpp
	g++ -c Satellite.cpp
ContactSat.o : ContactSat.cpp
	g++ -c ContactSat.cpp
MIP.o : MIP.cpp
	$(CCC) -c $(CCFLAGS)  MIP.cpp -o MIP.o 
ImproveGraph.o : ImproveGraph.cpp
	$(CCC) -c $(CCFLAGS)  ImproveGraph.cpp -o ImproveGraph.o 

clean:
	rm -rf *.o sched_cband 
