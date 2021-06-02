//
//  ContactSat.hpp
//  
//
//  Created by Laurent Houssin on 22/10/2019.
// contact sat -> User

#ifndef ContactSat_h
#define ContactSat_h

#include <stdio.h>
#include <iostream>
#include <utility>


using namespace std;

class ContactSat
{
private:

  
    
public:
    
    int User;
    int NumContact;
    int NumContactInPath;
    pair<long,long> ContactPeriod;
    
    //Constructor
    ContactSat(int u, int nc, int ncip, long s, long e);
    
    
    
    
    //Methods
    int GetUser() {return User;};
    int GetNumContact() {return NumContact;};
    int GetNumContactInPath() {return NumContactInPath;};
    long GetStartTime(){return ContactPeriod.first;}
    long GetEndTime(){return ContactPeriod.second;}
    void Display();
    int IsIncludedIn(long , long  );

}
;


#endif /* ContactSat_h */
