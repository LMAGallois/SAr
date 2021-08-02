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


void check_2sat_handover( vector< vector < vector<float> > > userToSat, defined_data its_data){
    int index = min(its_data.nbSatellites, its_data.nbAntennas*its_data.nbSites);
    const char* cfile = "in_out/plan_checker_h0.txt";
    ifstream file(cfile);
    if ( !file ) {
        cerr << "No such file: " << cfile << endl;
        throw(-1);
    }

    const char* cfile2 = "in_out/MIP_ssc_h0/checker_stats_2sat_h.txt";
//    ofstream file2(cfile2, ios::app);
    ofstream file2(cfile2);

    int taille=326169;

    //int taille=19760;
    vector < vector <schedule_ssc> > plan;
    vector <int> aff_1(index);
    vector <int> aff(index);
    int sat;
    float site;
    float antenna;
    int s;
    int e;
    int contact1;
    int contact11;
    int contact111;
    int s_1=-1;
    float min=INT_MAX;
    float max=INT_MIN;
    
    for(int i =0; i < taille; i++){
        file >> s;
        file >> e;
        file >> sat;
        file >> site;
        file >> antenna;
        file>>contact1;
        file>>contact11;
        file>>contact111;

        if(s_1 ==-1 || s_1!= s){
            vector <schedule_ssc> temp;
            plan.push_back(temp);
            schedule_ssc slot;
            slot.sat=sat;
            slot.s=s;
            slot.e=e;
            slot.site=site;
            slot.antenna=antenna;
            slot.contact1=contact1;
            slot.contact11=contact11;
            slot.contact111=contact111;
            plan.back().push_back(slot);
        }else{
            schedule_ssc slot;
            slot.sat=sat;
            slot.s=s;
            slot.e=e;
            slot.site=site;
            slot.antenna=antenna;
            slot.contact1=contact1;
            slot.contact11=contact11;
            slot.contact111=contact111;
            plan.back().push_back(slot);
        }

        s_1=s;
    }
    taille=plan.size();
    int taille2;
    float nb=0;
    float nb2=0;
    float dureeSlot=0;
    int nb_h=0;
    int sat_wout_h=0;
    int nb_sat=0;

    for(int i=0; i <taille; i ++ ){

        taille2=plan[i].size();
        nb=0;

        for(int k=0; k < its_data.nbUsers; k++){        
            sat_wout_h=0;
            nb_sat=0;

            for(int j=0; j < taille2; j++){
                schedule_ssc temp=plan[i][j];
                aff[temp.site*4+temp.antenna]=temp.sat;
                dureeSlot=temp.e-temp.s; 
                
                if(i==0 && aff[temp.site*4+temp.antenna]!=-1 && userToSat[k][temp.sat][temp.contact1]==1){
                    nb_sat++;

                }
                if( i!=0  && aff[temp.site*4+temp.antenna]!=-1 && userToSat[k][temp.sat][temp.contact1]==1){
                    nb_sat++;
                    if(aff[temp.site*4+temp.antenna]==aff_1[temp.site*4+temp.antenna]){
                        sat_wout_h++;
                    }
                }
                if(k==0 && i!=0 && (aff[temp.site*4+temp.antenna]!=aff_1[temp.site*4+temp.antenna]|| (aff_1[temp.site*4+temp.antenna]!=-1 && aff[temp.site*4+temp.antenna]==-1))){
                    nb_h++;
                }

            }
            
            if(i==0){
                if(nb_sat>=2){
                    nb2+=dureeSlot;
                    nb+=dureeSlot;
                }
            }else{
                if(nb_sat>=2){
                    if(sat_wout_h>=2){
                        nb2+=dureeSlot;
                        nb+=dureeSlot;
                    }else{
                        nb2+=dureeSlot-60;
                        nb+=dureeSlot-60;
                    }
                }
                
            } 
        
        }
        
        for(int d=0; d < index; d++){
            aff_1[d]=-1;
        }
        for(int j=0; j < taille2; j++){
            aff_1[plan[i][j].site*4+plan[i][j].antenna]=plan[i][j].sat;
        }
        float covering_slot_nb=0;
        if(nb>0){
            covering_slot_nb = (nb*100)/(1400*dureeSlot);
        }
        if(covering_slot_nb<min){
            min=covering_slot_nb;
        }
        if(covering_slot_nb>max){
            max=covering_slot_nb;
        }
        file2 << "slot "<< i <<endl;
        file2 << covering_slot_nb<<"%"<<endl;
        file2 << "----------------------------"<<endl;
        
    }
    float covering_nb = (nb2*100)/(1400*its_data.horizon_c);

    file2 << "covering nb sur la totalité du temps: "<<covering_nb <<"%"<<endl;
    file2 << "min covering slot : "<<min<<"%"<<endl;
    file2 << "max covering slot : "<<max<<"%"<<endl;
    file2<< "nb_h : "<<nb_h<<endl;

}

void check_2sat(    vector< vector < vector<float> > > userToSat, defined_data its_data){
    int index = min(its_data.nbSatellites, its_data.nbAntennas);
    
    const char* cfile = "in_out/plan_checker_h0.txt";
    ifstream file(cfile);
    if ( !file ) {
        cerr << "No such file: " << cfile << endl;
        throw(-1);
    }

    const char* cfile2 = "in_out/MIP_ssc_h0/checker_stats_2sat.txt";
//    ofstream file2(cfile2, ios::app);
    ofstream file2(cfile2);

    int taille=326169;

 //   int taille=19760;
    vector < vector <schedule_ssc> > plan;
    int sat;
    float site;
    float antenna;
    int s;
    int e;
    int s_1=-1;
    float min=INT_MAX;
    float max=INT_MIN;
    int contact1;
    int contact11;
    int contact111;

    for(int i =0; i < taille; i++){
        file >> s;
        file >> e;
        file >> sat;
        file >> site;
        file >> antenna;
        file>>contact1;
        file>>contact11;
        file>>contact111;

        if(s_1 ==-1 || s_1!= s){
            vector <schedule_ssc> temp;
            plan.push_back(temp);
            schedule_ssc slot;
            slot.sat=sat;
            slot.s=s;
            slot.e=e;
            slot.site=site;
            slot.antenna=antenna;
            slot.contact1=contact1;
            slot.contact11=contact11;
            slot.contact111=contact111;
            plan.back().push_back(slot);
        }else{
            schedule_ssc slot;
            slot.sat=sat;
            slot.s=s;
            slot.e=e;
            slot.site=site;
            slot.antenna=antenna;
            slot.contact1=contact1;
            slot.contact11=contact11;
            slot.contact111=contact111;
            plan.back().push_back(slot);
        }

        s_1=s;
    }
    taille=plan.size();
    int taille2;
    float nb=0;
    float nb2=0;
    float dureeSlot=0;
    int nb_sat=0;

    for(int i=0; i <taille; i ++ ){

        taille2=plan[i].size();
        nb=0;

        for(int k=0; k < its_data.nbUsers; k++){        
            nb_sat=0;

            for(int j=0; j < taille2; j++){
                schedule_ssc temp=plan[i][j];
                dureeSlot=temp.e-temp.s; 
                
                if( temp.sat!=-1){
                    if(userToSat[k][temp.sat][temp.contact1]==1){
                        nb_sat++;
                    }
                }

            }

            if(nb_sat >= 2){
                nb2+=dureeSlot;
                nb+=dureeSlot;
            }
        
        }

        float covering_slot_nb=0;
        if(nb>0){
            covering_slot_nb = (nb*100)/(1400*dureeSlot);
        }        
        if(covering_slot_nb<min){
            min=covering_slot_nb;
        }
        if(covering_slot_nb>max){
            max=covering_slot_nb;
        }
        file2 << "slot "<< i <<endl;
        file2 << covering_slot_nb<<"%"<<endl;
        file2 << "----------------------------"<<endl;
        
    }

    float covering_nb = (nb2*100)/(1400*its_data.horizon_c);
    file2 << "covering nb sur la totalité du temps: "<<covering_nb <<"%"<<endl;
    file2 << "min covering slot : "<<min<<"%"<<endl;
    file2 << "max covering slot : "<<max<<"%"<<endl;

}

void check_1sat(    vector< vector < vector<float> > > userToSat, defined_data its_data){
    const char* cfile = "in_out/plan_checker_h0.txt";
    ifstream file(cfile);
    if ( !file ) {
        cerr << "No such file: " << cfile << endl;
        throw(-1);
    }

    const char* cfile2 = "in_out/MIP_ssc_h0/checker_stats_1sat.txt";
//    ofstream file2(cfile2, ios::app);
    ofstream file2(cfile2);

    int taille=326169;

//    int taille=19760;
    vector < vector <schedule_ssc> > plan;

    int sat;
    float site;
    float antenna;
    int s;
    int e;
    int s_1=-1;
    float min=INT_MAX;
    float max=INT_MIN;
    int contact1;
    int contact11;
    int contact111;

    for(int i =0; i < taille; i++){
        file >> s;
        file >> e;
        file >> sat;
        file >> site;
        file >> antenna;
        file>>contact1;
        file>>contact11;
        file>>contact111;

        if(s_1 ==-1 || s_1!= s){
            vector <schedule_ssc> temp;
            plan.push_back(temp);
            schedule_ssc slot;
            slot.sat=sat;
            slot.s=s;
            slot.e=e;
            slot.site=site;
            slot.antenna=antenna;
            slot.contact1=contact1;
            slot.contact11=contact11;
            slot.contact111=contact111;
            plan.back().push_back(slot);
        }else{
            schedule_ssc slot;
            slot.sat=sat;
            slot.s=s;
            slot.e=e;
            slot.site=site;
            slot.antenna=antenna;
            slot.contact1=contact1;
            slot.contact11=contact11;
            slot.contact111=contact111;
            plan.back().push_back(slot);
        }

        s_1=s;
    }
    taille=plan.size();
    int taille2;
    float nb=0;
    float nb2=0;
    float dureeSlot=0;
    int nb_sat=0;

    for(int i=0; i <taille; i ++ ){

        taille2=plan[i].size();
        nb=0;
       
        for(int k=0; k < its_data.nbUsers; k++){        
            nb_sat=0; 

            for(int j=0; j < taille2; j++){
                schedule_ssc temp=plan[i][j];
                dureeSlot=temp.e-temp.s; 
                
                if( temp.sat!=-1){
                    if(userToSat[k][temp.sat][temp.contact1]==1){
                        nb_sat++;
                    }
                }

            }

            if(nb_sat >= 1){
                nb2+=dureeSlot;
                nb+=dureeSlot;
            }
        
        }

        float covering_slot_nb=0;
        if(nb>0){
            covering_slot_nb = (nb*100)/(1400*dureeSlot);
        }
        if(covering_slot_nb>max){
            max=covering_slot_nb;
        }
        if(covering_slot_nb<min){
            min=covering_slot_nb;
        }
        file2 << "slot "<< i <<endl;
        file2 << covering_slot_nb<<"%"<<endl;
        file2 << "----------------------------"<<endl;
        
    }

    float covering_nb = (nb2*100)/(1400*its_data.horizon_c);
    file2 << "covering nb sur la totalité du temps: "<<covering_nb <<"%"<<endl;
    file2 << "min covering slot : "<<min<<"%"<<endl;
    file2 << "max covering slot : "<<max<<"%"<<endl;

}

void check_1sat_handover(    vector< vector < vector<float> > > userToSat, defined_data its_data){
    int index = min(its_data.nbSatellites, its_data.nbAntennas*its_data.nbSites);
    const char* cfile = "in_out/plan_checker_h0.txt";
    ifstream file(cfile);
    if ( !file ) {
        cerr << "No such file: " << cfile << endl;
        throw(-1);
    }

    const char* cfile2 = "in_out/MIP_ssc_h0/checker_stats_1sat_h.txt";
//    ofstream file2(cfile2, ios::app);
    ofstream file2(cfile2);


    int taille=326169;
  //   int taille=57080;

    vector < vector <schedule_ssc> > plan;
    vector <int> aff_1(index);
    vector <int> aff(index);
    int sat;
    float site;
    float antenna;
    int s;
    int e;
    int s_1=-1;
    int contact1;
    int contact11;
    int contact111;
    
    for(int i =0; i < taille; i++){
        file >> s;
        file >> e;
        file >> sat;
        file >> site;
        file >> antenna;
        file>>contact1;
        file>>contact11;
        file>>contact111;

        if(s_1 ==-1 || s_1!= s){
            vector <schedule_ssc> temp;
            plan.push_back(temp);
            schedule_ssc slot;
            slot.sat=sat;
            slot.s=s;
            slot.e=e;
            slot.site=site;
            slot.antenna=antenna;
            slot.contact1=contact1;
            slot.contact11=contact11;
            slot.contact111=contact111;
            plan.back().push_back(slot);
        }else{
            schedule_ssc slot;
            slot.sat=sat;
            slot.s=s;
            slot.e=e;
            slot.site=site;
            slot.antenna=antenna;
            slot.contact1=contact1;
            slot.contact11=contact11;
            slot.contact111=contact111;
            plan.back().push_back(slot);
        }

        s_1=s;
    }
    taille=plan.size();
    int taille2;
    float dureeSlot=0;
    float min=INT_MAX;
    float max=INT_MIN;
    float nb=0;
    float nb2=0;
    int nb_h=0;
    int sat_wout_h=0;
    int nb_sat=0;

    for(int i=0; i <taille; i ++ ){

        taille2=plan[i].size();
        nb=0;

        for(int k=0; k < its_data.nbUsers; k++){        
            sat_wout_h=0;
            nb_sat=0;

            for(int j=0; j < taille2; j++){
                schedule_ssc temp=plan[i][j];
                aff[temp.site*4+temp.antenna]=temp.sat;
                dureeSlot=temp.e-temp.s; 
                
                if(i==0 && aff[temp.site*4+temp.antenna]!=-1 && userToSat[k][temp.sat][temp.contact1]==1){
                    nb_sat++;

                }
                if( i!=0  && aff[temp.site*4+temp.antenna]!=-1 && userToSat[k][temp.sat][temp.contact1]==1){
                    nb_sat++;
                    if(aff[temp.site*4+temp.antenna]==aff_1[temp.site*4+temp.antenna]){
                        sat_wout_h++;
                    }
                }
                if(k==0 && i!=0 && (aff[temp.site*4+temp.antenna]!=aff_1[temp.site*4+temp.antenna]|| (aff_1[temp.site*4+temp.antenna]!=-1 && aff[temp.site*4+temp.antenna]==-1))){
                    nb_h++;
                }

            }
            
            if(i==0){
                if(nb_sat>=1){
                    nb2+=dureeSlot;
                    nb+=dureeSlot;
                }
            }else{
                if(nb_sat>=1){
                    if(sat_wout_h>=1){
                        nb2+=dureeSlot;
                        nb+=dureeSlot;
                    }else{
                        nb2+=dureeSlot-60;
                        nb+=dureeSlot-60;
                    }
                }
                
            } 
        
        }

        for(int d=0; d < index; d++){
            aff_1[d]=-1;
        }
        for(int j=0; j < taille2; j++){
            aff_1[plan[i][j].site*4+plan[i][j].antenna]=plan[i][j].sat;
        }

        float covering_slot_nb=0;
        if(nb>0){
            covering_slot_nb = (nb*100)/(1400*dureeSlot);
        }
        if(covering_slot_nb<min){
            min=covering_slot_nb;
        }
        if(covering_slot_nb>max){
            max=covering_slot_nb;
        }
        file2 << "slot "<< i <<endl;
        file2 << covering_slot_nb<<"%"<<endl;
        file2 << "----------------------------"<<endl;
        
    }

    float covering_nb = (nb2*100)/(1400*its_data.horizon_c);
    file2 << "covering nb sur la totalité du temps: "<<covering_nb <<"%"<<endl;
    file2 << "min covering slot : "<<min<<"%"<<endl;
    file2 << "max covering slot : "<<max<<"%"<<endl;
    file2<< "nb_h : "<<nb_h<<endl;

}