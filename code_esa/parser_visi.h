/////////////////////////////////////////////
/*
parseur pour les données des fenetres de visibilité entre utilisateurs et satellites sur 10 jours et les fenetres entre sites et satellites des 5 sites.
les matrices d'adjacence à 3dim (user/site, satellites et contacts) sont ainsi remplis (valeur entre 0 et 1 => 1: en vue ; 0: pas dans le champ)
*/
/////////////////////////////////////////////
#ifndef parser_visi_h
#define parser_visi_h

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <list>

using namespace std;

struct v_3{
    float x;
    float y;
    float z;
};

 struct defined_data{
    int nb_Antennas;
    int nb_ULS_s;
    int horizon_c;
    int nb_SVs;
    vector<int> SVs_identity;
    int time_slot_sp3;
};

defined_data parse_config();
void parse_matrix_s( vector< vector < vector <int> > > &antennaToSat, defined_data its_data);
void parse_matrix_u( vector< vector < vector <int> > > &userToSat, defined_data its_data);
void display_matrix(vector< vector<v_3> > &matrice);
void display_3Dmatrix(vector< vector < vector<int> > > &matrice);
void display_array(vector<v_3> &matrice);

#endif