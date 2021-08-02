#ifndef checker_h
#define checker_h

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <fstream>
#include "parser.h"

using namespace std;

void check_1sat(    vector< vector < vector<float> > > userToSat, defined_data its_data);
void check_2sat_handover(    vector< vector < vector<float> > > userToSat, defined_data its_data);
void check_1sat_handover(    vector< vector < vector<float> > > userToSat, defined_data its_data);
void check_2sat(    vector< vector < vector<float> > > userToSat, defined_data its_data);

#endif
