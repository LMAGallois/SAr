#ifndef parser_sites_h
#define parser_sites_h

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

void parse_contacts_sites(vector < vector <vector <float> > > &contacts, defined_data its_data);
void parse_matrix_s(vector < vector <vector <float> > > &contacts, vector< vector < vector <int> > > &antennaToSat, defined_data its_data);
void parse_matrix_u(vector < vector <vector <float> > > &contacts ,vector< vector < vector <int> > > &userToSat, defined_data its_data);
void get_parser_sites (vector< vector < vector<float> > > &contacts_sites,vector< vector < vector <int> > > &userToSat,vector< vector < vector <int> > >&antennaToSat, vector< vector < vector <int> > > &antennaToSatP, defined_data its_data );
void parse_matrix_s_precise(vector< vector < vector<float> > > &contacts_sites, vector< vector < vector <int> > > &antennaToSatP, defined_data its_data);

#endif