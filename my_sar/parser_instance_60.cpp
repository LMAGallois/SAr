#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <fstream>
#include <limits.h>
#include "parser_sites.h"
#include "parser.h"
#include "parser_instance_60.h"
#include <math.h>

using namespace std;
#define PI 3.14159265

float ele_a_20_60(v_3 sat , v_3 on_earth){

    float res;
    v_3 a={-sat.x, -sat.y, -sat.z};

    v_3 b={on_earth.x-sat.x, on_earth.y-sat.y, on_earth.z-sat.z};

    float nume=a.x*b.x+a.y*b.y+a.z*b.z;

    float deno=sqrt(a.x*a.x+a.y*a.y+a.z*a.z )*sqrt(b.x*b.x+b.y*b.y+b.z*b.z);

    res=acos(nume/deno) * 180.0 / PI;

    if (res <= (90.0-20.0)){
        res=1;
    }else{
        res=0;
    }
    return res;
}

void get_visi_u_60(vector < vector <v_3> > &trackSat, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <float> > > &u_or_aToSat, vector < vector<float> > contacts_sites){
    vector <vector <float> > temp;
    vector <float> temp2;
    float time_slot;

    for(int j=0; j < coordsOnEarth.size(); j++){
        u_or_aToSat.push_back(temp);
        for(int k=0; k< its_data.nbSatellites; k++){
            u_or_aToSat[j].push_back(temp2);
            for(int i=0; i < contacts_sites.size(); i++){
                time_slot=round(contacts_sites[i][0]/its_data.time_slot_sp3);
                v_3 a=trackSat[k][time_slot];
                v_3 b=coordsOnEarth[j];

                if(ele_a_20_60(a,b)){
                    u_or_aToSat[j][k].push_back(1);
                }else{
                    u_or_aToSat[j][k].push_back(0);
                }
            }
        }
    }
    cout <<"-------------------------get visi fin--------------------------------"<<endl;

}

void get_visi_a_60_thales(vector < vector <v_3> > &trackSat, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <int> > > &u_or_aToSat, vector < vector<float> > contacts_sites){
    vector <vector <int> > temp;
    vector <int> temp2;
    float time_slot;

    for(int j=0; j < coordsOnEarth.size(); j++){
        
        for(int t=0; t< its_data.nbAntennas; t++){
            u_or_aToSat.push_back(temp);

            for(int k=0; k< its_data.nbSatellites; k++){
                u_or_aToSat[j*4+t].push_back(temp2);

                for(int i=0; i < contacts_sites.size(); i++){
                    v_3 b=coordsOnEarth[j];
                    time_slot=round(contacts_sites[i][0]/its_data.time_slot_sp3);
                    v_3 a=trackSat[k][time_slot];

                    if(ele_a_20_60(a,b)){
                        u_or_aToSat[j*4+t][k].push_back(1);
                    }else{
                        u_or_aToSat[j*4+t][k].push_back(0);
                    }
                }
            }
        }
    }
    cout <<"-------------------------get visi fin--------------------------------"<<endl;

}

void parse_contacts_sites_60_i(vector< vector < vector <int> > > &antennaToSat, vector < vector <float> > &contacts, defined_data its_data)
{
    list<float> contacts_temp;
    const char* output_file = "in_out/contacts_sites_h.txt";

    string str;
    int c;
    int nbSat=-1;
    int nbContact;
    int sat;
    float s;
    float e=0;
    float d;
    int contact=0;
    cout << "------ start reading site ---------" << endl;
    for(int i=0; i< antennaToSat.size(); i++){
        for(int j=0; j< antennaToSat[i].size(); j++){
            for(int k=0; k< antennaToSat[i][j].size(); k++){
                if(k*its_data.time_slot_sp3 <= its_data.horizon_c){
                    if(antennaToSat[i][j][k]==1 && contact==0){
                        contact=1;
                        contacts_temp.push_back(float(k*its_data.time_slot_sp3));
                        contacts_temp.sort();  
                    }
                    if(antennaToSat[i][j][k]==0 && contact==1){
                        contact=0;
                        contacts_temp.push_back(float(k*its_data.time_slot_sp3));
                        contacts_temp.sort();  
                    }
                }
            }
        }
    }

    cout << "------ end reading ---------" << endl;
    
    //on enleve les doublons avant d ecrire dans le fichier
    int taille = contacts_temp.size();
    float memo1=-1;
    for(int it=0; it < taille ; it++){
        
        float temp1=contacts_temp.front();
        contacts_temp.pop_front();
        float temp2=contacts_temp.front();
        if(temp1==temp2){
            if(temp1!=memo1){
                contacts_temp.push_back(temp1);
                memo1=temp1;
            }

        }else{
            if(temp1!=memo1){
                contacts_temp.push_back(temp1);
                contacts_temp.push_back(temp2);
            }else{
                contacts_temp.push_back(temp2);
            }
            memo1=temp2;
        }
    }
    contacts_temp.pop_front();
    contacts_temp.sort();
    // contcats_temp comprend tous les temps
    
    int a;
    int b;
    int j=0;
    int k=0;
    float memo60=0;
    int al2=0;

    while(contacts_temp.size()!=0){
        a=contacts_temp.front();
        contacts_temp.pop_front();
        al2=0;

        if(k==0){
            vector < float > list_temp;
            contacts.push_back(list_temp);
            memo60=a;
            
        }
        if( a-memo60 >=60){
            vector < float > list_temp;
            contacts.push_back(list_temp);
            j++;
            contacts[j].push_back(a);
            memo60=a;
            al2=1;

        }
        if(!al2){
            contacts[j].push_back(a);
            k++;
        }
    }

//ecriture dans le fichier in_out/contacts
    ofstream output(output_file);

    taille=contacts.size();
    for(int it=0; it < taille ; it++){
        int taille2=contacts[it].size();

        for(int it2=0; it2 < taille2 ; it2++){
            
            float temp= contacts[it][it2];
            output << temp << endl;
            
        }
    }

    cout << "------ end parsing ---------" << endl;
}

void reconstruct_visi_a_60(vector< vector < vector <int> > > &antennaToSat, vector < vector <float> > &contacts, vector< vector < vector <int> > > &antennaToSatF, defined_data its_data){
    vector<vector<int> > temp1;
    vector<int> temp2;
    int index=0;

    for(int i=0; i< antennaToSat.size(); i++){
        antennaToSatF.push_back(temp1);

        for(int j=0; j< antennaToSat[i].size(); j++){
            antennaToSatF[i].push_back(temp2);
            index=0;

            for(int k=0; k< contacts.size(); k++){

                if(antennaToSat[i][j][index]==1 ){
                    antennaToSatF[i][j].push_back(1);
                }

                if(antennaToSat[i][j][index]==0 ){
                    antennaToSatF[i][j].push_back(0);
                }

                for(int l=0; l< contacts[k].size(); l++){

                    index++;
                    
                }
            }
        }
    }
}