SYSTEM     = x86-64_linux

# ---------------------------------------------------------------------
# Compiler selection 
# ---------------------------------------------------------------------

CCC = g++

# ---------------------------------------------------------------------
# Compiler options 
# ---------------------------------------------------------------------

CCOPT = -m64 -fPIC -fno-strict-aliasing -fexceptions -DNDEBUG -std=c++14

all:
	make all_cpp

CCFLAGS = $(CCOPT)

CPP_EX = paper_schedule

all_cpp: $(CPP_EX)

clean :
	/bin/rm -rf *.o *~ *.class
	/bin/rm -rf $(CPP_EX)

paper_schedule: main.o a_i.o s_i.o u_i.o parser_eph.o parser_visi.o parser_eph2.o
	$(CCC) $(CCFLAGS) -o paper_schedule main.o a_i.o s_i.o u_i.o parser_eph.o parser_visi.o parser_eph2.o $(CCLNFLAGS)


main.o: main.cpp
	$(CCC) -c $(CCFLAGS) main.cpp -o main.o

a_i.o: a_i.cpp
	$(CCC) -c $(CCFLAGS) a_i.cpp -o a_i.o

s_i.o: s_i.cpp
	$(CCC) -c $(CCFLAGS) s_i.cpp -o s_i.o

u_i.o: u_i.cpp
	$(CCC) -c $(CCFLAGS) u_i.cpp -o u_i.o

parser_eph.o: parser_eph.cpp
	$(CCC) -c $(CCFLAGS) parser_eph.cpp -o parser_eph.o

parser_visi.o: parser_visi.cpp
	$(CCC) -c $(CCFLAGS) parser_visi.cpp -o parser_visi.o

parser_eph2.o: parser_eph2.cpp
	$(CCC) -c $(CCFLAGS) parser_eph2.cpp -o parser_eph2.o