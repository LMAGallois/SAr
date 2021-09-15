#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <math.h>
#include <limits.h>
#include "u_i.h"

#define PI 3.14159265

int nb_eph;

float e_dist(vector <v_3> &coorSite ,vector < vector <v_3> > &trackSat, int sat, int site, int c){
    float res;
    v_3 a={coorSite[site].x-trackSat[sat][c].x, coorSite[site].y-trackSat[sat][c].y, coorSite[site].z-trackSat[sat][c].z};
    res=sqrt(a.x*a.x+a.y*a.y+a.z*a.z);

    return res;
}

float ele_a(vector < vector <v_3> > &trackSat, int sat, int c){
    float res;
    v_3 a={-trackSat[sat][c].x, -trackSat[sat][c].y, -trackSat[sat][c].z};

    v_3 b={4038377.4-trackSat[sat][c].x, 704809.7-trackSat[sat][c].y, 4870031.1-trackSat[sat][c].z};

    float nume=a.x*b.x+a.y*b.y+a.z*b.z;

    float deno=sqrt(a.x*a.x+a.y*a.y+a.z*a.z )*sqrt(b.x*b.x+b.y*b.y+b.z*b.z);

    res=acos(nume/deno) * 180.0 / PI;

    return res;
}

int min(int a, int b){
    int res;

    if(a<b){
        res=a;
    }else{
        res=b;
    }

    return res;
}

//on compte le nb de satellites a allouer qui sont en vu du site 
int nb_sat_in_view(vector <int> &res, vector< vector < vector <int> > > &antennaToSat, int site, int contact, defined_data its_data){
    int reslt=0;
    
    for(int i=0; i<its_data.nb_SVs; i++){
        int sat= i;

        if(res[i]==-1 && antennaToSat[site][sat][contact]==1){
            reslt++;
        }
        
    }
    return reslt;
}

//on compte le nb de sites qui sont en vu du satellite
int nb_site_in_view(vector< vector < vector <int> > > &antennaToSat, int sat, int contact, defined_data its_data){
    int res=0;
    
    for(int i=0; i<its_data.nb_ULS_s; i++){
        int site =i;

        if(antennaToSat[site][sat][contact]==1){
            res++;
        }
    }
                    //cout <<"coucou"<<endl;
    return res;
}

//on cherche le site qui a le minimum de satellites en vu
int which_ULS_s(vector <int> &res, vector <v_3> &coorSite, vector < vector <v_3> > &trackSat, vector <int> &tab_chosen_ULS_s, vector< vector < vector <int> > > &antennaToSat, int sat, int contact, defined_data its_data){
    int min=INT_MAX;
    int reslt=-1;

    
    for(int i=0; i<tab_chosen_ULS_s.size(); i++){
        int temp=nb_sat_in_view(res, antennaToSat, tab_chosen_ULS_s[i], contact, its_data);

        
        if(temp<=min){
            min=temp;
            reslt=tab_chosen_ULS_s[i];
        }else if(temp==min){
            nb_eph++;
            if(e_dist(coorSite, trackSat, sat, tab_chosen_ULS_s[i], contact) < e_dist(coorSite, trackSat, sat, tab_chosen_ULS_s[i], contact)){
                min=temp;
                reslt=tab_chosen_ULS_s[i];
            }
        }
    }
    if(reslt==-1){
        //printf("pas de minimum trouvé pour which_ULS_s \n");
    }
    return reslt;
    
};

list<int> priority(vector<int > &dd_sat, vector < vector <v_3> > &trackSat, vector< vector < vector <int> > > &antennaToSat, vector <int>tab_sat, int contact , defined_data its_data){
    list <int> res;
    int chosen_sat;
    int min=INT_MAX;

    for (int j=0; j<tab_sat.size(); j++){
        min=INT_MAX;
        for(int i=0; i<tab_sat.size(); i++){
            if(tab_sat[i]!=-1){
                int temp=nb_site_in_view(antennaToSat, tab_sat[i], contact, its_data);
                if( temp<=min){
                    min=temp;
                    chosen_sat=i;
                } 
            }
            
        }
        tab_sat[chosen_sat]=-1;
        
        if(j!=0){
            int temp=res.back(); 
            res.pop_back();

            if(nb_site_in_view(antennaToSat, temp, contact, its_data)==min){

                if(dd_sat[temp]<dd_sat[chosen_sat]){
                    res.push_back(chosen_sat);
                    res.push_back(temp);

                }
                else if(dd_sat[temp]>dd_sat[chosen_sat]){
                    res.push_back(temp);
                    res.push_back(chosen_sat);
                }
                else{

                    nb_eph++;
                    //elevation angle with regard to the european centre point (50°10'21'' N, 9°9'0''E)
                    //on cherche le plus petit car ele_a renvoit l'angle complémentaire à l'angle d'élévation
                    if(ele_a(trackSat, temp, contact)<ele_a(trackSat, chosen_sat, contact)){
                        res.push_back(temp);
                        res.push_back(chosen_sat);
                    }else{
                        res.push_back(chosen_sat);
                        res.push_back(temp);
                    }
                }
            }else{
                res.push_back(temp);
                res.push_back(chosen_sat);
            }
        }else{
            res.push_back(chosen_sat);
        }

    }
            return res;

    
}

//on attribue un site à chaque satellite suivant la prio du sat
vector<int> u_i(vector <int> &dd_sat, vector <v_3> &coorSite, vector < vector <v_3> > &trackSat, vector< vector < vector <int> > > &antennaToSat,  vector<int> s_i, int contact, defined_data its_data){
        int index = min(its_data.nb_SVs, its_data.nb_ULS_s*its_data.nb_Antennas);
        vector <int> res;
        
        for(int i=0; i < index; i++){
            res.push_back(-1);
        }

        list<int> s_i_prio=priority(dd_sat, trackSat, antennaToSat, s_i, contact, its_data);
        int s;
        vector <int> max_a(its_data.nb_ULS_s);
        for(int i=0; i < its_data.nb_ULS_s; i ++){
            max_a[i]=0;
        }

        for (int j=0; j<index; j++){

            s=s_i_prio.front();
            s_i_prio.pop_front();
            vector <int> tab_chosen_ULS_s;
            int site;
            if(j==11){
                for(int i=0; i < index; i++){
                }
            }

            for(int i=0; i<its_data.nb_ULS_s; i++){
                site=i;

                if(antennaToSat[site][s][contact]==1 && max_a[i] < its_data.nb_Antennas){

                    tab_chosen_ULS_s.push_back(i);
                }
            }

            int choice = which_ULS_s(res, coorSite, trackSat, tab_chosen_ULS_s, antennaToSat, s, contact, its_data);

            res[s]=choice;
            if(choice!=-1){
            max_a[choice]++;

            }

        }
        
        return res; 
    }




