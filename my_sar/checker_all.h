/*
A utiliser avec nouveaux contacts
Checker à utiliser avec des slots de visibilité (pas de contact) à durée constante.
Ce sont les slots des matrices de visibilité userToSat et antennaToSat.
Elles sont basées sur l'inteprolation des éphémérides des satellites.
C'est à dire, si la dureeSlot est de 60s alors nous avons tous les 60s l'emplacement du satellite dans le ciel.

Attention donc à ajuster dureeSlot dans le programme en fonction
*/

#ifndef checker_all_h
#define checker_all_h

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <fstream>
#include "parser.h"
#include "parser_instance.h"

using namespace std;

void check_1sat_all(    vector< vector < vector<float> > > userToSat, defined_data its_data);
void check_2sat_handover_all(    vector< vector < vector<float> > > userToSat, defined_data its_data);
void check_1sat_handover_all(    vector< vector < vector<float> > > userToSat, defined_data its_data);
void check_2sat_all(    vector< vector < vector<float> > > userToSat, defined_data its_data);

#endif
