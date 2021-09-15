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

using namespace std;

ILOSTLBEGIN

struct schedule_ssc {
    int s;
    int e;
    int contact1;
    int contact11;
    int contact111;
    int sat;
    int site;
    int antenna;

};

vector<schedule_ssc> solveMIP_ssc(vector< vector < vector<float> > > contacts_sites, vector< vector < vector <float> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, defined_data its_data,int contact1, int contact2);

#endif