
#ifndef parser_eph2_h
#define parser_eph2_h

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include "parser_eph.h"
#include "parser_visi.h"

using namespace std;

void parse_coords_u(vector<v_3> &userCoords, defined_data its_data);
void get_coords_sat(vector< vector <v_3> > &trackSat, vector<v_3> &userCoords, vector< vector < vector <int> > > &userToSat, defined_data its_data);

#endif