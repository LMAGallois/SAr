#ifndef solver_alone_h
#define solver_alone_h

#include <stdio.h>
#include <vector>
#include <iostream>
#include <list>
#include "ilcplex/ilocplex.h"
#include "ilconcert/ilomodel.h"
#include <chrono>
#include <future>
#include <thread>
#include <condition_variable>
#include <time.h>

using namespace std;
using namespace std::literals::chrono_literals;

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
        int nb_contacts;
    };

ILOSTLBEGIN

void solveMIP(vector< vector < vector <int> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, vector< vector <schedule> > &creneaux,vector<float> &contacts, defined_data &its_data, list<schedule> plan);
void get_plan(list<schedule> plan);
void solveMIP_basic(vector< vector < vector <int> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, vector< vector <schedule> > &creneaux, vector<float> &contacts, defined_data &its_data, list<schedule> plan);
void solveMIP_time(vector< vector < vector <int> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, vector< vector <schedule> > &creneaux, vector<float> &contacts, defined_data &its_data,list<schedule> plan);
void get_values (defined_data &its_data, vector< vector < vector <int> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, vector< vector <schedule> > &creneaux,vector<float> &contacts);


#endif