/////////////////////////////////////////////
/*
parseur pour les données des fenetres de visibilité entre utilisateurs et satellites sur 10 jours et les fenetres entre sites et satellites des 5 sites.
les matrices d'adjacence à 3dim (user/site, satellites et contacts) sont ainsi remplis (valeur entre 0 et 1 => 1: en vue ; 0: pas dans le champ)
*/
/////////////////////////////////////////////
#ifndef parser_h
#define parser_h

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <list>

using namespace std;

struct schedule {
    int sat;
    float s;
    float e;
};

struct defined_data{
    int horizon_c;
    int nbUsers;
    int nbSatellites;
    int nbSites;
    int nbAntennas;
    float time_slot_sp3;
};

defined_data get_data ();

void parse_contacts(vector<float> &contacts, defined_data its_data);

void parse_matrix_u( vector < float>  &contacts,vector< vector < vector <int> > > &userToSat, defined_data its_data);
void parse_matrix_s(vector < float> &contacts, vector< vector < vector <int> > > &antennaToSat, defined_data its_data);

void display_3Dmatrix(vector< vector < vector<int> > > &matrice);
void display_3Dmatrix_f(vector< vector < vector<float> > > &matrice);
void display_array(vector<float> &matrice);

#endif