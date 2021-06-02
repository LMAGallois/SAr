//
//  Satellite.hpp
//  
//
//  Created by Laurent Houssin on 15/10/2019.
//

#ifndef Satellite_hpp
#define Satellite_hpp


#include <iostream>
#include <vector>
#include "ContactSat.h"
#include "CAntennaSite.h"

#define nbSites 5
#define nbAntennas 4
#define Horizon 907200
#define OSContactPeriod 1200
#define OSContactDuration 120
#define GnavContactPeriod 10800
#define GnavContactDuration 100

using namespace std;

class Satellite
{
private:
    int nbContacts;
    
public:
    
//    vector <int> ContactUser;
//    vector <int> ContactNumber;         // number of the contact for the user User
//    vector < pair<long,long> > ContactPeriod;

    int Num;
    vector <ContactSat> tabContactSat;
    
    vector <long> tabStartTimeOS;         // Start time of OS, L-PRS  A-PRS  AND Gnav messages
    vector <long> tabEndTimeOS;         // End  time of OS, L-PRS and A-PRS AND Gnav messages
    
    vector <long> tabStartTimeGnav;         // not used anymore
    
    //vector<long> MergedContactStart;
    //vector<long> MergedContactEnd;
    //vector < pair<long,long> > MergedContactPeriod;
    vector <ContactSat> tabMergedContactSat;
    vector<vector<int> > CorrespondingContact;    // CorrespondingContacts[i] contains the index in tabContactSat of the merged contact of tabMergedContactSat[i]
    
    //Constructor
    Satellite(int i, int nbSat);
    //Satellite();
   
    //User(int a);
    
    
    
    
    
    //Methods

    int GetnbContacts(){return nbContacts;}
    void SortContact();
    void Merge(int n);
    //void AddContact(int u, int num, long s , long e);
    void AddContact(int u, int num, int numP, long s , long e);
    bool VisibleInLocation ( ContactSat c,CAntennaSite l);
    void DisplaytabContact();
    void DisplaytabMergedContact();
    void Decompose(vector <CAntennaSite> tabS);
    void BuildContact(int );
    void AdjustOSContact(vector <CAntennaSite> tabS);
}
;



#endif /* Satellite_hpp */
