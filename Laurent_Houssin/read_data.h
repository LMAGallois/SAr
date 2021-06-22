//
//  read_data.h
//  
//
//  Created by Laurent Houssin on 18/09/2019.
//

#ifndef read_data_h
#define read_data_h



#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include "User.h"
#include "CAntennaSite.h"

using namespace std;

#define nbSites 5
#define nbAntennas 4

int read_data_User( vector<User> & tab);        // return nbSat
void read_data_Antenna( vector<CAntennaSite> & tab);


#endif /* read_data_h */
