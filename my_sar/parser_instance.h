#ifndef parser_instance_h
#define parser_instance_h

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

struct v_3{
    float x;
    float y;
    float z;
};

void get_contacts (vector< vector < vector <float> > > &contacts_sites, defined_data its_data);
void get_trackSatInterpolate (vector < vector <v_3> > &trackSat, vector < vector <v_3> > &trackSatInterpolate, defined_data its_data);
void construct_visi_aP(vector< vector < vector <int> > > &antennaToSat, vector< vector < vector <float> > > &contacts, vector<vector <vector< vector < vector <int> > > > > &antennaToSatF, defined_data its_data);
void reconstruct_visi_a(vector< vector < vector <int> > > &antennaToSat, vector< vector < vector <float> > > &contacts, vector< vector < vector <int> > > &antennaToSatF, defined_data its_data);
void get_visi_a_thales(vector < vector <v_3> > &trackSat, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <int> > > &u_or_aToSat, vector< vector < vector<float> > > contacts_sites);
void get_visi_a(vector < vector <v_3> > &trackSat, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <int> > > &u_or_aToSat);
void parse_contacts_sites_i(vector< vector < vector <int> > > &antennaToSat, vector< vector < vector <float> > > &contacts, defined_data its_data);
void parse_coords_u(vector<v_3> &userCoords, defined_data its_data);
void parse_site(vector<v_3> &coorSite, defined_data its_data);
void parse_eph(vector < vector <v_3> > &trackSat, defined_data its_data, const char* path);
void get_visi_u(vector < vector <v_3> > &trackSat, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <float> > > &u_or_aToSat, vector< vector < vector<float> > > contacts_sites);
void get_visi_u_all(vector < vector <v_3> > &trackSat, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <float> > > &u_or_aToSat);
void get_visi_aP( vector <vector <v_3> > &trackSat, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <vector< vector<int> > > > > &u_or_aToSat, vector< vector < vector<float> > > contacts_sites);
void display_array_h(vector<v_3> &matrice);
void display_matrix_h(vector< vector<v_3> > &matrice);
void display_3Dmatrix_h(vector< vector < vector<int> > > &matrice, defined_data its_data);
void display_3Dmatrix_hf(vector< vector < vector<float> > > &matrice, defined_data its_data);
void display_4Dmatrix_h(vector <vector< vector < vector<v_3> > > > &matrice);

#endif