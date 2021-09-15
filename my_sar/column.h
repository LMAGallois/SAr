/*
début de fichier non utilisé pour la génération de colonnes
*/

#ifndef column_h
#define column_h

#include <stdio.h>
#include <vector>
#include <iostream>
#include <list>
#include "ilcplex/ilocplex.h"
#include "ilconcert/ilomodel.h"

using namespace std;

 struct defined_data{
        int horizon_c;
        int nbUsers;
        int nbSatellites;
        int nbSites;
        int nbAntennas;
        int nb_contacts;
    };

struct output{
    double obj;
    vector<double> gamma(its_data.nb_contacts);
}

ILOSTLBEGIN

void solveMIP(vector< vector < vector <int> > > userToSat, vector< vector < vector <int> > > antennaToSat, vector< vector <schedule> > creneaux,vector<float> contacts, defined_data its_data, list<schedule> plan);
void get_plan(list<schedule> plan);

#endif