#ifndef solver_sites_60_h
#define solver_sites_60_h

#include <stdio.h>
#include <vector>
#include <iostream>
#include <list>
#include "ilcplex/ilocplex.h"
#include "ilconcert/ilomodel.h"
#include "parser_sites_60.h"
#include "parser.h"
#include "solver.h"
#include "solver_sites.h"

using namespace std;

ILOSTLBEGIN

vector<schedule_ssc> solveMIP_ssc_60(vector< vector<float> > contacts_sites, vector< vector < vector <float> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, defined_data its_data,int contact1, int contact2);

#endif