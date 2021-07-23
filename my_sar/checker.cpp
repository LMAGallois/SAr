#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <fstream>
#include "parser.h"
#include "parser_sites.h"
#include "solver.h"
#include "solver_sites.h"
#include <limits.h>
#include <algorithm> 

using namespace std;

void check_2sat_handover(vector < vector < vector < int> > > userToSat, defined_data its_data){
    int index = min(its_data.nbSatellites, its_data.nbAntennas*its_data.nbSites);
    const char* cfile = "in_out/plan_checker.txt";
    ifstream file(cfile);
    if ( !file ) {
        cerr << "No such file: " << cfile << endl;
        throw(-1);
    }

    const char* cfile2 = "in_out/checker_stats.txt";
//    ofstream file2(cfile2, ios::app);
    ofstream file2(cfile2);


    int taille=38411;
    vector < vector <schedule_ssc> > plan;
    vector <int> aff_1(index);
    vector <int> aff(index);
    int sat;
    float site;
    float antenna;
    int s;
    int e;
    int s_1=-1;
    
    for(int i =0; i < taille; i++){
        file >> s;
        file >> e;
        file >> sat;
        file >> site;
        file >> antenna;

        if(s_1 ==-1 || s_1!= s){
            vector <schedule_ssc> temp;
            plan.push_back(temp);
            schedule_ssc slot={s,e, sat, site, antenna};
            plan.back().push_back(slot);
        }else{
            schedule_ssc slot={s,e, sat, site, antenna};
            plan.back().push_back(slot);
        }

        s_1=s;
    }
    taille=plan.size();
    int taille2;
    vector <int> people(its_data.nbUsers);
    vector <float> covering_slot(taille);
    int handover=0;

    for(int i=0; i <taille; i ++ ){

        taille2=plan[i].size();
        vector <int> people_al(its_data.nbUsers);

        for(int j=0; j < taille2; j++){
            handover=0;
            schedule_ssc temp=plan[i][j];
            aff[temp.site*4+temp.antenna]=temp.sat;
                

            if( i!=0 && aff[temp.site*4+temp.antenna]!=aff_1[temp.site*4+temp.antenna]){
                handover=1;
            }

            for(int k=0; k < its_data.nbUsers; k++){        
                
                if((people_al[k] ==0 || people_al[k] ==1) && userToSat[k][temp.sat][temp.s]==1){
                  
                    people_al[k]++;
                    if(people_al[k]==2){
                        if(handover==1){
                            people[k]+=540;
                        }else{
                            people[k]+=600;
                        }
                        
                    }
                }
                
            }
            for(int d=0; d < index; d++){
                aff_1[d]=-1;
            }
            aff_1[temp.site*4+temp.antenna]=temp.sat;
        }
        
        float nv=0;

        for(int i=0; i< its_data.nbUsers; i ++){
            if(people_al[i]==0 || people_al[i]==1){
                nv++;
            }
        }

        float covering_slot_nb = (((1400*600) - nv*600)*100)/(1400*600);
        covering_slot[i]=covering_slot_nb;
        file2 << "slot "<< i <<endl;
        file2 << covering_slot_nb<<"%"<<endl;
        file2 << "----------------------------"<<endl;
 
    }
    float nb=0;
    for(int i=0; i < taille; i++){
        nb+=covering_slot[i];
    }

    float nb2=0;
    for(int i=0; i < its_data.nbUsers; i++){
        nb2+=people[i];
    }

    float average_covering_slot=nb/taille;
    float covering_nb = (nb2*100)/(1400*its_data.horizon_c);
    file2<<"average covering slot : "<< average_covering_slot<<"%"<<endl;
    file2 << "covering nb sur la totalité du temps: "<<covering_nb <<"%"<<endl;

}

void check_2sat(vector < vector < vector < int> > > userToSat, defined_data its_data){
    int index = min(its_data.nbSatellites, its_data.nbAntennas);
    
    const char* cfile = "in_out/plan_checker.txt";
    ifstream file(cfile);
    if ( !file ) {
        cerr << "No such file: " << cfile << endl;
        throw(-1);
    }

    const char* cfile2 = "in_out/checker_stats.txt";
//    ofstream file2(cfile2, ios::app);
    ofstream file2(cfile2);


    int taille=38411;
    vector < vector <schedule_ssc> > plan;
    int sat;
    float site;
    float antenna;
    int s;
    int e;
    int s_1=-1;
    
    for(int i =0; i < taille; i++){
        file >> s;
        file >> e;
        file >> sat;
        file >> site;
        file >> antenna;

        if(s_1 ==-1 || s_1!= s){
            vector <schedule_ssc> temp;
            plan.push_back(temp);
            schedule_ssc slot={s,e, sat, site, antenna};
            plan.back().push_back(slot);
        }else{
            schedule_ssc slot={s,e, sat, site, antenna};
            plan.back().push_back(slot);
        }

        s_1=s;
    }
    taille=plan.size();
    int taille2;
    vector <int> people(its_data.nbUsers);
    vector <float> covering_slot(taille);

    for(int i=0; i <taille; i ++ ){

        taille2=plan[i].size();
        vector <int> people_al(its_data.nbUsers);

        for(int j=0; j < taille2; j++){
            schedule_ssc temp=plan[i][j];

            for(int k=0; k < its_data.nbUsers; k++){        
                
                if((people_al[k] ==0 || people_al[k] ==1) && userToSat[k][temp.sat][temp.s]==1){
                  
                    people_al[k]++;
                    if(people_al[k]==2){
                        people[k]+=600;
                    }
                }
                
            }
        }
        
        float nv=0;

        for(int i=0; i< its_data.nbUsers; i ++){
            if(people_al[i]==0 || people_al[i]==1){
                nv++;
            }
        }

        float covering_slot_nb = (((1400*600) - nv*600)*100)/(1400*600);
        covering_slot[i]=covering_slot_nb;
        file2 << "slot "<< i <<endl;
        file2 << covering_slot_nb<<"%"<<endl;
        file2 << "----------------------------"<<endl;
        
    }
    float nb=0;
    for(int i=0; i < taille; i++){
        nb+=covering_slot[i];
    }
    float nb2=0;
    for(int i=0; i < its_data.nbUsers; i++){
        nb2+=people[i];
    }
    float average_covering_slot=nb/taille;
    float covering_nb = (nb2*100)/(1400*its_data.horizon_c);
    file2<<"average covering slot : "<< average_covering_slot<<"%"<<endl;
    file2 << "covering nb sur la totalité du temps: "<<covering_nb <<"%"<<endl;

}

void check_1sat(vector < vector < vector < int> > > userToSat, defined_data its_data){
    const char* cfile = "in_out/plan_checker.txt";
    ifstream file(cfile);
    if ( !file ) {
        cerr << "No such file: " << cfile << endl;
        throw(-1);
    }

    const char* cfile2 = "in_out/checker_stats.txt";
//    ofstream file2(cfile2, ios::app);
    ofstream file2(cfile2);


    int taille=38411;
    vector < vector <schedule_ssc> > plan;

    int sat;
    float site;
    float antenna;
    int s;
    int e;
    int s_1=-1;
    
    for(int i =0; i < taille; i++){
        file >> s;
        file >> e;
        file >> sat;
        file >> site;
        file >> antenna;

        if(s_1 ==-1 || s_1!= s){
            vector <schedule_ssc> temp;
            plan.push_back(temp);
            schedule_ssc slot={s,e, sat, site, antenna};
            plan.back().push_back(slot);
        }else{
            schedule_ssc slot={s,e, sat, site, antenna};
            plan.back().push_back(slot);
        }

        s_1=s;
    }
    taille=plan.size();
    int taille2;
    vector <int> people(its_data.nbUsers);
    vector <float> covering_slot(taille);

    for(int i=0; i <taille; i ++ ){

        taille2=plan[i].size();
        vector <int> people_al(its_data.nbUsers);

        for(int j=0; j < taille2; j++){
            
            schedule_ssc temp=plan[i][j];

            for(int k=0; k < its_data.nbUsers; k++){        

                if(people_al[k] != -1 && userToSat[k][temp.sat][temp.s]==1){
                    people[k]+=600;
                    people_al[k]=-1;
                }           
            }
        }
        
        float nv=0;

        for(int i=0; i< its_data.nbUsers; i ++){
            if(people_al[i]==0 ){
                nv++;
            }
        }

        float covering_slot_nb = (((1400*600) - nv*600)*100)/(1400*600);
        covering_slot[i]=covering_slot_nb;
        file2 << "slot "<< i <<endl;
        file2 << covering_slot_nb<<"%"<<endl;
        file2 << "----------------------------"<<endl;
        
    }
    float nb=0;
    for(int i=0; i < taille; i++){
        nb+=covering_slot[i];
    }
    float nb2=0;
    for(int i=0; i < its_data.nbUsers; i++){
        nb2+=people[i];
    }
    float average_covering_slot=nb/taille;
    float covering_nb = (nb2*100)/(1400*its_data.horizon_c);
    file2<<"average covering slot : "<< average_covering_slot<<"%"<<endl;
    file2 << "covering nb sur la totalité du temps: "<<covering_nb <<"%"<<endl;

}

void check_1sat_handover(vector < vector < vector < int> > > userToSat, defined_data its_data){
    int index = min(its_data.nbSatellites, its_data.nbAntennas*its_data.nbSites);
    const char* cfile = "in_out/plan_checker.txt";
    ifstream file(cfile);
    if ( !file ) {
        cerr << "No such file: " << cfile << endl;
        throw(-1);
    }

    const char* cfile2 = "in_out/checker_stats.txt";
//    ofstream file2(cfile2, ios::app);
    ofstream file2(cfile2);


    int taille=38411;
    vector < vector <schedule_ssc> > plan;
    vector <int> aff_1(index);
    vector <int> aff(index);
    int sat;
    float site;
    float antenna;
    int s;
    int e;
    int s_1=-1;
    
    for(int i =0; i < taille; i++){
        file >> s;
        file >> e;
        file >> sat;
        file >> site;
        file >> antenna;

        if(s_1 ==-1 || s_1!= s){
            vector <schedule_ssc> temp;
            plan.push_back(temp);
            schedule_ssc slot={s,e, sat, site, antenna};
            plan.back().push_back(slot);
        }else{
            schedule_ssc slot={s,e, sat, site, antenna};
            plan.back().push_back(slot);
        }

        s_1=s;
    }
    taille=plan.size();
    int taille2;
    vector <int> people(its_data.nbUsers);
    vector <float> covering_slot(taille);
    int handover=0;

    for(int i=0; i <taille; i ++ ){

        taille2=plan[i].size();
        vector <int> people_al(its_data.nbUsers);

        for(int j=0; j < taille2; j++){
                        handover=0;

            schedule_ssc temp=plan[i][j];
            aff[temp.site*4+temp.antenna]=temp.sat;

            if( i!=0 && aff[temp.site*4+temp.antenna]!=aff_1[temp.site*4+temp.antenna]){
                handover=1;
            }
            for(int k=0; k < its_data.nbUsers; k++){        

                if(people_al[k] != -1 && userToSat[k][temp.sat][temp.s]==1){
                    if(handover==1){
                        people[k]+=540;
                    }else{
                        people[k]+=600;
                    }
                    people_al[k]=-1;
                }           
            }
                        for(int d=0; d < index; d++){
                aff_1[d]=-1;
            }
            aff_1[temp.site*4+temp.antenna]=temp.sat;
        }
        
        float nv=0;

        for(int i=0; i< its_data.nbUsers; i ++){
            if(people_al[i]==0 ){
                nv++;
            }
        }

        float covering_slot_nb = (((1400*600) - nv*600)*100)/(1400*600);
        covering_slot[i]=covering_slot_nb;
        file2 << "slot "<< i <<endl;
        file2 << covering_slot_nb<<"%"<<endl;
        file2 << "----------------------------"<<endl;
        
    }
    float nb=0;
    for(int i=0; i < taille; i++){
        nb+=covering_slot[i];
    }
    float nb2=0;
    for(int i=0; i < its_data.nbUsers; i++){
        nb2+=people[i];
    }
    float average_covering_slot=nb/taille;
    float covering_nb = (nb2*100)/(1400*its_data.horizon_c);
    file2<<"average covering slot : "<< average_covering_slot<<"%"<<endl;
    file2 << "covering nb sur la totalité du temps: "<<covering_nb <<"%"<<endl;

}