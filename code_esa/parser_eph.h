#ifndef parser_eph_h
#define parser_eph_h

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <list>
#include <vector>
#include "parser_visi.h"

using namespace std;

void parse_site(vector<v_3> &coorSite, defined_data its_data);
void parse_eph(vector < vector <v_3> > &trackSat, defined_data its_data, const char* path);
float ele_a_20(v_3 sat , v_3 on_earth);
void get_visi(vector < vector <v_3> > &trackSat, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <int> > > &u_or_aToSat);

#endif