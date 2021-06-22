//
//  MIP.cpp
//
//
//  Created by Laurent Houssin on 23/10/2019.
//

#include "MIP.h"


typedef IloArray<IloNumVarArray> NumVarMatrix;
typedef IloArray<NumVarMatrix>   NumVar3Matrix;


ILOSTLBEGIN


int solveMIP(vector <Satellite>  tabSat , vector <CAntennaSite> &tabS, vector <vector <int> >  &vX )
{
    
    
    
    IloEnv env;
    try {
        
        IloModel model(env);
        
        IloInt nbSat=tabSat.size();
        IloInt TotalnbContact=0;
        IloInt TotalnbMergedContact=0;
        
        vector <vector <int > > IntersecVecContact(nbSat);
        vector <vector <int > > IntersecVecOS(nbSat);
        //vector <vector <int > > IntersecVecGnav(nbSat);
        
        int siz=1;
        int r;
        
        
        // Variables ------------------------------------
        
        NumVar3Matrix x(env, nbSat);
        NumVar3Matrix e(env, nbSat);
        NumVar3Matrix f(env, nbSat);
        NumVarMatrix Sum1(env, nbSat);
        IloNumVarArray Sum2(env, nbSat);
        IloNumVar Sum(env);
        /* initialize these matrices */
        for (int s = 0; s < nbSat; s++)
        {
            tabSat[s].tabMergedContactSat = tabSat[s].tabContactSat;
        }
        for (int s = 0; s < nbSat; s++)
        {
            TotalnbMergedContact = TotalnbMergedContact+ tabSat[s].tabMergedContactSat.size();
            TotalnbContact = TotalnbContact+ tabSat[s].tabContactSat.size();
            
            x[s] = NumVarMatrix(env, tabSat[s].tabMergedContactSat.size());
            e[s] = NumVarMatrix(env, tabSat[s].tabStartTimeOS.size());
            //f[s] = NumVarMatrix(env, tabSat[s].tabStartTimeGnav.size());
            Sum1[s] = IloNumVarArray(env, tabSat[s].tabMergedContactSat.size());
            Sum2[s]=IloNumVar(env, 0.0, IloInfinity, ILOINT);
            //cout << "tabSat[s].tabMergedContactSat.size : "<< tabSat[s].tabMergedContactSat.size() <<endl;
            
            
            for(int j=0; j< tabSat[s].tabStartTimeOS.size(); j++)
            {
                
                e[s][j] = IloNumVarArray(env, nbSites, 0, 1, ILOINT);
                
            }
            
            //            for(int j=0; j< tabSat[s].tabStartTimeGnav.size(); j++)
            //            {
            //
            //                f[s][j] = IloNumVarArray(env, nbSites, 0, 1, ILOINT);
            //
            //            }
            
            for(int j=0; j< tabSat[s].tabMergedContactSat.size(); j++)
            {
                
                x[s][j] = IloNumVarArray(env, nbSites, 0, 1, ILOINT);
                Sum1[s][j] = IloNumVar(env, 0.0, IloInfinity, ILOINT);
                
                //                for(int k=0; k<nbSites; k++)
                //                {
                //                    x[s][j][k] = IloNumVar  (env, 0.0, 1.0, ILOINT);
                //                }
            }
            
        }
        
        //cout << "TotalnbContactMerged : " << TotalnbMergedContact <<endl;
        
        
        // Constraints  ------------------------------------
        
        // at most one location for each SatContact
        for (int s = 0; s < nbSat; s++)
        {
            for(int j=0; j< tabSat[s].tabMergedContactSat.size(); j++)
            {
                model.add(IloSum(x[s][j]) <= 1);       // ∑ (k) x[s][j] <= 1
            }
            
        }
        
        ////////////
        // Set to 0 non visible SatContact
        
        int test;
        int Settoz=0;
        
        for (int s = 0; s < nbSat; s++)
        {
            for(int j=0; j< tabSat[s].tabMergedContactSat.size(); j++)
            {
                test=0;
                
                for(int k=0; k<nbSites; k++)
                {
                    
                    if (tabSat[s].VisibleInLocation( tabSat[s].tabMergedContactSat[j],tabS[k]) ==0)
                    {
                        //                        cout << "non visible : " << s << " " <<j<< " " << k<<  " user  " << tabSat[s].tabMergedContactSat[j].GetUser()  << "  num contact " << tabSat[s].tabMergedContactSat[j].GetNumContact() << " start " << tabSat[s].tabMergedContactSat[j].GetStartTime() << " end "  << tabSat[s].tabMergedContactSat[j].GetEndTime() <<endl;
                        
                        model.add(x[s][j][k] ==0);       // if Contact j of Sat s is non visible for site k
                        test++;
                        
                    }
                    if( test ==nbSites)
                    {
                        Settoz++;
                        //                         cout << "non visible : " << s << " " <<j<< " " << k<<  " user  " << tabSat[s].tabMergedContactSat[j].GetUser()  << "  num contact " << tabSat[s].tabMergedContactSat[j].GetNumContact() << " start " << tabSat[s].tabMergedContactSat[j].GetStartTime() << " end "  << tabSat[s].tabMergedContactSat[j].GetEndTime() <<endl;
                        //                        cin.get();
                    }
                }
            }
        }
        
        cout << "Set to zero " << Settoz <<endl;
        
        /////////////////////////////////////////////////
        // Inter SatContact constraints
        
        cout << "IN  Inter SatContact constraints " <<endl;
        
        
        // number of active contact at the start of a standard contact
        for (int s1 = 0; s1 < nbSat; s1++)
        {
            
            cout << "sat " <<s1 << endl;
            for(int j=0; j< tabSat[s1].tabMergedContactSat.size(); j++)
            {
                //cout  << "tabSat[s1].tabMergedContactSat[j].GetStartTime() "<< tabSat[s1].tabMergedContactSat[j].GetStartTime()<< endl;
                IntersecVecContact.clear();
                IntersecVecOS.clear();
                //IntersecVecGnav.clear();
                IntersecVecContact.resize(nbSat);
                IntersecVecOS.resize(nbSat);
                //IntersecVecGnav.resize(nbSat);
                
                for (int s2 = 0; s2 < nbSat; s2++)
                {
                    if(s1!=s2)
                    {
                        for(int m=0; m< tabSat[s2].tabMergedContactSat.size(); m++)
                        {
                            
                            if(((tabSat[s1].tabMergedContactSat[j].GetStartTime() >= tabSat[s2].tabMergedContactSat[m].GetStartTime()) and (tabSat[s2].tabMergedContactSat[m].GetEndTime() >  tabSat[s1].tabMergedContactSat[j].GetStartTime())))
                            {
                                
                                
                                
                                IntersecVecContact[s2].push_back(m);
                                
                            }
                            
                        }
                        
                        ////////////////////////////////////////////////
                        
                        for(int m=0; m< tabSat[s2].tabStartTimeOS.size(); m++)
                        {
                            
                            if(((tabSat[s1].tabMergedContactSat[j].GetStartTime() >= tabSat[s2].tabStartTimeOS[m]) and
                                (tabSat[s2].tabEndTimeOS[m] >  tabSat[s1].tabMergedContactSat[j].GetStartTime())))
                                
                            {
                                //                                cout << " \t s1 j start " << tabSat[s1].tabMergedContactSat[j].GetStartTime() <<endl;
                                //                                cout << "  s2 m start " << tabSat[s2].tabStartTimeOS[m] <<endl;
                                //                                cout << " \t\t s2 m end " << tabSat[s2].tabStartTimeOS[m] +OSContactDuration <<endl;
                                
                                IntersecVecOS[s2].push_back(m);
                                
                            }
                            
                        }
                        
                        ////////////////////////////////////////////////
                        //
                        //                        for(int m=0; m< tabSat[s2].tabStartTimeGnav.size(); m++)
                        //                        {
                        //
                        //                            if(((tabSat[s1].tabMergedContactSat[j].GetStartTime() >= tabSat[s2].tabStartTimeGnav[m]) and
                        //                                (tabSat[s2].tabStartTimeGnav[m] +GnavContactDuration >  tabSat[s1].tabMergedContactSat[j].GetStartTime())))
                        //
                        //                            {
                        ////                                cout << " \t s1 j start " << tabSat[s1].tabMergedContactSat[j].GetStartTime() <<endl;
                        ////                                cout << "  s2 m start " << tabSat[s2].tabStartTimeGnav[m] <<endl;
                        ////                                cout << " \t\t s2 m end " << tabSat[s2].tabStartTimeGnav[m] +GnavContactDuration <<endl;
                        ////
                        //                                IntersecVecGnav[s2].push_back(m);
                        //
                        //                            }
                        //
                        //                        }
                        
                    }
                    //cout <<"IntersecVec[s2].size() " << IntersecVec[s2].size() <<endl;
                    
                    
                }
                
                
                
                for(int k=0; k<nbSites; k++)
                {
                    IloExpr Expr;
                    Expr = IloExpr(env);
                    
                    
                    Expr = x[s1][j][k];
                    
                    //cout << "x[s1][j][k]  " << s1 << " " <<  j << " " << k <<endl;
                    
                    
                    
                    for (int s2 = 0; s2 < nbSat; s2++)
                    {
                        if(s1!=s2 and  IntersecVecContact[s2].empty()!=1 )
                        {
                            Expr += x[s2][IntersecVecContact[s2][0]][k];          // Inter sat contact constraint
                            
                        }
                        if(s1!=s2 and  IntersecVecOS[s2].empty()!=1 )
                        {
                            Expr += e[s2][IntersecVecOS[s2][0]][k];   // Inter sat contact constraint
                            //cout << "e[s2][IntersecVecOS[s2][0]][k] " << s2 << " " <<  IntersecVecOS[s2][0] << " " << k <<endl;
                        }
                        //                        if(s1!=s2 and  IntersecVecGnav[s2].empty()!=1 )
                        //                        {
                        //                            Expr += f[s2][IntersecVecGnav[s2][0]][k];   // Inter sat contact constraint
                        //                            //cout << "f[s2][IntersecVecGnav[s2][0]][k] " << s2 << " " <<  IntersecVecGnav[s2][0] << " " << k <<endl;
                        //                        }
                        
                        
                    }
                    
                    
                    model.add(Expr <= nbAntennas);
                    
                    Expr.end();
                    
                    
                }
                //cin.get();
                
            }
            
        }
        
        // number of active contact at the start of a OS contact
        for (int s1 = 0; s1 < nbSat; s1++)
        {
            
            cout << "sat " <<s1 << endl;
            for(int j=0; j< tabSat[s1].tabStartTimeOS.size(); j++)
            {
                //cout  << "tabSat[s1].tabMergedContactSat[j].GetStartTime() "<< tabSat[s1].tabMergedContactSat[j].GetStartTime()<< endl;
                IntersecVecContact.clear();
                IntersecVecOS.clear();
                //IntersecVecGnav.clear();
                IntersecVecContact.resize(nbSat);
                IntersecVecOS.resize(nbSat);
                //IntersecVecGnav.resize(nbSat);
                
                for (int s2 = 0; s2 < nbSat; s2++)
                {
                    if(s1!=s2)
                    {
                        for(int m=0; m< tabSat[s2].tabMergedContactSat.size(); m++)
                        {
                            
                            if(((tabSat[s1].tabStartTimeOS[j] >= tabSat[s2].tabMergedContactSat[m].GetStartTime()) and (tabSat[s2].tabMergedContactSat[m].GetEndTime() >  tabSat[s1].tabStartTimeOS[j])))
                            {
                                
                                
                                
                                IntersecVecContact[s2].push_back(m);
                                
                            }
                            
                        }
                        
                        ////////////////////////////////////////////////
                        
                        for(int m=0; m< tabSat[s2].tabStartTimeOS.size(); m++)
                        {
                            
                            if(((tabSat[s1].tabStartTimeOS[j] >= tabSat[s2].tabStartTimeOS[m]) and
                                (tabSat[s2].tabEndTimeOS[m]  >  tabSat[s1].tabStartTimeOS[j])))
                                
                            {
                                //                                cout << " \t s1 j start " << tabSat[s1].tabMergedContactSat[j].GetStartTime() <<endl;
                                //                                cout << "  s2 m start " << tabSat[s2].tabStartTimeOS[m] <<endl;
                                //                                cout << " \t\t s2 m end " << tabSat[s2].tabStartTimeOS[m] +OSContactDuration <<endl;
                                
                                IntersecVecOS[s2].push_back(m);
                                
                            }
                            
                        }
                        
                        ////////////////////////////////////////////////
                        
                        //                        for(int m=0; m< tabSat[s2].tabStartTimeGnav.size(); m++)
                        //                        {
                        //
                        //                            if(((tabSat[s1].tabStartTimeGnav[j] >= tabSat[s2].tabStartTimeGnav[m]) and
                        //                                (tabSat[s2].tabStartTimeGnav[m] +GnavContactDuration >  tabSat[s1].tabStartTimeGnav[j])))
                        //
                        //                            {
                        ////                                cout << " \t s1 j start " << tabSat[s1].tabMergedContactSat[j].GetStartTime() <<endl;
                        ////                                cout << "  s2 m start " << tabSat[s2].tabStartTimeGnav[m] <<endl;
                        ////                                cout << " \t\t s2 m end " << tabSat[s2].tabStartTimeGnav[m] +GnavContactDuration <<endl;
                        ////
                        //                                IntersecVecGnav[s2].push_back(m);
                        //
                        //                            }
                        //
                        //                        }
                        
                        
                    }
                    //cout <<"IntersecVec[s2].size() " << IntersecVec[s2].size() <<endl;
                    
                    
                }
                
                
                
                for(int k=0; k<nbSites; k++)
                {
                    IloExpr Expr;
                    Expr = IloExpr(env);
                    
                    
                    Expr = e[s1][j][k];
                    
                    //cout << "x[s1][j][k]  " << s1 << " " <<  j << " " << k <<endl;
                    
                    
                    
                    for (int s2 = 0; s2 < nbSat; s2++)
                    {
                        if(s1!=s2 and  IntersecVecContact[s2].empty()!=1 )
                        {
                            Expr += x[s2][IntersecVecContact[s2][0]][k];          // Inter sat contact constraint
                            
                        }
                        if(s1!=s2 and  IntersecVecOS[s2].empty()!=1 )
                        {
                            Expr += e[s2][IntersecVecOS[s2][0]][k];   // Inter sat contact constraint
                            //cout << "e[s2][IntersecVecOS[s2][0]][k] " << s2 << " " <<  IntersecVecOS[s2][0] << " " << k <<endl;
                        }
                        //                        if(s1!=s2 and  IntersecVecGnav[s2].empty()!=1 )
                        //                        {
                        //                            //Expr += f[s2][IntersecVecGnav[s2][0]][k];   // Inter sat contact constraint
                        //                            cout << "f[s2][IntersecVecGnav[s2][0]][k] " << s2 << " " <<  IntersecVecGnav[s2][0] << " " << k <<endl;
                        //                        }
                        
                        
                    }
                    
                    
                    model.add(Expr <= nbAntennas);
                    
                    Expr.end();
                    
                    
                }
                //cin.get();
                
            }
            
        }
        cout << "OUT  Inter SatContact constraints " <<endl;
        
        ////////////
        // Contact includes contact constraints
        
        
        //        for (int s = 0; s < nbSat; s++)
        //        {
        //            for(int i=0; i< tabSat[s].tabMergedContactSat.size(); i++)
        //            {
        //                for(int j=i+1; j< tabSat[s].tabMergedContactSat.size(); j++)
        //                {
        //                    if(tabSat[s].tabMergedContactSat[i].GetStartTime()<= tabSat[s].tabMergedContactSat[j].GetStartTime() and tabSat[s].tabMergedContactSat[j].GetEndTime()<= tabSat[s].tabMergedContactSat[i].GetEndTime())
        //                    {
        //                        //                        cout << tabSat[s].tabMergedContactSat[i].GetStartTime() << "\t\t\t" << tabSat[s].tabMergedContactSat[i].GetEndTime() <<endl ;
        //                        //                        cout << "\t" << tabSat[s].tabMergedContactSat[j].GetStartTime() <<" " <<
        //                        //                        tabSat[s].tabMergedContactSat[j].GetEndTime() <<endl;
        //
        //                        for(int k=0; k<nbSites; k++)
        //                        {
        //
        //                            model.add( x[s][i][k] <=  x[s][j][k]);
        //
        //                        }
        //
        //
        //                    }
        //                }
        //            }
        //        }
        
        
        
        // OS, L-PRS and A-PRS messages constraints
        // Site visibility of sat constraints
        //  one location for each Contact OS
        
        int OSContactNonServed=0;
        //IloExpr expr2(env);
        
        for (int s = 0; s < nbSat; s++)
        {
            for(int j=0; j< tabSat[s].tabStartTimeOS.size(); j++)
            {
                test=0;
                for(int k=0; k<nbSites; k++)
                {
                    if (tabSat[s].VisibleInLocation( ContactSat(-1,-1,-1,tabSat[s].tabStartTimeOS[j],tabSat[s].tabEndTimeOS[j]),tabS[k]) ==0)
                    {
                        //cout << "non visible : sat " << s << " num contact " <<j<< " site " << k<<  " start " << tabSat[s].tabStartTimeOS[j] << " end "  << tabSat[s].tabStartTimeOS[j]+OSContactDuration <<endl;
                        
                        //expr2 +=e[s][j][k];
                        model.add(e[s][j][k] ==0);       // if Contact j of Sat s is non visible for site k
                        test++;
                    }
                    
                }
                if(test ==nbSites)
                {
                    cout << "non visible OSContact : sat " << s << " num contact " <<j<<  " start " << tabSat[s].tabStartTimeOS[j] << " end "  << tabSat[s].tabStartTimeOS[j]+OSContactDuration <<endl;
                    OSContactNonServed++;
                    //cin.get();
                    
                }
                else
                {
                    
                    model.add(IloSum(e[s][j]) == 1);       // ∑ (k) e[s][j] = 1
                    
                    
                    
                }
                
            }
        }
        cout << "-----------------------------"  <<endl;
        cout << "OSContactNonServed " << OSContactNonServed <<endl;
        
        ////////////
        // OS, L-PRS and A-PRS messages constraints
        // influence on x constraints
        
        //        for (int s = 0; s < nbSat; s++)
        //        {
        //            for(int i=0; i< tabSat[s].tabStartTimeOS.size(); i++)
        //            {
        //                for(int j=0; j< tabSat[s].tabMergedContactSat.size(); j++)
        //                {
        //
        //                    if(tabSat[s].tabStartTimeOS[i] <= tabSat[s].tabMergedContactSat[j].GetStartTime() and tabSat[s].tabMergedContactSat[j].GetEndTime() <= tabSat[s].tabStartTimeOS[i] + OSContactDuration  )       // contact in OS
        //                    {
        //                        cout << "contact in OS" <<endl;
        //                        cin.get();
        //                        for(int k=0; k<nbSites; k++)
        //                        {
        //
        //                            model.add( e[s][i][k] <=  x[s][j][k]);
        //
        //                        }
        //                    }
        //                    else
        //                    {
        //                        if((tabSat[s].tabStartTimeOS[i] < tabSat[s].tabMergedContactSat[j].GetEndTime() and tabSat[s].tabMergedContactSat[j].GetEndTime() < tabSat[s].tabStartTimeOS[i] + OSContactDuration ) or
        //                           (tabSat[s].tabStartTimeOS[i] < tabSat[s].tabMergedContactSat[j].GetStartTime() and tabSat[s].tabMergedContactSat[j].GetStartTime() < tabSat[s].tabStartTimeOS[i] + OSContactDuration ))   // intersection
        //                        {
        //                            cout << "possible ?" <<endl;
        //                            cout << "s  OSi " << s << " " << i << endl;
        //
        //                            cout << "\t" << tabSat[s].tabMergedContactSat[j].GetEndTime()<<endl;
        //                            cout << "" <<tabSat[s].tabStartTimeOS[i];
        //                            cout << "\t \t" << tabSat[s].tabStartTimeOS[i]+OSContactDuration  <<endl;
        //
        //                            cout << "\t" << tabSat[s].tabMergedContactSat[j].GetStartTime()<<endl;
        //                            cout << "" <<tabSat[s].tabStartTimeOS[i];
        //                            cout << "\t \t" << tabSat[s].tabStartTimeOS[i]+OSContactDuration  <<endl;
        //
        //
        //                            cin.get();
        //
        //                            for(int k=0; k<nbSites; k++)
        //                            {
        //
        //                                //model.add( x[s][j][k] <=  e[s][i][k]);
        //
        //                            }
        //                        }
        //
        //                        else     // OS in contact i
        //                        {
        //                            if( tabSat[s].tabMergedContactSat[j].GetStartTime()  <= tabSat[s].tabStartTimeOS[i] and   tabSat[s].tabStartTimeOS[i] + OSContactDuration <= tabSat[s].tabMergedContactSat[j].GetEndTime() )
        //                            {
        //                                cout << "OS in contact i" <<endl;
        //                                cin.get();
        //                                for(int k=0; k<nbSites; k++)
        //                                {
        //
        //                                    model.add( x[s][j][k] <=  e[s][i][k]);
        //
        //                                }
        //                            }
        //                        }
        //
        //                    }
        //
        //
        //                }
        //
        //
        //
        //            }
        //        }
        //
        
        
        ////////////
        // L- and A-PRS G/NAV-09 services  constraints
        // Site visibility of sat constraints
        //  one location for each Contact Gnav
        
        //        int GnavContactNonServed=0;
        //
        //        for (int s = 0; s < nbSat; s++)
        //        {
        //            for(int j=0; j< tabSat[s].tabStartTimeGnav.size(); j++)
        //            {
        //                test=0;
        //                for(int k=0; k<nbSites; k++)
        //                {
        //                    if (tabSat[s].VisibleInLocation( ContactSat(-2,-2,-2,tabSat[s].tabStartTimeGnav[j],tabSat[s].tabStartTimeGnav[j]+GnavContactDuration ),tabS[k]) ==0)
        //                    {
        //                        //cout << "non visible : " << s << " " <<j<< " " << k<<  " user  " << tabSat[s].tabMergedContactSat[j].GetUser()  << "  num contact " << tabSat[s].tabMergedContactSat[j].GetNumContact() << " start " << tabSat[s].tabMergedContactSat[j].GetStartTime() << " end "  << tabSat[s].tabMergedContactSat[j].GetEndTime() <<endl;
        //
        //                        model.add(f[s][j][k] ==0);       // if Contact j of Sat s is non visible for site k
        //                        test++;
        //                    }
        //
        //                }
        //                if(test ==5)
        //                {
        //                    cout << "non visible Gnavcontact  : sat " << s << " num contact " <<j<<  " start " << tabSat[s].tabStartTimeGnav[j] << " end "  << tabSat[s].tabStartTimeGnav[j]+GnavContactDuration <<endl;
        //                    GnavContactNonServed++;
        //                    //cin.get();
        //
        //                }
        //                else
        //                {
        //                    model.add(IloSum(f[s][j]) == 1);       // ∑ (k) f[s][j] = 1
        //
        //                }
        //
        //            }
        //        }
        //
        //        cout << "GnavContactNonServed " << GnavContactNonServed <<endl;
        //        cout << "-----------------------------"  <<endl;
        //
        //        ////////////
        //        // L- and A-PRS G/NAV-09 services  constraints
        //        // influence on x constraints
        //
        //        for (int s = 0; s < nbSat; s++)
        //        {
        //            for(int i=0; i< tabSat[s].tabStartTimeGnav.size(); i++)
        //            {
        //                for(int j=0; j< tabSat[s].tabMergedContactSat.size(); j++)
        //                {
        //
        //                    if(tabSat[s].tabStartTimeGnav[i] <= tabSat[s].tabMergedContactSat[j].GetStartTime() and tabSat[s].tabMergedContactSat[j].GetEndTime() <= tabSat[s].tabStartTimeGnav[i] + OSContactDuration  )   // contact in Gnav
        //                    {
        //                        for(int k=0; k<nbSites; k++)
        //                        {
        //
        //                            model.add( f[s][i][k] <=  x[s][j][k]);
        //
        //                        }
        //                    }
        //                    else
        //                    {
        //                        if((tabSat[s].tabStartTimeGnav[i] <= tabSat[s].tabMergedContactSat[j].GetEndTime() and tabSat[s].tabMergedContactSat[j].GetEndTime() <= tabSat[s].tabStartTimeGnav[i] + OSContactDuration ) or
        //                           (tabSat[s].tabStartTimeGnav[i] <= tabSat[s].tabMergedContactSat[j].GetStartTime() and tabSat[s].tabMergedContactSat[j].GetStartTime() <= tabSat[s].tabStartTimeGnav[i] + OSContactDuration ))    // intersection
        //                        {
        //                            for(int k=0; k<nbSites; k++)
        //                            {
        //
        //                                //model.add( x[s][j][k] <=  f[s][i][k]);
        //
        //                            }
        //                        }
        //
        //                        else
        //                        {
        //                            if( tabSat[s].tabMergedContactSat[j].GetStartTime()  <= tabSat[s].tabStartTimeGnav[i] and   tabSat[s].tabStartTimeGnav[i] + OSContactDuration <= tabSat[s].tabMergedContactSat[j].GetEndTime() )        // Gnav in Contact
        //                            {
        //                                for(int k=0; k<nbSites; k++)
        //                                {
        //
        //                                    model.add( x[s][j][k] <=  f[s][i][k]);
        //
        //                                }
        //                            }
        //                        }
        //
        //                    }
        //
        //
        //                }
        //
        //
        //
        //            }
        //        }
        //
        
        
        
        // Criterion  ------------------------------------
        //     ∑ (s)∑ (j) Wjl (∑ (l) x[s][j][l])
        // with Wjl= nb of contact representing by tabSat[s].tabMergedContactSat[j] = tabSat[s].CorrespondingContact[j].size()
        
        for (int s = 0; s < nbSat; s++)
        {
            model.add(Sum2[s] == IloSum(Sum1[s]));
            
            for(int j=0; j< tabSat[s].tabMergedContactSat.size(); j++)
            {
                //cout << "tabSat[s].CorrespondingContact[j].size() " << tabSat[s].CorrespondingContact[j].size() <<endl;
                model.add(Sum1[s][j] ==  tabSat[s].tabMergedContactSat[j].GetNumContact()* IloSum(x[s][j]));
            }
        }
        //model.add (IloMinimize(env, TotalnbContact-  IloSum(Sum2)))   ;
        model.add (IloMaximize(env, IloSum(Sum2)))  ;
        
        //___________________________________________________________________
        
        // Optimize
        IloCplex cplex(model);
        cplex.setParam(IloCplex::TiLim, 200);    // time out
        //cplex.setParam(IloCplex::EpGap, 0.01);     // duality gap set to 2%
        cplex.solve();
        
        env.out() << "Obj : " << cplex.getObjValue() << endl;
        cout << "TotalnbContact : " << TotalnbContact <<endl;
        //env.out() << "Unserved Contacts : " <<  TotalnbContact -  cplex.getObjValue() << endl;
        
        
        
        ////////////////////////////
        for (int s = 0; s < nbSat; s++)
        {
            for(int i=0; i< tabSat[s].tabMergedContactSat.size(); i++)
            {
                for(int j=i+1; j< tabSat[s].tabMergedContactSat.size(); j++)
                {
                    if(tabSat[s].tabMergedContactSat[i].GetStartTime()<= tabSat[s].tabMergedContactSat[j].GetStartTime() and tabSat[s].tabMergedContactSat[j].GetEndTime()<= tabSat[s].tabMergedContactSat[i].GetEndTime())
                    {
                        //                        cout << tabSat[s].tabMergedContactSat[i].GetStartTime() << "\t\t\t" << tabSat[s].tabMergedContactSat[i].GetEndTime() <<endl ;
                        //                        cout << "\t" << tabSat[s].tabMergedContactSat[j].GetStartTime() <<" " <<
                        //                        tabSat[s].tabMergedContactSat[j].GetEndTime() <<endl;
                        
                        for(int k=0; k<nbSites; k++)
                        {
                            
                            if(cplex.getValue(x[s][i][k]) > cplex.getValue(x[s][j][k]))
                            {
                                cout << "pb ! sat : " << s << " i " << i << " j " << j << " l " << k<<endl;
                                cout << tabSat[s].tabMergedContactSat[i].GetStartTime() << "\t\t\t" << tabSat[s].tabMergedContactSat[i].GetEndTime() << "\t\t\t" << cplex.getValue(x[s][i][k])<<endl ;
                                cout << "\t" << tabSat[s].tabMergedContactSat[j].GetStartTime() <<" " <<
                                tabSat[s].tabMergedContactSat[j].GetEndTime() << "\t\t\t" << cplex.getValue(x[s][j][k])<<endl ;
                                
                                //cin.get();
                            }
                            //x[s][i][k] <=  x[s][j][k]
                            
                        }
                        
                        
                    }
                }
            }
        }
        //        cout << "(x[19][0][3]) : " << cplex.getValue(x[19][0][3])<< " (x[19][1][3]) : " << cplex.getValue(x[19][1][3])<< endl ;
        //        cout << "\t" << tabSat[19].tabMergedContactSat[0].GetStartTime() <<endl;
        //        cout << "\t" << tabSat[19].tabMergedContactSat[1].GetStartTime() <<endl;
        
        ////////////////////////////
        
        int served;
        
        
        for (int s = 0; s < nbSat; s++)
        {
            //cout << s  << "  "<< tabSat[s].tabMergedContactSat.size()<<endl;
            
            for(int j=0; j< tabSat[s].tabStartTimeOS.size(); j++)
            {
                served=0;
                for(int l=0; l<nbSites; l++)
                {
                    if(cplex.getValue(e[s][j][l])==1)
                    {
                        //cout << "served : " << s << " " <<j<< " " << l <<endl;
                        tabS[l].ScheduleCAntenna[0].push_back(Contact(s, tabSat[s].tabStartTimeOS[j] , tabSat[s].tabEndTimeOS[j] ));
                        served=1;
                    }
                }
                if (served==0)
                {
//                    cout << "non served : " << s << " " <<j<< " " <<endl;
//                    cout << tabSat[s].tabStartTimeOS[j] << " " << tabSat[s].tabEndTimeOS[j] <<endl;
//
                }
            }
            
            //
            //            for(int j=0; j< tabSat[s].tabStartTimeGnav.size(); j++)
            //            {
            //
            //                for(int l=0; l<nbSites; l++)
            //                {
            //                    if(cplex.getValue(f[s][j][l])==1)
            //                    {
            //                        //cout << "served : " << s << " " <<j<< " " << l <<endl;
            //                        tabS[l].ScheduleCAntenna[0].push_back(Contact(s,
            //                                                                      tabSat[s].tabStartTimeGnav[j] , tabSat[s].tabStartTimeGnav[j] +GnavContactDuration));
            //
            //                    }
            //                }
            //            }
            
            
            
            
            for(int j=0; j< tabSat[s].tabMergedContactSat.size(); j++)
            {
                
                for(int l=0; l<nbSites; l++)
                {
                    if(cplex.getValue(x[s][j][l])==1)
                    {
                        //cout << "served : " << s << " " <<j<< " " << l <<endl;
                        tabS[l].ScheduleCAntenna[0].push_back(Contact(s, tabSat[s].tabMergedContactSat[j].GetStartTime(),tabSat[s].tabMergedContactSat[j].GetEndTime()));
                        
                    }
                    
                    vX[s][j]+=cplex.getValue(x[s][j][l]);
                    
                }
                
            }
        }
        
        
        
        cout << "MIP end" <<endl;
        
        
    }
    catch (IloException& ex) {
        cerr << "Error: " << ex << endl;
    }
    catch (...) {
        cerr << "Error" << endl;
    }
    env.end();
    
    return(0);
}


