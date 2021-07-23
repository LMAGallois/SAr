#ifndef solver_sites_h
#define solver_sites_h

#include <stdio.h>
#include <vector>
#include <iostream>
#include <list>
#include "ilcplex/ilocplex.h"
#include "ilconcert/ilomodel.h"
#include "parser_sites.h"
#include "parser.h"
#include "solver.h"

using namespace std;

ILOSTLBEGIN

struct schedule_ssc {
    int s;
    int e;
    int sat;
    int site;
    int antenna;

};

void get_plan_checker(list<schedule_ssc> plan);
void solveMIP_ssc(vector< vector < vector <int> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, defined_data its_data,int contact1, int contact2);

#endif