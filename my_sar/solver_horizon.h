#ifndef solver_horizon_h
#define solver_horizon_h

#include <stdio.h>
#include <vector>
#include <iostream>
#include <list>
#include "ilcplex/ilocplex.h"
#include "ilconcert/ilomodel.h"
#include "parser_sites.h"
#include "parser.h"
#include "solver_sites.h"

using namespace std;

ILOSTLBEGIN

void solveMIP_ssc_horizon(vector< vector < vector<float> > > contacts_sites, vector< vector < vector <float> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, defined_data its_data);

#endif