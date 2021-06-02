//
//  CAntennaSite.h
//  
//
//  Created by Laurent Houssin on 07/10/2019.
//
// ScheduleCAntenna contains the solution of the global problem

#ifndef CAntennaSite_h
#define CAntennaSite_h

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "Contact.h"


#define nbSites 5
#define nbAntennas 4

using namespace std;


class CAntennaSite
{
private:
    string Location;
    int nbContacts;
    vector <Contact> tabContact;
    
    
    
public:
    
    vector <Contact> ScheduleCAntenna[nbAntennas];        // 4 antennas/site
    
    //Constructor
    CAntennaSite(string l);
    CAntennaSite();

    
    
    
    //Methods
    
    int GetnbContacts(){return nbContacts;}
    void AddContact(int sat, long s, long e);
    int GetSatContact(int i);
    long GetStartContact(int i);
    long GetEndContact(int i);
    string GetLocation() {return Location;}

    void SortSchedule();
    void Display();
    void DisplaySchedule();

    void AssignAntenna(int c);
    
    void OutputFormat();
    
}
;

#endif /* CAntennaSite_hpp */
