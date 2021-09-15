/* L'algoritme est expliqué dans l'overleaf en partie 1
Il est donc découpé en trois parties, 
s_i : allocation des satellites
u_i : allocation des sites
a_i : allocationdes antennes
les fichiers vincenty ne sont pas utilisés
les fichiers parse_eph, parse_eph2 et parse_site sont utilisés pour parser les données

to run the code : 

make clean
make
./paper_schedule arg1 arg2

arg1 : date de l'instance (regarder dans data les differentes possibilités)
arg2 : pour checker, 
 - couverture de un satellite sans prendre en compte les handovers : 0
 - couverture de un satellite avec prise en compte des handovers : 1
 - couverture de deux satellites sans prise en compte des handovers : 2
 - couverture de deux satellites avec prise en compte des satellites : 3

Vous trouverez dans data/config.txt les données de configuration à changer si nécessaire

 */

//on prend pas en compte les contraintes MCD or NDRR (que 600s de connection duration)
//correspondance entre nb sat utilisé dans fonctions allocation (de 0 à nbSat) et nb sat constellation galiléo

#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include "s_i.h"
#include "u_i.h"
#include "a_i.h"
#include "parser_visi.h"
#include "parser_eph2.h"
#include "parser_eph.h"

#define nbUsers 1400

int check_last_alloc(vector< vector < vector <int> > > &antennaToSat, vector<int> tab_sat, vector<int> tab_site, int c, defined_data its_data){
    int still_visi=1;

// on check again les visibilités entre satellites et sites
    for(int i=0; i < tab_sat.size(); i++){
        if( tab_site[i]==-1 || (tab_site[i]!=-1 && antennaToSat[tab_site[i]][tab_sat[i]][c]==0)){
            still_visi=0;
        }
    }    

    return still_visi;
}

int main(int argc, char *argv[]){
    if(argc != 3){
        cerr<<"nombre de paramètres invalides"<<endl;
    }
    const char* path=argv[1];
    int checker =atoi(argv[2]);
    defined_data its_data=parse_config();

    vector<v_3> userCoords(nbUsers);
    parse_coords_u(userCoords, its_data);

    vector <v_3> coorSite;
    parse_site(coorSite, its_data);

    vector< vector <v_3> > trackSat;

    //cout <<its_data.nb_SVs<<endl;
    parse_eph(trackSat, its_data, path);
       //display_matrix(trackSat);

    vector< vector < vector <int> > > userToSat;
    get_visi(trackSat, userCoords, its_data, userToSat);
    vector< vector < vector <int> > > antennaToSat;
    get_visi(trackSat, coorSite, its_data, antennaToSat);
    //display_3Dmatrix(antennaToSat);
    
    const char* cfile_conf = "data/plan.txt";
    ofstream output(cfile_conf);
    int nb_slot=its_data.horizon_c/60;
    int index = min(its_data.nb_SVs, its_data.nb_ULS_s*its_data.nb_Antennas);
    vector< vector<int> > _a(3); 
    vector<int> vec_temp(index);
    vector <int> dd_sat;
    vector <int> cd_sat;
    vector <int> avail_a;
    vector <int> former_sat;
    vector <int> former_site;
    vector <int> former_a;
    int nb_handover=0;
    
    for(int i =0; i < its_data.nb_SVs; i++){
        dd_sat.push_back(0);
        cd_sat.push_back(0);
    }

    for(int i=0; i< its_data.nb_Antennas*its_data.nb_ULS_s; i++){
        avail_a.push_back(1);
    } 
    
    for(int i=0; i< index; i++){
        former_sat.push_back(-1);
        former_site.push_back(-1);
        former_a.push_back(-1);
    }

    for(int i=0; i < 3; i++){
        _a.push_back(vec_temp);
        for(int j=0; j < index; j++){
            _a[i].push_back(-1);
        }
    }

    //--------------new alloc
    //init
    _a[0]=s_i(cd_sat, dd_sat ,0, its_data, trackSat);

    _a[1]=u_i(dd_sat,coorSite, trackSat, antennaToSat, _a[0], 0, its_data);


    _a[2]=a_i(trackSat, former_site, former_sat, _a[1], _a[2], avail_a, cd_sat, dd_sat, 0, its_data);  

    for(int i=0; i < index; i++){
        cd_sat[i]+=1;
    }

    int mini=INT_MAX;
    int maxi=INT_MIN;
    float average=0;
    float nb=0;

    for (int c=0; c < nb_slot; c++){
        cout << "slot " << c << endl;

        for(int i=0; i < index; i++){
            former_sat[i]=_a[0][i];
            former_site[i]=_a[1][i];
            former_a[i]=_a[2][i];
        }


        if(check_last_alloc(antennaToSat, _a[0], _a[1], c, its_data)==0){

            _a[0]=s_i( cd_sat, dd_sat, c, its_data, trackSat);

            _a[1]=u_i(dd_sat, coorSite, trackSat, antennaToSat, _a[0], c, its_data);

            _a[2]=a_i(trackSat, former_site, former_sat, _a[1], _a[2], avail_a, cd_sat, dd_sat, c, its_data);  
            
            for(int i=0; i < index; i++){
                if(former_site[i]!=-1 && _a[1][i]!=-1 && former_site[i]==_a[1][i] && former_a[i]==_a[2][i]){
                    cd_sat[i]+=1; 
                    dd_sat[i]=0;         
                }else if(former_site[i]!=-1 && _a[1][i]!=-1 ){
                    cd_sat[i]=0;
                    dd_sat[i]=0;
                }else{
                    dd_sat[i]+=1;
                }
            }
        }else{

            for(int i=0; i < index; i++){
                if(former_site[i]!=-1 ){
                    cd_sat[i]+=1;
                    dd_sat[i]=0;
                }else{
                    dd_sat[i]+=1;
                }
            }
        }

        //ecriture de l'allocation dans le fichier output
        output << "numéro de slot : "<< c << endl;
        for (int i=0; i < _a[0].size()-1; i++){
            output << _a[0][i];
            output << " "; 
        }
        output << _a[0][_a[0].size()-1] <<endl;

        for (int i=0; i < _a[1].size()-1; i++){
            output << _a[1][i] ;
            output << " "; 
        }
        output << _a[1][_a[1].size()-1] <<endl;

        for (int i=0; i < _a[2].size()-1; i++){
            output << _a[2][i] ;
            output << " "; 
        }
        output << _a[2][_a[2].size()-1] <<endl;
        output << "---------------------------------- "<< endl;

        nb=0;
        int nb_sat=0;
        int sat_wout_h=0;

    switch(checker){
            case 0: 
                for(int k=0; k < nbUsers; k++){        
                    nb_sat=0;

                    for(int j=0; j < index; j++){
                        
                        if( _a[1][j]!=-1){
                            if(userToSat[k][_a[0][j]][c]==1){
                                nb_sat++;
                            }
                        }

                    }

                    if(nb_sat >= 1){
                        nb+=60;
                    }
            
                }
                break;


            case 1:

                for(int k=0; k < nbUsers; k++){        
                    sat_wout_h=0;
                    nb_sat=0;

                    for(int j=0; j < index; j++){
                        
                        if(c==0 && _a[1][j]!=-1 && userToSat[k][_a[0][j]][c]==1){
                            nb_sat++;

                        }
                        if( c!=0  && _a[1][j]!=-1 && userToSat[k][_a[0][j]][c]==1){
                            nb_sat++;
                            if(_a[1][j]==former_site[j] && _a[2][j]==former_a[j]){
                                sat_wout_h++;
                            }
                        }
                        if(k==0 && c!=0 && (_a[1][j]!=former_site[j] || _a[2][j]!=former_a[j] || (_a[1][j]!=former_site[j]!=-1 && _a[1][j]==-1))){
                            nb_handover++;
                        }

                    }
                    
                    if(c==0){
                        if(nb_sat>=1){
                            nb+=60;
                        }
                    }else{
                        if(nb_sat>=1){
                            if(sat_wout_h>=1){
                                nb+=60;
                            }
                        }
                        
                    } 
                
                }
                break;

            case 2:

                for(int k=0; k < nbUsers; k++){        
                    nb_sat=0;

                    for(int j=0; j < index; j++){
                        
                        if( _a[1][j]!=-1){
                            if(userToSat[k][_a[0][j]][c]==1){
                                nb_sat++;
                            }
                        }

                    }

                    if(nb_sat >= 2){
                        nb+=60;
                    }
                
                }
                break;

            case 3:

                for(int k=0; k < nbUsers; k++){        
                    sat_wout_h=0;
                    nb_sat=0;

                    for(int j=0; j < index; j++){
                        
                        if(c==0 && _a[1][j]!=-1 && userToSat[k][_a[0][j]][c]==1){
                            nb_sat++;

                        }
                        if( c!=0  && _a[1][j]!=-1 && userToSat[k][_a[0][j]][c]==1){
                            nb_sat++;
                            if(_a[1][j]==former_site[j] && _a[2][j]==former_a[j]){
                                sat_wout_h++;
                            }
                        }
                        if(k==0 && c!=0 && (_a[1][j]!=former_site[j] || _a[2][j]!=former_a[j] || (_a[1][j]!=former_site[j]!=-1 && _a[1][j]==-1))){
                            nb_handover++;
                        }

                    }
                    
                    if(c==0){
                        if(nb_sat>=2){
                            nb+=60;
                        }
                    }else{
                        if(nb_sat>=2){
                            if(sat_wout_h>=2){
                                nb+=60;
                            }
                        }
                        
                    } 
            
                }
                break;

        }

        if(nb>maxi){
        maxi=nb;
        }
        if(nb < mini){
        mini=nb;
        }

        average+=nb;

    }
    float total=(average*100)/(1400*its_data.horizon_c);
    
    cout << "max" << maxi << endl;
    cout << "min "<< mini << endl;
    cout << "moyenne "<< average/nb_slot << endl;
    cout << " couverture totale " << total <<"%"<<endl;
    cout << "nb handover " << nb_handover <<endl;

    return 0;
}