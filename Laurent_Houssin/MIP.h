//
//  MIP.h
//  
//
//  Created by Laurent Houssin on 23/10/2019.
//

#ifndef MIP_h
#define MIP_h

#include <stdio.h>
#include <ilcplex/ilocplex.h>
#include "Contact.h"
#include "CAntennaSite.h"
#include "Satellite.h"

#define nbSites 5
#define nbAntennas 4
#define Horizon 907200
#define OSContactPeriod 1200
#define OSContactDuration 120
#define GnavContactPeriod 10800
#define GnavContactDuration 100


using namespace std;

ILOSTLBEGIN

int solveMIP(vector <Satellite>  tabSat , vector <CAntennaSite> &tabS, vector <vector <int> >  &vX);

#endif /* MIP_h */
