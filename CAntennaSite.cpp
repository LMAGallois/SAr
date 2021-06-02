    //
//  CAntennaSite.cpp
//  
//
//  Created by Laurent Houssin on 07/10/2019.
//

#include "CAntennaSite.h"


//Constructor

CAntennaSite::CAntennaSite(string l)
{
    Location=l;
    nbContacts=0;
    
}

CAntennaSite::CAntennaSite()
{
    nbContacts=0;
}


//Methods



void CAntennaSite::AddContact(int sat ,long s, long e)
{
    tabContact.push_back(Contact (sat, s,e));
    nbContacts++;
}


long CAntennaSite::GetStartContact(int i)
{
    
    return(tabContact[i].GetStartTime());
}

long CAntennaSite::GetEndContact(int i)
{
    return(tabContact[i].GetEndTime());
}

int CAntennaSite::GetSatContact(int i)
{
    return(tabContact[i].GetSat());
}

void CAntennaSite::Display()            // display all the contacts with sat
{
    cout << "Site : " << Location << endl;
    cout << "nbContacts : " << nbContacts << endl;
    for(int i=0; i<nbContacts; i++)
    {
        cout << i << "\t" << "Sat:" << GetSatContact(i) <<"\t" << "start:" << GetStartContact(i) << "\t" <<  "end:" << GetEndContact(i) << endl;
    }
}

void CAntennaSite::DisplaySchedule()            // display all the solution
{
    SortSchedule();
    cout << "-----------------"  << endl;
    cout << "Site : " << Location << endl;
    cout << "-----------------"  << endl;
    
    for(int i=0; i<nbAntennas; i++)
    {
        cout << "Site : " << Location << endl;
        cout << "Anntenna : " << i <<" nbContacts : " << ScheduleCAntenna[i].size() << endl;
        cout <<  "\t" << "Sat " << "\t" << "StartTime " "\t" << "EndTime " <<endl;
        for(int j=0; j<ScheduleCAntenna[i].size(); j++)
        {
            cout << j << "\t"  << ScheduleCAntenna[i][j].GetSat() <<"\t" <<   ScheduleCAntenna[i][j].GetStartTime()  << "\t" <<  ScheduleCAntenna[i][j].GetEndTime()  << endl;
        }
        
        
    }
    
    
}


bool mySort (Contact i,Contact j) { return (i.GetStartTime()<j.GetStartTime()); }

void CAntennaSite::SortSchedule()
{
    for(int i=0; i<nbAntennas; i++)
    {
        sort(ScheduleCAntenna[i].begin(),ScheduleCAntenna[i].end(),mySort);
        
    }
    
}

//void CAntennaSite::AssignAntenna()
//{
//    SortSchedule();
//
//    vector <Contact> S=ScheduleCAntenna[0];
//    ScheduleCAntenna[0].clear();
//
//    vector <long> vs,ve;
//    long s,e;
//    vector <int> v;
//    int w=0;
//    vector<int>::iterator it;
//    long nbCTtotal= ScheduleCAntenna[0].size();
//    vector <long> VecEndTimeAntenna(nbAntennas,0);
//    vector <int> VecEndSatAntenna(nbAntennas,-1);
//
//    for(int i=0; i<nbCTtotal; i++)
//    {
//        it=find(VecEndSatAntenna.begin(), VecEndSatAntenna.end(), S[i].GetSat());
//        if(find(VecEndSatAntenna.begin(), VecEndSatAntenna.end(), S[i].GetSat()) != VecEndSatAntenna.end() )
//        {
//            w = distance(VecEndSatAntenna.begin(), it);
//
//
//
//        }
//
//    }
//
//
//
//}

//
//
//void CAntennaSite::AssignAntenna(int nbSat)
//{
//    SortSchedule();
//    //    vector <Contact> S=ScheduleCAntenna[0];
//    //    ScheduleCAntenna[0].clear();
//    vector <Contact> S;
//    vector <long> vs,ve;
//    long s,e;
//    long CurrentMin,CurrentMax;
//
//    int w=0;
//    int concerned=0;
//
//    long nbCTtotal= ScheduleCAntenna[0].size();
//
//    vector < vector <Contact> > MatSatCont(nbSat);
//    cout << "cucouc assign" <<endl;
//
//    for(int i=0; i<nbCTtotal; i++)
//    {
//        MatSatCont[ScheduleCAntenna[0][i].GetSat()].push_back(ScheduleCAntenna[0][i]);
//    }
//
//
//    for(int s=0; s<nbSat; s++)
//    {
//
//
//
//        for(int i=0; i<MatSatCont[s].size(); i++)
//        {
//            w=0;
//            e=MatSatCont[s][i].GetEndTime();
//            for(int j=i+1; j<MatSatCont[s].size(); j++)
//            {
//                if(MatSatCont[s][i].GetSat()==MatSatCont[s][j].GetSat())
//                {
//                    cout << "sat  " << MatSatCont[s][i].GetSat() << " " <<MatSatCont[s][j].GetSat() <<endl;
//                    if(MatSatCont[s][j].GetStartTime()<=e)
//                    {
//
//                        w=1;
//
//                        cout << i << " " << j  << " "  << MatSatCont[s][i].GetEndTime() << " " << MatSatCont[s][j].GetStartTime() <<endl;
//                        cin.get();
//                    }
//                }
//            }
//
//            if(w==0 )    // last contact with intersection
//            {
//                vs.push_back(MatSatCont[s][i].GetStartTime());
//                ve.push_back(MatSatCont[s][i].GetEndTime());
//                s=*min_element(vs.begin(), vs.end());
//                e=*max_element(ve.begin(), ve.end());
//
//                S.push_back(Contact(MatSatCont[s][i].GetSat(), s,e));
//
//
//                vs.clear();
//                ve.clear();
//
//                cout << "  fdsfdfds :  " << MatSatCont[s][i].GetEndTime()<<endl;
//
//
//            }
//            else
//            {
//
//
//                vs.push_back(MatSatCont[s][i].GetStartTime());
//                ve.push_back(MatSatCont[s][i].GetEndTime());
//                e=*max_element(ve.begin(), ve.end());
//                cout << i << " dans else  "  <<MatSatCont[s][i].GetStartTime() << " " << e<<endl;
//
//            }
//
//        }
//
//    }
//
//    cout << "size " <<ScheduleCAntenna[0].size()<<endl;
//    cout << "size " <<S.size()<<endl;
//    ScheduleCAntenna[0]=S;
//    SortSchedule();
//    cout << "size " <<  ScheduleCAntenna[0].size()<<endl;
//
//
//    S.clear();
//    DisplaySchedule();
//
//
//}
//
//
//
//void CAntennaSite::AssignAntenna()
//{
//    SortSchedule();
//    //    vector <Contact> S=ScheduleCAntenna[0];
//    //    ScheduleCAntenna[0].clear();
//    vector <Contact> S;
//    vector <long> vs,ve;
//    long s,e;
//    long CurrentMin,CurrentMax;
//
//    int w=0;
//    int concerned=0;
//
//    long nbCTtotal= ScheduleCAntenna[0].size();
//
//
//        for(int i=0; i<nbCTtotal; i++)
//        {
//            w=0;
//            e=ScheduleCAntenna[0][i].GetEndTime();
//            for(int j=i+1; j<nbCTtotal; j++)
//            {
//                if(ScheduleCAntenna[0][i].GetSat()==ScheduleCAntenna[0][j].GetSat())
//                {
//
//                    if(ScheduleCAntenna[0][j].GetStartTime()<=e)
//                    {
//
//                        w=1;
//
//                        cout << i << " " << j  << " "  << ScheduleCAntenna[0][i].GetEndTime() << " " << ScheduleCAntenna[0][j].GetStartTime() <<endl;
//                        cin.get();
//                    }
//                }
//            }
//
//            if(w==0 )    // last contact with intersection
//            {
//                vs.push_back(ScheduleCAntenna[0][i].GetStartTime());
//                ve.push_back(ScheduleCAntenna[0][i].GetEndTime());
//                s=*min_element(vs.begin(), vs.end());
//                e=*max_element(ve.begin(), ve.end());
//
//                S.push_back(Contact(ScheduleCAntenna[0][i].GetSat(), s,e));
//
//
//                vs.clear();
//                ve.clear();
//
//                cout << "  fdsfdfds :  " << ScheduleCAntenna[0][i].GetEndTime()<<endl;
//
//
//            }
//            else
//            {
//
//
//                vs.push_back(ScheduleCAntenna[0][i].GetStartTime());
//                ve.push_back(ScheduleCAntenna[0][i].GetEndTime());
//                e=*max_element(ve.begin(), ve.end());
//                cout << i << " dans else  "  << ScheduleCAntenna[0][i].GetStartTime() << " " << e<<endl;
//
//            }
//
//        }
//
//    cout << "size " <<ScheduleCAntenna[0].size()<<endl;
//    cout << "size " <<S.size()<<endl;
//    ScheduleCAntenna[0]=S;
//    SortSchedule();
//    cout << "size " <<  ScheduleCAntenna[0].size()<<endl;
//
//
//    S.clear();
//    DisplaySchedule();
//
//
//}







void CAntennaSite::AssignAntenna(int check)
{
    SortSchedule();
    vector <Contact> S=ScheduleCAntenna[0];
    ScheduleCAntenna[0].clear();
    long nbCTtotal= S.size();
    
    //cout << "nbCTtotal" << nbCTtotal <<endl;
    int a=0;
    
    vector <long> VecEndTimeAntenna(nbAntennas,0);
    vector <int> VecEndSatAntenna(nbAntennas,-1);
    vector<int>::iterator it;
    int minIndex;
    int w;
    
    //int maxElementIndex = std::max_element(v.begin(),v.end()) - v.begin();
    
    for (int i=0; i<nbCTtotal; i++)
    {
        it=find(VecEndSatAntenna.begin(), VecEndSatAntenna.end(), S[i].GetSat());
        
        if(it != VecEndSatAntenna.end() )
        {
            w = distance(VecEndSatAntenna.begin(), it);
            //cout << "w " << w <<endl;
            ScheduleCAntenna[w].push_back(S[i]);
            VecEndTimeAntenna[w]=S[i].GetEndTime();
            VecEndSatAntenna[w]=S[i].GetSat();
        }
        else
        {
            //cout << "S[i].GetSat() " << S[i].GetSat() <<endl;
            
            minIndex = min_element(VecEndTimeAntenna.begin(),VecEndTimeAntenna.end()) - VecEndTimeAntenna.begin();
            
            ScheduleCAntenna[minIndex].push_back(S[i]);
            VecEndTimeAntenna[minIndex]=S[i].GetEndTime();
            VecEndSatAntenna[minIndex]=S[i].GetSat();
            
            //        a++;
            //        a=a%4;
        }
        
    }
    
    //    for (int i=0;i<nbAntennas; i++)
    //    {
    //        reverse(ScheduleCAntenna[i].begin(),ScheduleCAntenna[i].end());
    //
    //    }
    
    
    // check
    if(check==1)
    {
        
        for (int i=0;i<nbAntennas; i++)
        {
            for (int j=0;j<ScheduleCAntenna[i].size()-1; j++)
            {
                if (ScheduleCAntenna[i][j].GetEndTime() > ScheduleCAntenna[i][j+1].GetStartTime())
                {
                    
                    if (ScheduleCAntenna[i][j].GetSat()== ScheduleCAntenna[i][j+1].GetSat())
                    {
                        //cout << " COOL " << endl;
                    }
                    else
                    {
                        cout << " what  !!!" << endl;
                        cout << "sat concerned " << ScheduleCAntenna[i][j].GetSat() << " " << ScheduleCAntenna[i][j+1].GetSat() << " ";
                        cout << ScheduleCAntenna[i][j].GetEndTime() << " " << ScheduleCAntenna[i][j+1].GetStartTime() <<endl;
                        cin.get();
                    }
                    //cin.get();
                    
                }
                
            }
            
        }
    }
    
    
}

void CAntennaSite::OutputFormat()
{
    
    int sat;
    long s,e;
    
    SortSchedule();
    cout << "-----------------"  << endl;
    cout << "Site : " << Location << endl;
    cout << "-----------------"  << endl;
    
    for(int i=0; i<nbAntennas; i++)
    {
        cout << "Site : " << Location << endl;
        cout << "Anntenna : " << i  << endl;
        cout  << "Sat " << "\t" << "StartTime " "\t" << "EndTime " <<endl;
        for(int j=0; j<ScheduleCAntenna[i].size(); j++)
        {
            if(j==0)
            {
                sat=  ScheduleCAntenna[i][j].GetSat();
                s= ScheduleCAntenna[i][j].GetStartTime();
                e= ScheduleCAntenna[i][j].GetEndTime() ;
            }
            else
            {
                if(ScheduleCAntenna[i][j].GetSat() != ScheduleCAntenna[i][j-1].GetSat())
                {
                    cout   << sat <<"\t" <<  s  << "\t" <<  e  << endl;
                    sat=  ScheduleCAntenna[i][j].GetSat();
                    s= ScheduleCAntenna[i][j].GetStartTime();
                    e= ScheduleCAntenna[i][j].GetEndTime() ;
                    
                }
                else
                {
                    e= max(e,ScheduleCAntenna[i][j].GetEndTime()) ;
                    
                }
                
                if(j==ScheduleCAntenna[i].size()-1)
                {
                    cout  << sat <<"\t" <<  s  << "\t" <<  e  << endl;
                    
                }
                
                
            }
            
        }
        
        
    }
    
    
}




