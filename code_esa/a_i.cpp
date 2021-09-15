#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include "a_i.h"
#include <limits.h>
 
using namespace std;

vector<int> a_i(vector < vector <v_3> > &trackSat,vector <int> former_site, vector<int> former_sat,vector<int> _a_1, vector<int> _a_2, vector <int> &avail_a,vector<int > &cd_sat, vector <int> &dd_sat, int c, defined_data its_data){
    int index = min(its_data.nb_SVs, its_data.nb_ULS_s*its_data.nb_Antennas);
    int al_taken=0;
    vector<int> res;

    for(int i=0; i < index; i++){
        res.push_back(-1);
    }


//les antennes affectées au tour d'avant qui vont bien toujours, on les garde
    if(_a_2.front()!=-1){
        
        for(int i=0; i < index; i++){
            if(_a_1[i]!=-1){
            if(former_site[i]!=-1 && former_site[i]==_a_1[i] && avail_a[_a_1[i]*4+_a_2[i]]==0){
                
                al_taken=0;

                for(int j=0; j < index; j++){
                    if(_a_1[i]== _a_1[j] && _a_2[i]==res[j]){
                        al_taken=1;
                    }
                }
                        
                if(al_taken!=1){

                    
                    res[i]=_a_2[i];

                }        

            }
            }
        }


     //si il y a des antennes de libres pour les sites qui n'en n'ont pas encore, alors elles sont affectées, en priorité celles avec la plus longue duree de connection pour essayer de satisfaire le plus possible la contrainte des 600s
        
        for(int i=0; i < index; i++){
            if(_a_1[i]!=-1 && res[i]==-1){
                int nb=-1;
                for(int j=0; j < its_data.nb_Antennas; j++){
                    if( avail_a[_a_1[i]*4+j] && (dd_sat[i] ==0 || dd_sat[i] >=60)){
                            res[i]=j;
                            avail_a[_a_1[i]*4+j]=0;
                        
                    }
                }
                
            }
        }


//toutes les antennes affectées précedemment qui ne peuvent pas reservir sont de nouveau libres !!!!!!!si elles ont ete connectees pour 600s
//les antennes pas affectées lors de ce slot sont considérées comme disponibles meme si elles ont été connectées à un sat précedemment
        for (int i=0; i<its_data.nb_ULS_s; i++){
            for(int j=0; j < its_data.nb_Antennas; j++){
                if( avail_a[i*4+j]==0 ){
                    al_taken=0;
                    for(int k=0; k < index; k++){
                        if(_a_1[k]==i && res[k]==j){
                            al_taken=1;
                        }   
                    }

                    if(al_taken!=1){
                        avail_a[i*4+j]=1;
                    }
                }
            }
        
        } 



//maintenant on n'a pas d autres choix que de déconnecter une antenne pas déjà affiliée lors de ce slot de temps   
//raisonner avec sites necessaires , antennes non libres non deja affectées lors de ce slot de temps peuvent etre utilisées --> handover


        int y=0;
        while(y < index){
            if(res[y]==-1){ 

                    int chosen_j=-1;
                    int max=INT_MIN;
                    int cd_time;
                    int j;
                                            if(_a_1[y]!=-1){

                    for( j=0; j< its_data.nb_Antennas; j++){
                        cd_time=0;

                        if(avail_a[_a_1[y]*4+j] ){
                            int i;
                            for( i =0; i< index; i++){
                                if(former_site[i]!= -1 && former_site[i]==_a_1[y] && _a_2[i]==j ){
                                    cd_time=cd_sat[former_sat[i]];
                                    
                                }

                            }
     
                            if( cd_time > max){
                                max=cd_time;
                                chosen_j=j;

                            }else if(cd_time == max){
                                int temp2= ele_a(trackSat, former_sat[i], c);
                                //ele_a renvoit l angle complémentaire de l'angle d'elevation donc on cherche le plus grand
                                if(temp2 > ele_a(trackSat, chosen_j, c)){
                                    max=cd_time;
                                    chosen_j=j;
                                }
                            }
                        }       
                    }
                    //handover
                    res[y]=chosen_j;
                    avail_a[_a_1[y]*4+chosen_j]=0;
                }
            }
            y++;
        }
      

//init 
    }else{
 //si il y a des antennes de libres pour les sites qui n'en n'ont pas encore, alors elles sont affectées
        for(int i=0; i < index; i++){
            for(int j=0; j < its_data.nb_Antennas; j++){

                if(res[i]==-1 && avail_a[_a_1[i]*4+j]){

                    res[i]=j;
                    avail_a[_a_1[i]*4+j]=0;
                }
            }
        }

    }

    return res;
}
