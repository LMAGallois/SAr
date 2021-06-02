//
//  Satellite.cpp
//  
//
//  Created by Laurent Houssin on 15/10/2019.
//

#include "Satellite.h"



//Constructor

Satellite::Satellite(int id, int nbSat)
{
    /////////////////////////////// In Sattellite.cpp and read_data.cpp (2x)
     //Nominal scenario
        int myints[] ={2,4,6,10,12,14,18,20,22,26,28,30,34,36,38,42,44,46};
//        //  sub Nominal scenario
//        int myints[] ={0,2,4,6,7,8,10,12,14,16,17,18,19,20,22,24,26,28,30,32,34,36,38,40,42,44,46};
//     //Extended scenario
 //   int myints[] ={2,6,10,14,18,22,26,30,34,38,42,46};
    vector <int> ForbiddenSat(myints,myints+ sizeof(myints) / sizeof(int));
    ///////////////////////////////
    
    
    
    nbContacts=0;
    Num=id;
    
    if(find(ForbiddenSat.begin(),ForbiddenSat.end(),id) == ForbiddenSat.end())
    {
        
        cout << "sat : "<< id <<endl;
        
        int nbc=Horizon/OSContactPeriod  ;
        int shift = OSContactPeriod/nbSat;
        
        //    cout << "nbc  " << nbc << endl;
        
        for(int j=0; j<nbc;j++)
        {
            tabStartTimeOS.push_back(shift*id + j*OSContactPeriod);

            tabEndTimeOS.push_back(shift*id + j*OSContactPeriod+OSContactDuration);

            if(j%24==0)
            {
                tabEndTimeOS.back()=tabEndTimeOS.back()+GnavContactDuration;
            }


            if(j%9==0)
            {
                tabEndTimeOS.back()=tabEndTimeOS.back()+GnavContactDuration;
            }
//
//            if(j%2==0)
//            {
//
//
//                tabStartTimeOS.push_back(shift*id + j*OSContactPeriod);
//
//                tabEndTimeOS.push_back(shift*id + j*OSContactPeriod+OSContactDuration);
//
//                if(j%24==0)
//                {
//                    tabEndTimeOS.back()=tabEndTimeOS.back()+GnavContactDuration;
//                }
//
//
//                if(j%9==0)
//                {
//                    tabEndTimeOS.back()=tabEndTimeOS.back()+GnavContactDuration;
//                }
//            }
//            else
//            {
//                tabStartTimeOS.push_back((1200-shift*id) + j*OSContactPeriod);
//
//                tabEndTimeOS.push_back(shift*id + j*OSContactPeriod+OSContactDuration);
//
//                if(j%24==0)
//                {
//                    tabEndTimeOS.back()=tabEndTimeOS.back()+GnavContactDuration;
//                }
//
//
//                if(j%9==0)
//                {
//                    tabEndTimeOS.back()=tabEndTimeOS.back()+GnavContactDuration;
//                }
//            }
//

            
            
            
            
        }
        
        //
        //
        //        for(int j=0; j<tabStartTimeGnav.size();j++)
        //        {
        //
        //
        //                cout << "sat  " <<Num << "contact  " << j << " Gnav :  " << tabStartTimeGnav[j] <<endl;
        //
        //
        //
        //        }
        
        //
        //        nbc=Horizon/GnavContactPeriod  ;
        //        shift = GnavContactPeriod/nbSat;
        //
        //        cout << "shift  Gnav " << shift << endl;
        //
        //        for(int j=0; j<nbc;j++)
        //        {
        //            tabStartTimeGnav.push_back(shift*id + j*GnavContactPeriod+OSContactDuration);
        //        }
        //
        // check overlapp
        
        long a,b;
        
        for(int l=0; l<tabStartTimeOS.size();l++)
        {
            
            for(int j=0; j<tabStartTimeGnav.size();j++)
            {
                a= max(tabStartTimeOS[l]+OSContactDuration,tabStartTimeGnav[j]+GnavContactDuration);
                b= min(tabStartTimeOS[l],tabStartTimeGnav[j]);
                if(OSContactDuration+GnavContactDuration > a-b )
                {
                    cout << "sat : " << Num <<endl;
                    cout << "overlapp " <<endl;
                    cout << l << " OS  :  " << tabStartTimeOS[l] <<endl;
                    cout << j << " Gnav :  " << tabStartTimeGnav[j] <<endl;
                    cout <<"press key "<<endl;
                    cin.get();
                    
                }
                
            }
            
        }
        
        
        
        
        
        
    }
}

//Satellite::Satellite()
//{
//    nbContacts=0;
//    Num=-1;
//}


//Methods






void Satellite::AddContact(int u, int num, int numP, long s , long e)
{
    
    tabContactSat.push_back(ContactSat (u,num, numP, s,e));
    nbContacts++;
    
}




bool myfunction (ContactSat i,ContactSat j)
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


void Satellite::SortContact()
{
    sort(tabContactSat.begin(),tabContactSat.end(),myfunction);
    sort(tabMergedContactSat.begin(),tabMergedContactSat.end(),myfunction);
}


void Satellite::Merge(int n)         // n = 0 merge identical slot (can come from different user)
{                                    // n>=1 merge slot not different from n*1000 sec
    // fill tabMergedContactSat
    SortContact();
    vector <int> v;
    
    int w=0;
    
    
    if(n>=0)
    {
        vector <ContactSat> tabCSnew;
        
        for (int i = 0; i < tabContactSat.size(); i++)
        {
            w=0;
            for (int j = i+1; j < tabContactSat.size(); j++)
            {
                if (tabContactSat[i].GetStartTime()==tabContactSat[j].GetStartTime() and tabContactSat[i].GetEndTime()==tabContactSat[j].GetEndTime())
                {
                    //cout << tabContactSat[i].GetUser() << " " <<                tabContactSat[j].GetUser() << " "  << tabContactSat[i].GetStartTime() << " "  << tabContactSat[j].GetStartTime() << " " <<  tabContactSat[i].GetEndTime() << " " << tabContactSat[j].GetEndTime()<< endl;
                    
                    w=1;
                    
                }
                
            }
            if(w==0)
            {
                tabCSnew.push_back(ContactSat(tabContactSat[i].User, tabContactSat[i].NumContact,tabContactSat[i].NumContactInPath, tabContactSat[i].ContactPeriod.first,tabContactSat[i].ContactPeriod.second));
                
                v.push_back(i);
                CorrespondingContact.push_back(v);
                v.clear();
            }
            else
            {
                v.push_back(i);
            }
            
            
            
        }
        
        tabMergedContactSat=tabCSnew;
        tabCSnew.clear();
        
        
        
        if(n>=1)            // extended merge
        {
            //vector <ContactSat> tabCSnew;
            
            int a=0;
            vector <long> vs,ve;
            long s,e;
            
            for (int i = 0; i < tabMergedContactSat.size(); i++)
            {
                w=0;
                for (int j = i+1; j < tabMergedContactSat.size(); j++)
                {
                    
                    
                    
                    if (abs(tabMergedContactSat[i].GetStartTime()-tabMergedContactSat[j].GetStartTime()) + abs(tabMergedContactSat[i].GetEndTime()- tabMergedContactSat[j].GetEndTime())<=1000*n)
                    {
                        //cout << tabContactSat[i].GetUser() << " " <<                tabContactSat[j].GetUser() << " "  << tabContactSat[i].GetStartTime() << " "  << tabContactSat[j].GetStartTime() << " " <<  tabContactSat[i].GetEndTime() << " " << tabContactSat[j].GetEndTime()<< endl;
                        
                        w=1;
                        //cin.get();
                        
                    }
                    
                }
                if(w==0)
                {
                    vs.push_back(tabMergedContactSat[i].GetStartTime());
                    ve.push_back(tabMergedContactSat[i].GetEndTime());
                    s=*min_element(vs.begin(), vs.end());
                    e=*max_element(ve.begin(), ve.end());
                    
                    tabCSnew.push_back(ContactSat(tabMergedContactSat[i].User, tabMergedContactSat[i].NumContact,tabMergedContactSat[i].NumContactInPath, s,e));
                    //
                    v.insert( v.end(), CorrespondingContact[i].begin(), CorrespondingContact[i].end() );
                    CorrespondingContact[a]=v;
                    v.clear();
                    vs.clear();
                    ve.clear();
                    a++;
                }
                else
                {
                    v.insert( v.end(), CorrespondingContact[i].begin(), CorrespondingContact[i].end() );
                    vs.push_back(tabMergedContactSat[i].GetStartTime());
                    ve.push_back(tabMergedContactSat[i].GetEndTime());
                }
                
                
                
            }
            
            CorrespondingContact.erase(CorrespondingContact.begin()+tabCSnew.size(),CorrespondingContact.end());
            tabMergedContactSat=tabCSnew;
            tabCSnew.clear();
            
            
        }
        
    }
    
    //    cout <<"nb  contact " << tabContactSat.size();
    //    cin.get();
    
}

bool Satellite::VisibleInLocation ( ContactSat c,CAntennaSite l)
{
    int visi=0;
    
    //cout << "c.start " << c.GetStartTime()  << " " << "c.end " << c.GetEndTime() <<endl;
    
    for (int i=0; i<l.GetnbContacts() ; i++)
    {
        //cout << " sat " << l.GetSatContact(i) << " " << Num  <<endl;
        
        if(l.GetSatContact(i)==Num)
        {
            
            //cout << "GetStartContact " << l.GetStartContact(i)  << " " << "GetEndContact " << l.GetEndContact(i) <<endl;
            if(l.GetStartContact(i) <= c.GetStartTime()    and c.GetEndTime()<= l.GetEndContact(i))
            {
                visi=1;
                return(visi);
            }
        }
    }
    
    return (visi);
}


void Satellite::DisplaytabContact()
{
    SortContact();
    cout << "Load of satellite : " << Num  << "  number of user contact : "<< tabContactSat.size() <<endl;
    cout << "User" << "\t" << "NumContact"  << "\t" << "NumContactInPath" << "\t" << "Start" << "\t" << "End" << endl;
    for (int i = 0; i < tabContactSat.size(); i++)
    {
        cout<< i << "  ";
        
        tabContactSat[i].Display();
        cout <<endl;
    }
    
}

void Satellite::DisplaytabMergedContact()
{
    
    SortContact();
    cout << "Load of satellite : " << Num  << "  number of user contact : "<< tabMergedContactSat.size() <<endl;
    cout << "User" << "\t" << "NumContact"  << "\t" << "NumContactInPath" << "\t" << "Start" << "\t" << "End" << "\t    weight " <<endl;
    for (int i = 0; i < tabMergedContactSat.size(); i++)
    {
        cout<< i << " ";
        
        tabMergedContactSat[i].Display();
        cout << "\t" << CorrespondingContact[i].size();
        cout <<endl;
    }
    
    ///////
    cout << "CorrespondingContact " <<endl;
    
    for (int i = 0; i < CorrespondingContact.size(); i++)
    {
        cout<< i <<" : [ " ;
        
        for (int j = 0; j < CorrespondingContact[i].size(); j++)
        {
            cout << CorrespondingContact[i][j] << " " ;
        }
        cout << "] " <<endl;
    }
}



void Satellite::Decompose(vector <CAntennaSite> tabS)
{
    
    vector <ContactSat > tabCnew;
    int n=tabContactSat.size();
    
    int s;
    
    for(int l=0; l<tabS.size(); l++)
    {
        n=tabContactSat.size();
        for(int i=0; i<n; i++)
        {
            //cout <<" indice "<<  i  <<" "<< tabContactSat[i].GetStartTime()<<" "<< tabContactSat[i].GetEndTime() <<endl;
            s=0;
            
            for (int j=0; j<tabS[l].GetnbContacts() ; j++)
            {
                
                
                
                if(Num==tabS[l].GetSatContact(j))
                {
                    
                    
                    if (tabS[l].GetStartContact(j)<= tabContactSat[i].GetStartTime() and  tabContactSat[i].GetEndTime() <= tabS[l].GetEndContact(j) ) // case 1
                    {
                        
                        //cout << "\t" << GetStartContact(i) << " " <<  GetEndContact(i) <<endl;
                        //cout << tabS[l].GetStartContact(j) << "\t\t " << tabS[l].GetEndContact(j) <<endl;
                        
                        tabCnew.push_back(ContactSat(-1,0,-1,tabContactSat[i].GetStartTime(),tabContactSat[i].GetEndTime() ));
                        s++;
                        //                        cout << "1 \t" << GetStartContact(i) << " " <<  GetEndContact(i) <<endl;
                        //                        cout << tabS[l].GetStartContact(j) << "\t\t " << tabS[l].GetEndContact(j) <<endl;
                        //                        cin.get();
                    }
                    
                    //////
                    
                    if (tabS[l].GetEndContact(j) > tabContactSat[i].GetStartTime() and  tabContactSat[i].GetEndTime() > tabS[l].GetEndContact(j)
                        and tabS[l].GetStartContact(j)< tabContactSat[i].GetStartTime() ) // case 2
                    {
                        tabCnew.push_back(ContactSat(-1,0,-1,tabContactSat[i].GetStartTime(),tabS[l].GetEndContact(j) ));
                        tabCnew.push_back(ContactSat(-1,0,-1,tabS[l].GetEndContact(j),tabContactSat[i].GetEndTime() ));
                        
                        //                        cout << "2 \t" << GetStartContact(i) << " " <<  GetEndContact(i) <<endl;
                        //                        cout << tabS[l].GetStartContact(j) << "\t\t " << tabS[l].GetEndContact(j) <<endl;
                        //                        cin.get();
                        s++;
                    }
                    
                    //////
                    
                    if (tabS[l].GetStartContact(j) > tabContactSat[i].GetStartTime() and tabS[l].GetStartContact(j) < tabContactSat[i].GetEndTime()
                        and tabContactSat[i].GetEndTime() < tabS[l].GetEndContact(j) ) // case 3
                    {
                        tabCnew.push_back(ContactSat(-1,0,-1,tabContactSat[i].GetStartTime(),tabS[l].GetStartContact(j) ));
                        tabCnew.push_back(ContactSat(-1,0,-1,tabS[l].GetStartContact(j),tabContactSat[i].GetEndTime()));
                        
                        //                        cout << "3 \t" << GetStartContact(i) << " " <<  GetEndContact(i) <<endl;
                        //                        cout << tabS[l].GetStartContact(j) << "\t\t " << tabS[l].GetEndContact(j) <<endl;
                        //                        cin.get();
                        s++;
                    }
                    
                    if (tabS[l].GetStartContact(j) > tabContactSat[i].GetStartTime() and  tabContactSat[i].GetEndTime() > tabS[l].GetEndContact(j) ) // case 4
                    {
                        tabCnew.push_back(ContactSat(-1,0,-1,tabContactSat[i].GetStartTime(),tabS[l].GetStartContact(j) ));
                        tabCnew.push_back(ContactSat(-1,0,-1,tabS[l].GetStartContact(j),tabS[l].GetEndContact(j) ));
                        tabCnew.push_back(ContactSat(-1,0,-1,tabS[l].GetEndContact(j) ,tabContactSat[i].GetEndTime()));
                        
                        //                        cout << "4 \t" << GetStartContact(i) << " " <<  GetEndContact(i) <<endl;
                        //                        cout << tabS[l].GetStartContact(j) << "\t\t " << tabS[l].GetEndContact(j) <<endl;
                        //                        cin.get();
                        s++;
                        
                    }
                    
                    
                }
                
            }
            
            if(s!=1)    // case 5
            {
                //cout <<"satellite " << Num  <<" non visible contact for location " << l <<endl;
                //                cout << " " << GetStartContact(i) << " " <<  GetEndContact(i) <<endl;
                //cout << tabS[l].GetStartContact(j) << "\t\t " << tabS[l].GetEndContact(j) <<endl;
                //cin.get();
                if(tabContactSat[i].GetStartTime()<Horizon)
                {
                    tabCnew.push_back(ContactSat(-1,0,-1,tabContactSat[i].GetStartTime(),tabContactSat[i].GetEndTime() ));
                }
                
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
        tabContactSat=tabCnew;
        nbContacts=tabCnew.size();
        tabCnew.clear();
        
    }
    
    SortContact();
}

void Satellite::BuildContact(int nbSat)
{
    int nbPRScontact=Horizon/OSContactPeriod;       //756
    int shift = OSContactPeriod/nbSat;              //25
    
    //int i=1;
    //cout <<"sat  " << i <<endl ;
    if(Num!=0 and !tabStartTimeOS.empty()) AddContact(-1, 0,-1, 0, shift*Num);
    
    for (int j=0; j<tabStartTimeOS.size(); j++)
    {
        if( !tabStartTimeOS.empty() and  tabStartTimeOS[j] <Horizon)
        {
            //cout << "j  " << j <<endl;
            if (j<tabStartTimeOS.size()-1)
                
            {
                AddContact(-1, 0,-1, tabEndTimeOS[j] , tabStartTimeOS[j+1]);
                
            }else
            {
                AddContact(-1, 0,-1, tabEndTimeOS[j] ,Horizon);
                
                
                //cout << "tabSatellite[i].tabStartTimeOS[j+1] "<< j <<" "  << tabSatellite[i].tabStartTimeOS[nbPRScontact-1] <<endl;
            }
            
        }
        
    }
}

void Satellite::AdjustOSContact(vector <CAntennaSite> tabS)
{
    int test;
    int l;
    int numcont;
    long maxEnd;
    long Dev;
    int ContactToMove=1;
    
    while(ContactToMove)
    {
        ContactToMove=0;
        for(int j=0; j< tabStartTimeOS.size(); j++)
        {
            test=0;
            for(int k=0; k<nbSites; k++)
            {
                if (VisibleInLocation( ContactSat(-1,-1,-1,tabStartTimeOS[j],tabEndTimeOS[j]),tabS[k]) ==0)
                {
                    //cout << "non visible : sat " << s << " num contact " <<j<< " site " << k<<  " start " << tabSat[s].tabStartTimeOS[j] << " end "  << tabSat[s].tabStartTimeOS[j]+OSContactDuration <<endl;
                    
                    test++;
                }
                
            }
            
            if(test ==5)
            {
                cout << "non visible OSContact : sat " << Num << " num contact " <<j<<  " start " << tabStartTimeOS[j] << " end "  << tabEndTimeOS[j] <<endl;
                //OSContactNonServed++;
                //cin.get();
                
                ContactToMove=1;
                maxEnd =0;
                
                for(int k=0; k<nbSites; k++)
                {
                    for (int i=0; i<tabS[k].GetnbContacts() ; i++)
                    {
                        //cout << " sat " << l.GetSatContact(i) << " " << Num  <<endl;
                        
                        if(tabS[k].GetSatContact(i)==Num)
                        {
                            
                            if( tabS[k].GetEndContact(i) <= tabEndTimeOS[j]  and   maxEnd < tabS[k].GetEndContact(i) )
                            {//and   maxEnd < tabS[k].GetEndContact(i)
                                
                                maxEnd = max(maxEnd, tabS[k].GetEndContact(i));
                                l=k;
                                numcont= i;
                                
                                //cout << "GetStartContact Location " << k << " " << tabS[k].GetStartContact(i)  << " " << "GetEndContact  " << tabS[k].GetEndContact(i) << " maxend " << maxEnd << endl;
                                
                            }
                        }
                    }
                }
                
                //cout <<  " maxend " << maxEnd <<  " l " << l <<  " numcont " << numcont << endl;
                
                Dev=tabEndTimeOS[j]  - maxEnd;
                
                cout <<  " Dev " << Dev <<endl;
                
                if(tabEndTimeOS[j-1] <= tabStartTimeOS[j]-Dev)
                {
                    
                    for (int i=j; i< tabStartTimeOS.size() ; i++)
                    {
                        
                        tabStartTimeOS[i]=tabStartTimeOS[i]-Dev;
                        tabEndTimeOS[i]=tabEndTimeOS[i]-Dev;
                    }
                    
                    cout << "Move to visible slot  : ----->" << " num contact " <<j<<  " start " << tabStartTimeOS[j] << " end "  << tabEndTimeOS[j] <<endl;
                }
                else
                {
                    cout <<"Overlap on OS Contact "<<endl;
                    cout <<"press key "<<endl;
                    cin.get();
                }
                
                
                
            }
            
            
        }
    }
    
}

