//
//  ContactSat.cpp
//  
//
//  Created by Laurent Houssin on 22/10/2019.
//

#include "ContactSat.h"


//Constructor
ContactSat::ContactSat(int u, int nc, int ncip, long s, long e)
{
    User=u;
    NumContact=nc;
    NumContactInPath=ncip;
    ContactPeriod.first=s;
    ContactPeriod.second=e;
}


//Methods

void ContactSat::Display()
{
    
    cout << GetUser() << "\t" << GetNumContact() << "\t\t\t" << GetNumContactInPath() << "\t\t" << GetStartTime() << "\t" << GetEndTime() ;
    
    
}

int ContactSat::IsIncludedIn(long s, long e)
{
    return(s <= ContactPeriod.first and  ContactPeriod.second <= e);
}
