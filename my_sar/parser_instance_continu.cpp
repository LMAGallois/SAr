#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <fstream>
#include <limits.h>
#include "parser_sites.h"
#include "parser.h"
#include "parser_instance.h"
#include "parser_instance_continu.h"
#include <math.h>

using namespace std;
#define PI 3.14159265

float ele_a_20_c(v_3 sat , v_3 on_earth){

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

void get_visi_u_c(vector < vector <v_3> > &trackSat, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <float> > > &u_or_aToSat, vector< vector < vector<float> > > contacts_sites){
    vector <vector <float> > temp;
    vector <float> temp2;
    float time_slot;
    float debut=-1;
    float end_time=0;

    for(int j=0; j < coordsOnEarth.size(); j++){
        u_or_aToSat.push_back(temp);

        for(int k=0; k< its_data.nbSatellites; k++){
            u_or_aToSat[j].push_back(temp2);
            
            for(int i=0; i < contacts_sites.size(); i++){
                v_3 b=coordsOnEarth[j];
                debut=-1;
                end_time=-1;
                time_slot=round(contacts_sites[i][0][0]/its_data.time_slot_sp3);
                v_3 a=trackSat[k][time_slot];

                if(ele_a_20_c(a,b)){

                    u_or_aToSat[j][k].push_back(1);

                }else{
                    //on regarde si visi a 20deg sur plus petite periode pour avoir un nb entre 0 et 1
                    for(int l=0; l< contacts_sites[i].size(); l++){

                        for(int m=0; m< contacts_sites[i][l].size(); m++){

                            time_slot=round(contacts_sites[i][l][m]/its_data.time_slot_sp3);
                            v_3 aa=trackSat[k][time_slot];

                            if(ele_a_20_c(aa,b) && debut==-1){
                                debut=contacts_sites[i][l][m];
                            }

                            if(!ele_a_20_c(aa,b) && debut!=-1){
                                end_time=contacts_sites[i][l][m];
                            }
                        }
                    }
                    if(debut!=-1 && end_time < debut && i < contacts_sites.size()-1){
                        end_time=contacts_sites[i+1][0][0];
                    }

                    if(debut!=-1 && debut >= contacts_sites[i][0][0]){

                        if((i+1) <= contacts_sites.size() && (end_time-debut >0)){                      
                            float time_temp=(end_time-debut)/(contacts_sites[i+1][0][0]-contacts_sites[i][0][0]);
                            u_or_aToSat[j][k].push_back(time_temp);
                        }
                    }
                }
            }
        }
    }
    cout <<"-------------------------get visi fin--------------------------------"<<endl;

}

