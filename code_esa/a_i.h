#ifndef a_i_h
#define a_i_h

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include "parser_visi.h"
#include "u_i.h"
#include <limits.h>


using namespace std;

float ele_a(vector < vector <v_3> > &trackSat, int sat, int c);
vector<int> a_i(vector < vector <v_3> > &trackSat,vector <int> former_site, vector<int> former_sat,vector<int> _a_1, vector<int> _a_2, vector <int> &avail_a, vector <int> &cd_sat, vector <int> &dd_sat, int c, defined_data its_data);

#endif