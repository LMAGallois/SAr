#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <limits.h>
#include "s_i.h"
#include <float.h>

using namespace std;


//on prend pas en compte les contraintes MCD or NDRR donc pas de sat prioritaires par rapport à ces contraintes 

/*on remplit le s_i avec matrix_index sat (nb de sat)
le sat avec le mini gcd discarded*/

float gcd(int sat, int contact, vector< vector <v_3> > trackSat){
    float res=0;
    float x=trackSat(sat, contact).x;
    float y=trackSat(sat, contact).y;
    float z=trackSat(sat, contact).z;

    for(int i=0; i< its_data.nb_SVs; i++){
        if(i!=sat){
            res=res+((x-trackSat(i,contact).x )+ (y-trackSat(i,contact).y) + (z-trackSat(i,contact).z));
        }
    }

    return res;
}

vector<int> s_i( vector<int > &cd_sat, vector <int> &dd_sat, int contact, defined_data its_data, vector< vector <v_3> > trackSat){
    int index = min(its_data.nb_SVs, its_data.nb_ULS_s*its_data.nb_Antennas);
    vector<int> res_temp(its_data.nb_SVs);
    vector <int> res(index);
    for(int i=0; i < its_data.nb_SVs; i++){
        res_temp[i]=i;
    }

    int nb_to_discard = its_data.nb_SVs - index;
    int chosen_sat;

    if(nb_to_discard > 0){
        for (int j = 0 ; j < nb_to_discard ; j++){
        
            for(int i = 0 ; i < its_data.nb_SVs ; i++){
                float min = FLOAT_MAX;
                float temp = gcd(i, contact, trackSat);

                if(temp < min){
                    min = temp;
                    chosen_sat=i;
                }
            }

            res_temp[chosen_sat]=-1;
        }
    }
    for(int i=0; i < its_data.nb_SVs; i++){
                
        if(res_temp[i]!=-1){
            res[i]=i;
        }
    }
    return res;
}