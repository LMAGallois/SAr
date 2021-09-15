#ifndef u_i_h
#define u_i_h

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include "parser_visi.h"
#include <limits.h>
#include "parser_eph.h"

using namespace std;

int min(int a, int b);
float ele_a(vector < vector <v_3> > &trackSat, int sat, int c);
//int in_view(vector< vector < vector <int> > > &antennaToSat, int site, int sat, int c, defined_data its_data);
vector<int> u_i( vector<int> &dd_sat, vector <v_3> &coorSite, vector < vector <v_3> > &trackSat, vector< vector < vector <int> > > &antennaToSat, vector<int> s_i, int contact, defined_data its_data);


#endif