#ifndef parser_instance_60_h
#define parser_instance_60_h

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <fstream>
#include <limits.h>
#include "parser.h"
#include "parser_instance.h"
#include "solver.h"

using namespace std;

void get_visi_u_60(vector < vector <v_3> > &trackSat, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <float> > > &u_or_aToSat,vector < vector<float> > contacts_sites);
void get_visi_a_60_thales(vector < vector <v_3> > &trackSat, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <int> > > &u_or_aToSat, vector < vector<float> > contacts_sites);
void parse_contacts_sites_60_i(vector< vector < vector <int> > > &antennaToSat, vector < vector <float> > &contacts, defined_data its_data);
void reconstruct_visi_a_60(vector< vector < vector <int> > > &antennaToSat, vector < vector <float> > &contacts, vector< vector < vector <int> > > &antennaToSatF, defined_data its_data);

#endif