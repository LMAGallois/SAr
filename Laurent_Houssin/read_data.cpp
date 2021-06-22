//
//  read_data.cpp
//  
//
//  Created by Laurent Houssin on 17/09/2019.
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "User.h"
#include "CAntennaSite.h"
#include <map>

#define nbUsers 1400

using namespace std;

int read_data_User(vector<User> & tab)
{
///////////////////////////////
    const char* cfile = "data/cov20.cvaa";

    /////////////////////////////// In Sattellite.cpp and read_data.cpp  (2x)
    // Nominal scenario
        int myints[] ={2,4,6,10,12,14,18,20,22,26,28,30,34,36,38,42,44,46};
        //  sub Nominal scenario
      //  int myints[] ={0,2,4,6,7,8,10,12,14,16,17,18,19,20,22,24,26,28,30,32,34,36,38,40,42,44,46};
    // Extended scenario
    //int myints[] ={2,6,10,14,18,22,26,30,34,38,42,46};
    vector <int> ForbiddenSat(myints,myints+ sizeof(myints) / sizeof(int));
    ///////////////////////////////
    
    
    //const char* cfile = "visi/fromGroundPoint20.cvaa";

    string str;
    int found;
    int c;
    int i;

    int nbSat;
    int nbRegion;
    //User u;
    int nbContact;
    int sat;
    float s;
    float e;

    ifstream file(cfile);
    if ( !file ) {
        cerr << "No such file: " << cfile << endl;
        throw(-1);
    }
    
    
    cout << "------ start reading ---------" << endl;



    found=-1;
    while(found==-1)
    {
        file >> str;
        //cout << "ligne " << str << endl;
        found=str.find("NumberOfAssets");
        //cout << found << endl;
    }

    file >> nbSat ;
    nbSat--;
    cout << "nbsat " << nbSat <<endl;

    found=-1;
    while(found==-1)
    {
        file >> str;
        //cout << "ligne " << str << endl;
        found=str.find("NumberOfRegions");
        //cout << found << endl;
    }

    file >> nbRegion ;
    cout << "nbRegion " << nbRegion <<endl;

    ////////////////////////////////////////

    //cout << "---------------" << endl;

    User *u;
    u=new User();

    found=-1;

    c=0;

    while(c<nbUsers)
    {

        found=-1;
        while(found==-1)
        {
            file >> str;
            //cout << "ligne1 " << str << endl;
            found=str.find("PointNumber");
            //cout << found << endl;
        }

        found=-1;
        while(found==-1)
        {
            file >> str;
            //cout << "ligne " << str << endl;
            found=str.find("NumberOfAccesses");
            //cout << found << endl;
        }
        file >> nbContact ;
        //cout << "nbContact " << nbContact <<endl;
        //file >> str;

        for( i=0;i<nbContact ; i++)
        {
            file >> sat;
            file >> s;
            file >> e;
            
            sat--;                      //pb numbering satellite in the file
            
            if(find(ForbiddenSat.begin(),ForbiddenSat.end(),sat) == ForbiddenSat.end())
            {
            u->AddContact(sat,s,e);
            }

            //u->AddContact(sat,s,e);
        }

        //u->Display();

        tab.push_back(*u);
        delete(u);
        u=new User();

        //cout << "user "<< c <<endl;
        c++;



    }// end of while(c<nbUsers)

    cout << "------ end reading ---------" << endl;

    return(nbSat);

}





void read_data_Antenna(vector<CAntennaSite> & tab)
{
    
    //const char* cfile = "data/visiKIR.txt";
    
    /////////////////////////////// In Sattellite.cpp and read_data.cpp (2x)
    // Nominal scenario
        int myints[] ={2,4,6,10,12,14,18,20,22,26,28,30,34,36,38,42,44,46};
    //  sub Nominal scenario
     //   int myints[] ={0,2,4,6,7,8,10,12,14,16,17,18,19,20,22,24,26,28,30,32,34,36,38,40,42,44,46};
    // Extended scenario
    // int myints[] ={2,6,10,14,18,22,26,30,34,38,42,46};
    vector <int> ForbiddenSat(myints,myints+ sizeof(myints) / sizeof(int));
    ///////////////////////////////
    
    
    string str;

    int sat;
    long s;
    long e;
    long d;
    
    const char* cfile;
    
    CAntennaSite *a;

    
    cout << "------ start reading site ---------" << endl;
    
    for (int i=0; i<nbSites;i++)
    {
        
        switch (i) {
            case 0:
                cfile = "data/visiSVA.txt";
                a=new CAntennaSite("Svalbard");
                break;
            case 1:
                cfile = "data/visiKOU.txt";
                a=new CAntennaSite("Kourou");
                break;
            case 2:
                cfile = "data/visiTHT.txt";
                a=new CAntennaSite("Tahiti");
                break;
            case 3:
                cfile = "data/visiREU.txt";
                a=new CAntennaSite("Reunion");
                break;
            case 4:
                cfile = "data/visiNOU.txt";
                a=new CAntennaSite("Noumea");
                break;
                
                
            default:
                break;
        }
        
        
        ifstream file(cfile);
        if ( !file ) {
            cerr << "No such file: " << cfile << endl;
            throw(-1);
        }
        
        
        file.ignore(256,'\n');
        file.ignore(256,'\n');
        file.ignore(256,'\n');
        
        while(file >> sat)
        {
            //cout << "ligne " << sat << endl;
            file >> s;
            //cout << "ligne " << s << endl;
            file >> e;
            //cout << "ligne " << e << endl;
            file >> d;
            
            
            if(find(ForbiddenSat.begin(),ForbiddenSat.end(),sat) == ForbiddenSat.end())
            {
                a->AddContact(sat,s,e);
            }
            
            //a->AddContact(sat,s,e);
            
            
            
        }
        
        tab.push_back(*a);
        delete(a);
        a=new CAntennaSite();
        
    }
    
    
    cout << "------ end reading site ---------" << endl;
    
    
    
}












