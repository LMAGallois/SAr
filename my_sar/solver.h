#ifndef solver_h
#define solver_h

#include <stdio.h>
#include <vector>
#include <iostream>
#include <list>
#include "ilcplex/ilocplex.h"
#include "ilconcert/ilomodel.h"
#include "parser.h"

using namespace std;

ILOSTLBEGIN

void solveMIP(vector< vector < vector <int> > > userToSat, vector< vector < vector <int> > > antennaToSat, vector< vector <schedule> > creneaux,vector<float> contacts, defined_data its_data, list<schedule> plan);
void get_plan(list<schedule> plan);
void solveMIP_basic(vector< vector < vector <int> > > userToSat, vector< vector < vector <int> > > antennaToSat, vector< vector <schedule> > creneaux, vector<float> contacts, defined_data its_data, list<schedule> plan);
void solveMIP_time(vector< vector < vector <int> > > userToSat, vector< vector < vector <int> > > antennaToSat, vector< vector <schedule> > creneaux, vector<float> contacts, defined_data its_data,list<schedule> plan);

#endif