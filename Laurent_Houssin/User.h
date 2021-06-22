//
//  User.h
//  
//
//  Created by Laurent Houssin on 16/09/2019.
//

#ifndef User_h
#define User_h

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <list>
#include "Contact.h"
#include "CAntennaSite.h"
#include "Satellite.h"
#include "ContactSat.h"

using namespace std;






class User
{
private:
    int nbContacts;

   
   
public:
    vector <pair<int,int> > *G ; //Adjacency list

    vector <Contact> tabContact;
    vector <Contact> tabContact2;       // without contact of Path
    
    vector < pair<long,long> > SchedulePeriod;      // concerns Path
    vector <int> ScheduleSat;                       // concerns Path
    vector<int> Path;           // vector path stores the shortest path
    vector <int> PathContactServed;
    long DurationServed;
    
    vector < pair<long,long> > SchedulePeriod2;      // concerns Path2
    vector <int> ScheduleSat2;                       // concerns Path2
    vector<int> Path2;           // vector path stores the shortest path2
    
    //Constructor
    User();
    //User(int a);

    

    
    
    //Methods
    int GetnbContacts(){return nbContacts;}
    void AddContact(int sat, long s, long e);
    long GetStartContact(int i);
    long GetEndContact(int i);
    int GetSatContact(int i);
    void DisplayContact(int i);
    
    void Decompose(vector <CAntennaSite> tabS);
    void BuildGraph();
    void Dijkstra();
    void BFS();
    void MakePath2();
    void DisplayPath(int i);
    
    //int IsServed(int k , vector <CAntennaSite> tabS); // node j in path is served ?

    void IsolateContactGraph(int k);        // k=index of the contact to isolate
    
    bool myfunSort (Contact i,Contact j);
    void SortContact();
    
    void ComputePathContactServed(int ind, vector <vector <int> >  vX , vector <Satellite>  tabSat ) ;
    void DisplayPathServed( )   ;
    
}
;
#endif /* User_h */
