//
//  User.cpp
//  
//
//  Created by Laurent Houssin on 16/09/2019.
//

#include "User.h"

#define Horizon 907200
#define INF INT_MAX //Infinity

//Constructor

User::User()
{
    nbContacts=0;
}

//User::User(int a)
//{
//    nbContacts=a;
//}


//Methods


void User::AddContact(int sat ,long s, long e)
{
    tabContact.push_back(Contact (sat, s,e));
    nbContacts++;
}


long User::GetStartContact(int i)
{
    
    return(tabContact[i].GetStartTime());
}

long User::GetEndContact(int i)
{
    return(tabContact[i].GetEndTime());
}

int User::GetSatContact(int i)
{
    return(tabContact[i].GetSat());
}

void User::DisplayContact(int i)            // display all the contacts
{
    if(i==1)
    {
        
        cout << "nbContacts " << nbContacts << endl;
        for(int i=0; i<nbContacts; i++)
        {
            cout << i << "\t" << "Sat:" << GetSatContact(i) <<"\t" << "start:" << GetStartContact(i) << "\t" <<  "end:" << GetEndContact(i) << endl;
        }
    }
    else
    {
        cout << "nbContacts " << tabContact2.size() << endl;
        for(int i=0; i<tabContact2.size(); i++)
        {
            cout << i << "\t" << "Sat:" << GetSatContact(i) <<"\t" << "start:" << tabContact2[i].Start  << "\t" <<  "end:" << tabContact2[i].End  << endl;
        }
    }
        
}


void User::Decompose(vector <CAntennaSite> tabS)
{
    
    vector <Contact > tabCnew;
    int n=nbContacts;
    
    for(int l=0; l<tabS.size(); l++)
    {
        
        int s;
        
        n=tabContact.size();
        
        
        for(int i=0; i<n; i++)
        {
            //cout <<" indice "<<  i  <<" "<< GetStartContact(i)<<" "<< GetEndContact(i) <<endl;
            s=0;
            
            for (int j=0; j<tabS[l].GetnbContacts() ; j++)
            {
                
                
                
                if(GetSatContact(i)==tabS[l].GetSatContact(j))
                {
                    
                    
                    if (tabS[l].GetStartContact(j)<= GetStartContact(i) and  GetEndContact(i) <= tabS[l].GetEndContact(j) ) // case 1
                    {
                        
                        //cout << "\t" << GetStartContact(i) << " " <<  GetEndContact(i) <<endl;
                        //cout << tabS[l].GetStartContact(j) << "\t\t " << tabS[l].GetEndContact(j) <<endl;
                        
                        tabCnew.push_back(Contact(GetSatContact(i),GetStartContact(i),GetEndContact(i) ));
                        s++;
//                        cout << "1 \t" << GetStartContact(i) << " " <<  GetEndContact(i) <<endl;
//                        cout << tabS[l].GetStartContact(j) << "\t\t " << tabS[l].GetEndContact(j) <<endl;
//                        cin.get();
                    }
                    
                    //////
                    
                    if (tabS[l].GetEndContact(j) > GetStartContact(i) and  GetEndContact(i) > tabS[l].GetEndContact(j)
                        and tabS[l].GetStartContact(j)< GetStartContact(i) ) // case 2
                    {
                        tabCnew.push_back(Contact(GetSatContact(i),GetStartContact(i),tabS[l].GetEndContact(j) ));
                        tabCnew.push_back(Contact(GetSatContact(i),tabS[l].GetEndContact(j),GetEndContact(i) ));
                        
//                        cout << "2 \t" << GetStartContact(i) << " " <<  GetEndContact(i) <<endl;
//                        cout << tabS[l].GetStartContact(j) << "\t\t " << tabS[l].GetEndContact(j) <<endl;
//                        cin.get();
                        s++;
                    }
                    
                    //////

                    if (tabS[l].GetStartContact(j) > GetStartContact(i) and tabS[l].GetStartContact(j) < GetEndContact(i)
                        and GetEndContact(i) < tabS[l].GetEndContact(j) ) // case 3
                    {
                        tabCnew.push_back(Contact(GetSatContact(i),GetStartContact(i),tabS[l].GetStartContact(j) ));
                        tabCnew.push_back(Contact(GetSatContact(i),tabS[l].GetStartContact(j),GetEndContact(i) ));
                        
//                        cout << "3 \t" << GetStartContact(i) << " " <<  GetEndContact(i) <<endl;
//                        cout << tabS[l].GetStartContact(j) << "\t\t " << tabS[l].GetEndContact(j) <<endl;
//                        cin.get();
                        s++;
                    }
                    
                    if (tabS[l].GetStartContact(j) > GetStartContact(i) and  GetEndContact(i) > tabS[l].GetEndContact(j) ) // case 4
                    {
                        tabCnew.push_back(Contact(GetSatContact(i),GetStartContact(i),tabS[l].GetStartContact(j) ));
                        tabCnew.push_back(Contact(GetSatContact(i),tabS[l].GetStartContact(j),tabS[l].GetEndContact(j) ));
                        tabCnew.push_back(Contact(GetSatContact(i),tabS[l].GetEndContact(j) ,GetEndContact(i) ));
                        
//                        cout << "4 \t" << GetStartContact(i) << " " <<  GetEndContact(i) <<endl;
//                        cout << tabS[l].GetStartContact(j) << "\t\t " << tabS[l].GetEndContact(j) <<endl;
//                        cin.get();
                        s++;
                    
                    }
                    

                }
    
            }
            
            if(s!=1)    // case 5
            {
//                cout << s  <<" non visible contact for location " << l <<endl;
//                cout << " " << GetStartContact(i) << " " <<  GetEndContact(i) <<endl;
                //cout << tabS[l].GetStartContact(j) << "\t\t " << tabS[l].GetEndContact(j) <<endl;
                //cin.get();
                tabCnew.push_back(Contact(GetSatContact(i),GetStartContact(i),GetEndContact(i) ));
                
            }
            
        }
        
//        DisplayContact(1);
//        cout << " nbContacts before " << nbContacts << endl;
//        cout << l << " nbContacts " << tabCnew.size() << endl;
//        for(int i=0; i<tabCnew.size(); i++)
//        {
//            cout << i << "\t" << "Sat:" << tabCnew[i].GetSat() <<"\t" << "start:" << tabCnew[i].GetStartTime() << "\t" <<  "end:" << tabCnew[i].GetEndTime() << endl;
//
//        }
//         cin.get();
        tabContact=tabCnew;
        nbContacts=tabCnew.size();
        tabCnew.clear();
        
        
       
    }
    
     SortContact();
//    cout << "sortie" <<endl;
//    DisplayContact(1);
//    cin.get();
}





void User::BuildGraph()         // Build the Interval graph
{
    int nbArcs=0;
    
    //cout << "------------ start building graph ------------" <<endl;
    
    G=new vector < pair<int,int> >[nbContacts+2];
    
    
    for(int i=0; i<nbContacts; i++)
    {
        if(GetStartContact(i)==0)
        {
            G[nbContacts].push_back(make_pair(i,1));
            nbArcs++;
            
           // cout << "\t arc: " << nbContacts << "->" << i <<endl;
        }
        
        for(int j=0; j<nbContacts; j++)
        {
            if(i!=j &&  GetStartContact(i)< GetStartContact(j) && GetStartContact(j)<GetEndContact(i))
            {
                G[i].push_back(make_pair(j,1));
                nbArcs++;
                
              //  cout << "arc: " << i << "->" << j <<endl;
            }
        }
        
        if(GetEndContact(i)==Horizon)
        {
            G[i].push_back(make_pair(nbContacts+1,1));
            nbArcs++;

            
           // cout << "\t arc: " << i << " -> " << nbContacts+1 <<endl;
        }
        
        
    }
    
    //cout << "nbArcs: " << nbArcs<<   "\t nbNodes  " << nbContacts+2  <<  "\t nbArcs/nbNodes  " << nbArcs/(nbContacts+2) <<endl;
    
    //cout << "------------ end building graph ------------" <<endl;
}



void User::Dijkstra()         // find shortest path in the Interval graph and fill the Schedule vectors
{
    int nbC=GetnbContacts();
    int source=nbC;
    
    int dis[nbC+2]; //Stores shortest distance
    bool vis[nbC+2]; //Determines whether the node has been visited or not
    int prev[nbC+2];
    
    for(int i=0;i<GetnbContacts()+2;i++) //Set initial distances to Infinity
    {
        dis[i]=INF;
        vis[i]=0;
    }
    //Custom Comparator for Determining priority for priority queue (shortest edge comes first)
    class prioritize{public: bool operator ()(pair<int, int>&p1 ,pair<int, int>&p2){return p1.second>p2.second;}};
    priority_queue<pair<int,int> ,vector<pair<int,int> >, prioritize> pq; //Priority queue to store vertex,weight pairs
    pq.push(make_pair(source,dis[source]=0)); //Pushing the source with distance from itself as 0
    while(!pq.empty())
    {
        pair<int, int> curr=pq.top(); //Current vertex. The shortest distance for this has been found
        pq.pop();
        int cv=curr.first,cw=curr.second; //'cw' the final shortest distance for this vertex
        if(vis[cv]) //If the vertex is already visited, no point in exploring adjacent vertices
            continue;
        vis[cv]=true;
        for(int i=0;i<G[cv].size();i++) //Iterating through all adjacent vertices
            if(!vis[G[cv][i].first] && G[cv][i].second+cw<dis[G[cv][i].first]) //If this node is not visited and the current parent node distance+distance from there to this node is shorted than the initial distace set to this node, update it
                pq.push(make_pair(G[cv][i].first,(dis[G[cv][i].first]=G[cv][i].second+cw))); //Set the new distance and add to priority queue
    }
    
    
    //    for(int i=0;i<GetnbContacts()+2;i++) //Set initial distances to Infinity
    //    {
    //        cout << "i : " << i << "\t" << dis[i]<<endl;
    //
    //    }
    
}

void User::BFS()
{
    
    
    Path.clear();
    ScheduleSat.clear();
    SchedulePeriod.clear();
    
    int s=nbContacts;
    int V=nbContacts+2;
    // Mark all the vertices as not visited
    bool visited [V];
    int pred[V];
    for(int i = 0; i < V; i++)
    {
        visited[i] = false;
        pred[i] = -1;
    }
    
    // Create a queue for BFS
    list<int> queue;
    
    // Mark the current node as visited and enqueue it
    visited[s] = true;
    pred[s] = s;
    queue.push_back(s);
    
    // 'i' will be used to get all adjacent
    // vertices of a vertex
    //list<int>::iterator i;
    
    while(!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        s = queue.front();
        //cout << s << " ";
        queue.pop_front();
        
        //if (s==6) cout << "what !!!" <<endl;
        
        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        for(int i=0;i<G[s].size();i++)
        {
            if (!visited[G[s][i].first])
            {
                visited[G[s][i].first] = true;
                queue.push_back(G[s][i].first);
                pred[G[s][i].first]=s;
            }
            
            //if(G[s][i].first==nbContacts+1) break;
            
        }
        
    }
    
    
//        for(int i=0;i<V;i++)
//        {
//            cout << "i: " << i << "\t prev: " << pred[i] <<endl;
//        }
//
    

    int j = nbContacts+1;
    
    Path.push_back(pred[j]);
    ScheduleSat.push_back(tabContact[pred[j]].GetSat());
    SchedulePeriod.push_back(make_pair(tabContact[pred[j]].GetStartTime(),Horizon));
    //cout << "x : "  << tabContact[pred[j]].GetStartTime() <<"\t" << Horizon << endl ;
    j = pred[j];
    
    
    while (pred[j] != nbContacts) {
        Path.push_back(pred[j]);
        ScheduleSat.push_back(tabContact[pred[j]].GetSat());
        SchedulePeriod.push_back(make_pair(tabContact[pred[j]].GetStartTime(),tabContact[j].GetStartTime()));
        //cout << "x : "  << tabContact[pred[j]].GetStartTime() <<"\t" << tabContact[j].GetStartTime() << endl ;
        j = pred[j];
    }
    

    reverse(Path.begin(),Path.end());
    reverse(ScheduleSat.begin(),ScheduleSat.end());
    reverse(SchedulePeriod.begin(),SchedulePeriod.end());
    


}



void User::MakePath2()
{
    tabContact2 = tabContact;
    //cout << tabContact.size() << " taille " << tabContact2.size() <<   endl;
    //DisplayContact(2);
    
    for(int i =0; i< Path.size() ; i++)
    {
        tabContact2[Path[i]].Start=SchedulePeriod[i].second;
    }
    //DisplayContact(2);
    
    ///////////////////// Graph Building /////
    
    int nbArcs=0;
    
    //cout << "------------ start building graph ------------" <<endl;
    
    G=new vector < pair<int,int> >[nbContacts+2];
    
    
    for(int i=0; i<nbContacts; i++)
    {
        if(tabContact2[i].Start==0)
        {
            G[nbContacts].push_back(make_pair(i,1));
            nbArcs++;
            
            // cout << "\t arc: " << nbContacts << "->" << i <<endl;
        }
        
        for(int j=0; j<nbContacts; j++)
        {
            if(i!=j &&  tabContact2[i].Start < tabContact2[j].Start && tabContact2[j].Start<tabContact2[i].End && tabContact2[i].End-tabContact2[j].Start !=0 )
            {
                G[i].push_back(make_pair(j,1));
                nbArcs++;
                
                //  cout << "arc: " << i << "->" << j <<endl;
            }
        }
        
        if(tabContact2[i].End==Horizon)
        {
            G[i].push_back(make_pair(nbContacts+1,1));
            nbArcs++;
            
            
            // cout << "\t arc: " << i << " -> " << nbContacts+1 <<endl;
        }
        
        
    }
    
    ///////////////////// End Graph Building /////
    
    ///////////////////// BFS2 /////
    
    

    Path2.clear();
    ScheduleSat2.clear();
    SchedulePeriod2.clear();

    int s=nbContacts;
    int V=nbContacts+2;
    // Mark all the vertices as not visited
    bool visited [V];
    int pred[V];
    for(int i = 0; i < V; i++)
    {
        visited[i] = false;
        pred[i] = -1;
    }

    // Create a queue for BFS
    list<int> queue;

    // Mark the current node as visited and enqueue it
    visited[s] = true;
    pred[s] = s;
    queue.push_back(s);

    // 'i' will be used to get all adjacent
    // vertices of a vertex
    //list<int>::iterator i;

    while(!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        s = queue.front();
        //cout << s << " ";
        queue.pop_front();

        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        for(int i=0;i<G[s].size();i++)
        {
            if (!visited[G[s][i].first])
            {
                visited[G[s][i].first] = true;
                queue.push_back(G[s][i].first);
                pred[G[s][i].first]=s;
            }

            //if(G[s][i].first==nbContacts+1) break;

        }

    }


    //        for(int i=0;i<V;i++)
    //        {
    //            cout << "i: " << i << "\t prev: " << pred[i] <<endl;
    //        }
    //


    int j = nbContacts+1;

    Path2.push_back(pred[j]);
    ScheduleSat2.push_back(tabContact2[pred[j]].GetSat());
    SchedulePeriod2.push_back(make_pair(tabContact2[pred[j]].GetStartTime(),Horizon));
    //cout << "x : "  << tabContact[pred[j]].GetStartTime() <<"\t" << Horizon << endl ;
    j = pred[j];


    while (pred[j] != nbContacts) {
        Path2.push_back(pred[j]);
        ScheduleSat2.push_back(tabContact2[pred[j]].GetSat());
        SchedulePeriod2.push_back(make_pair(tabContact2[pred[j]].GetStartTime(),tabContact2[j].GetStartTime()));
        //cout << "x : "  << tabContact[pred[j]].GetStartTime() <<"\t" << tabContact[j].GetStartTime() << endl ;
        j = pred[j];
    }


    reverse(Path2.begin(),Path2.end());
    reverse(ScheduleSat2.begin(),ScheduleSat2.end());
    reverse(SchedulePeriod2.begin(),SchedulePeriod2.end());
    
    
    
}


void User::DisplayPath(int i)            // display all the contacts of the schedule Path
{
    
    if(i==1)
    {
        
        cout << "nbContacts in Path1 : " << Path.size() << endl;
        
        // printing path from source to destination
        cout << " Path1 is:" <<endl;
        cout << "Contact" << "\t" << "Sat" <<"\t" << "StartTime " << "\t" << "EndTime " << endl ;
        for(int i=0; i<Path.size(); i++)
        {

             cout << Path[i] << "\t" << GetSatContact(Path[i]) <<"\t" << SchedulePeriod[i].first <<"\t\t" << SchedulePeriod[i].second << endl ;
        }
    }else
    {
        cout << "nbContacts in Path2 : " << Path2.size() << endl;
        
        // printing path from source to destination
        cout << " Path2 is:" <<endl;
        cout << "Contact" << "\t" << "Sat" <<"\t" << "StartTime " << "\t" << "EndTime " << endl ;
        for(int i=0; i<Path2.size(); i++)
        {
            
            cout << Path2[i] << "\t" << GetSatContact(Path2[i]) <<"\t" << SchedulePeriod2[i].first <<"\t\t" << SchedulePeriod2[i].second << endl ;
        }
        
    }
        
    
    
}


//int User::IsServed(int k , vector <CAntennaSite> tabS)  // k=index in the Path
//{
//    int served=0;
//    for(int l=0; l<tabS.size(); l++)
//    {
//        for(int i=0; i<nbAntennas; i++)
//        {
//            for(int j=0; j<tabS[l].ScheduleCAntenna[i].size(); j++)
//            {
//                if(tabContact[Path[k]].GetSat()== tabS[l].ScheduleCAntenna[i][j].GetSat())
//                {
//                    if(tabS[l].ScheduleCAntenna[i][j].GetStartTime()  <= SchedulePeriod[k].first and
//                       SchedulePeriod[k].second <= tabS[l].ScheduleCAntenna[i][j].GetEndTime())
//                    {
//                        //cout << "served ! " << " site "<< l << " sat " << tabContact[Path[k]].GetSat() <<   endl;
//                        //cout << SchedulePeriod[k].first   << "  " <<  SchedulePeriod[k].second <<endl;
//                        served=1;
//                        return(1);
//                    }
//
//                }
//            }
//        }
//
//    }
//    return(served);
//
//}


bool m (Contact i,Contact j)
{
    if (i.GetStartTime() != j.GetStartTime() )
    {
    return (i.GetStartTime()<j.GetStartTime());
    }
    else{
        
        if (i.GetEndTime() != j.GetEndTime() )
        {
            return (i.GetEndTime()<j.GetEndTime());
        }
        else return(0);
        
    }
    
    
}




void User::SortContact()
{
    sort(tabContact.begin(),tabContact.end(),m);
    
}



void User::IsolateContactGraph(int k)           // k=index of the contact to isolate
{
    //cout << "contact  " << k << " nb succ  " << G[k].size() <<endl;
    G[k].clear();

   // for (int i=0; i<20; i++) {
      //      G[i].clear();
   // }
}


void User::ComputePathContactServed(int ind, vector <vector <int> >  vX , vector <Satellite>  tabSat )          //
{
    PathContactServed.clear();
    PathContactServed.resize(Path.size(),0);
    DurationServed=0;
    
    for(int s=0; s<tabSat.size(); s++)
    {
        for (int j=0; j< tabSat[s].tabMergedContactSat.size();j++)
        {
            if(vX[s][j]==1) // served
            {
                //cout << "s j non served : " << s <<" "<< j <<endl;
                //tabSat[s].tabMergedContactSat[j]
                for (int i=0; i< tabSat[s].CorrespondingContact[j].size();i++)
                {
          
                    if(tabSat[s].tabContactSat[tabSat[s].CorrespondingContact[j][i]].GetUser()==ind)
                    {
                        //cout << "served " << ind <<endl;
                        PathContactServed[tabSat[s].tabContactSat[tabSat[s].CorrespondingContact[j][i]].GetNumContactInPath()]=1;
                        DurationServed+=tabSat[s].tabContactSat[tabSat[s].CorrespondingContact[j][i]].GetEndTime() -
                        tabSat[s].tabContactSat[tabSat[s].CorrespondingContact[j][i]].GetStartTime();
                        //cout << "DurationServed  " << DurationServed <<endl;
                        //cin.get();
                    }
                    
                }
            }
        }
    }

}



void User::DisplayPathServed( )          //
{
    cout << "nbContacts in Path1 : " << Path.size() << endl;
    
    // printing path from source to destination
    cout << " Path1 is:" <<endl;
    cout << "Contact" << "\t" << "Sat" <<"\t" << "StartTime " << "\t" << "EndTime "<<  "\t" << "Served "  << endl ;
    for(int i=0; i<Path.size(); i++)
    {
        if(PathContactServed[i]==1)
        {
            cout << Path[i] << "\t" << GetSatContact(Path[i]) <<"\t" << SchedulePeriod[i].first <<"\t\t" << SchedulePeriod[i].second << "\t \t x" << endl ;
        }
        else
        {
            cout << Path[i] << "\t" << GetSatContact(Path[i]) <<"\t" << SchedulePeriod[i].first <<"\t\t" << SchedulePeriod[i].second << endl ;
        }

    }
     cout << "Service rate = " << 100.0*DurationServed/Horizon <<endl;
    //100*DurationServed/Horizon
    
}


