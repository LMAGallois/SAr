//
//  ImproveGraph.h
//  
//
//  Created by Laurent Houssin on 01/11/2019.
//

#ifndef ImproveGraph_h
#define ImproveGraph_h

#include <stdio.h>
#include "Contact.h"
#include "User.h"
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


void ImproveSat(vector <vector <int> >  vX , vector <Satellite>  &tabSat , vector <User> &tabUser );

void ImproveContactSat(ContactSat cs , vector <User> &tabU );


#endif /* ImproveGraph_h */
