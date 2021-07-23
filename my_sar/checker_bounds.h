#ifndef checker_bounds_h
#define checker_bounds_h

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <fstream>
#include <limits.h>
#include "parser.h"
#include "solver.h"

using namespace std;

void parse_contacts_user(vector<float> &contacts, int user, defined_data its_data);
void parse_matrix_u(vector < float > &contacts ,vector< vector < vector <int> > > &userToSat, defined_data its_data, int user);
float check_bounds( vector < vector < vector < int> > > &userToSat, defined_data its_data, vector<float> &contacts,int nbSatCov);
void get_upper_bound (int nbSatCov);


#endif