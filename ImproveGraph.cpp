//
//  ImproveGraph.cpp
//  
//
//  Created by Laurent Houssin on 01/11/2019.
//

#include "ImproveGraph.h"


using namespace std;


void ImproveSat(vector <vector <int> >  vX , vector <Satellite>  &tabSat , vector <User> &tabU )
{
    
    for(int s=0; s<tabSat.size(); s++)
    {
        for (int j=0; j< tabSat[s].tabMergedContactSat.size();j++)
        {
            if(vX[s][j]==0)
            {
                //cout << "s j non served : " << s <<" "<< j <<endl;
                //tabSat[s].tabMergedContactSat[j]
                for (int i=0; i< tabSat[s].CorrespondingContact[j].size();i++)
                {
//                    tabSat[s].tabContactSat[tabSat[s].CorrespondingContact[j][i]].Display();
//                    cout << endl;
                    ImproveContactSat(tabSat[s].tabContactSat[tabSat[s].CorrespondingContact[j][i]], tabU);
                    
                }
            }
        }
    }
}

void ImproveContactSat(ContactSat cs , vector <User> &tabU )
{
//    cs.Display();
//    cout << endl;

    tabU[cs.GetUser()].IsolateContactGraph(cs.GetNumContact());
}

