//
//  Contact.h
//  
//
//  Created by Laurent Houssin on 17/09/2019.
//

#ifndef Contact_h
#define Contact_h

#include <stdio.h>



class Contact
{
private:
 
    
public:
    
    int Satellite;
    long Start;
    long End;
    
    //Constructor
    Contact(int sat, long s, long e);
    
    
    
    
    //Methods
    int GetSat() {return Satellite;};
    long GetStartTime(){return Start;}
    long GetEndTime(){return End;}
    
}
;

#endif /* Contact_h */
