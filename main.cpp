//
//  main.cpp
//  
//
//  Created by Laurent Houssin on 17/09/2019.
//

#include <iostream>
#include <string>
#include "User.h"
#include "Contact.h"
#include "read_data.h"
#include "CAntennaSite.h"
#include "Satellite.h"
#include "MIP.h"
#include "ImproveGraph.h"

#define nbSites 5
#define nbAntennas 4
#define Horizon 907200


using namespace std;


int main()
{
    
    vector <User> tabUser;
    int nbSat;
    int nbUsers;
    vector <CAntennaSite> tabSite;
    vector  <vector <int> > X;
    
    nbSat=read_data_User(tabUser);
    read_data_Antenna(tabSite);

    tabUser[1399].DisplayContact(1);
    
    int totalnbContact=0;
    
    vector <Satellite>   tabSatellite;
    
    ///////////////////////////// Satellite Creation ////////////////
    
    for (int i=0; i<nbSat; i++)
    {
        tabSatellite.push_back(Satellite(i,nbSat));
    }
    
    //    tabSatellite[40].AdjustOSContact(tabSite);
    
//
//        for(int j=0; j<tabSatellite[40].tabStartTimeOS.size();j++)
//        {
//            cout << "contact  " << j << " OS :  " << tabSatellite[40].tabStartTimeOS[j] << " " << tabSatellite[40].tabEndTimeOS[j]  <<  " " << tabSatellite[40].tabEndTimeOS[j]  - tabSatellite[40].tabStartTimeOS[j]  <<endl;
//        }
    
    
    
    ///////////////////////////// contact creation (except OS and Gnav message) ////////////////
    
    //contact creation (except OS and Gnav message)
    
    for (int i=0; i<nbSat; i++)
    {
        tabSatellite[i].BuildContact(nbSat);
        
    }
    
    
    
    //tabSatellite[1].DisplaytabContact();
    
    
    //////// faire un check de tout ça
    
    
    cout << "start decomposition " <<endl;
    for (int i=0; i<nbSat; i++)
    {
        tabSatellite[i].Decompose(tabSite);
    }
    
    cout << "end decomposition " <<endl;
    
    //tabSatellite[1].DisplaytabContact();
    
    
    
        for (int i=0; i<nbSat; i++)
        {
            cout << tabSatellite[i].tabContactSat.size()<<endl;
        }
    
    
    
    //////////////////// number of user in a contact counting ////////
    
    
    for (int u=0; u<tabUser.size(); u++)
    {
        
        //cout << "u " <<u <<endl;
        for(int i=0; i< tabUser[u].tabContact.size() ; i++ )
        {
            for (int s=0; s<nbSat; s++)
            {
                //cout << s <<endl;
                if (s==tabUser[u].tabContact[i].GetSat())
                {
                    //cout << "cou" <<endl;
                    for (int j=0; j<tabSatellite[s].tabContactSat.size(); j++)
                    {
                        if(tabSatellite[s].tabContactSat[j].IsIncludedIn(tabUser[u].tabContact[i].GetStartTime(),tabUser[u].tabContact[i].GetEndTime()))
                        {
                            tabSatellite[s].tabContactSat[j].NumContact++;
                        }
                        
                    }
                }
                
            }
        }
        
    }
    
    //tabSatellite[1].DisplaytabContact();
    
    
    //////////////////////////  Solve MIP ////////////////
    
//    X.resize(nbSat);
//    for (int s = 0; s < nbSat; s++)
//    {
//        X[s].resize(tabSatellite[s].tabContactSat.size());
//    }
//
//    solveMIP(tabSatellite ,tabSite, X);
    
    
    //////////////////////// display results ////////////////
    
    //    cout << "press key" <<endl;
    //    cin.get();
    //
    //    tabSite[0].DisplaySchedule();
    //        cout << "------------------" <<endl;
    //    tabSite[0].AssignAntenna(1);
    //
    //    tabSite[0].DisplaySchedule();
    //    tabSite[0].OutputFormat();
    //
    
    
//
//    for(int i=0 ; i<nbSites;i++)
//    {
//        //cout <<"site : " << i <<endl;
//        tabSite[i].SortSchedule();
//        //tabSite[i].DisplaySchedule();
//        tabSite[i].AssignAntenna(1);
//    }
//
//
//
//    for(int i=0 ; i<nbSites;i++)
//    {
//        //tabSite[i].OutputFormat();
//        //cin.get();
//
//        tabSite[i].DisplaySchedule();
//
//    }
//
}
