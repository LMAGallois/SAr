//un renumérote les satellites dans l'ordre de leur indice (PE01...PE30) en commencant par 0 
//pour connaitre la correspondance entre numero de site et nom de site, voir le fichier sites.txt dans data

#include <fstream>
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <list>
#include <vector>
#include "parser_eph.h"
#include "parser_visi.h"
#include <cstring>

using namespace std;
#define PI 3.14159265


void parse_site(vector<v_3> &coorSite, defined_data its_data){
    const char* cfile_u = "data/sites.txt";
    ifstream file(cfile_u);
    int found=-1;
    string str_temp;
    float x;
    float y;
    float z;

    if ( !file ) {
        cerr << "No such file: " << cfile_u << endl;
        throw(-1);
    }

    while(found==-1)
    {
        file >> str_temp;
        found=str_temp.find("coordinates");
    }

    for (int i=0; i < its_data.nb_ULS_s; i ++){
        file >> str_temp;
        file >> x;
        file >> y;
        file >> z;
        coorSite.push_back({x,y,z});
    }
            cout << "end of parse_sites " <<endl;

}

void parse_eph(vector < vector <v_3> > &trackSat, defined_data its_data, const char *path ){
    string str;
    int found;
    int j;
    int deb=0;

    float x=0;
    float y=0;
    float z=0;
    float clk=0;
    float heure;
    float minutes;
    int temps;
    vector<v_3> s;

    for(int j=0; j < its_data.nb_SVs; j++){
        trackSat.push_back(s);

        for(int i=0; i < its_data.horizon_c/60; i++){
            trackSat[j].push_back({0,0,0});
        }
    }

    const char* cfile;
    char chemin[100];
    for(int i=0; i< 11; i++){
        for(int t=0; t< 100; t++){
            chemin[t]='\0';
        }        switch (i) {
            case 0:
                strcpy(chemin, path);
                strcat(chemin, "/0.SP3");
                cfile =chemin;
                //cout <<chemin<<endl;
                //cout <<path<<endl;
                break;
            case 1:
                strcpy(chemin, path);
                strcat(chemin, "/1.SP3");
                //cout <<chemin<<endl;
                cfile =  chemin;
                break;
            case 2:
                strcpy(chemin, path);
                strcat(chemin, "/2.SP3");
                cfile =  chemin;
                break;
            case 3:
                strcpy(chemin, path);
                strcat(chemin, "/3.SP3");
                cfile =  chemin;
                break;
            case 4:
                strcpy(chemin, path);
                strcat(chemin, "/4.SP3");
                cfile =  chemin;
                break;
            case 5:
                strcpy(chemin, path);
                strcat(chemin, "/5.SP3");
                cfile = chemin;
                break;
            case 6:
                strcpy(chemin, path);
                strcat(chemin, "/6.SP3");
                cfile = chemin;
                break;
            case 7:
                strcpy(chemin, path);
                strcat(chemin, "/7.SP3");
                cfile =  chemin;
                break;
            case 8:
                strcpy(chemin, path);
                strcat(chemin, "/8.SP3");
                cfile =  chemin;
                break;
            case 9:
                strcpy(chemin, path);
                strcat(chemin, "/9.SP3");
                cfile = chemin;
                break;
            case 10:
                strcpy(chemin, path);
                strcat(chemin, "/10.SP3");
                cfile =  chemin;
                break;

                
            default:
                break;
        }

            ifstream file(cfile);
        if ( !file ) {
            cerr << "No such file: " << cfile << endl;
            throw(-1);
        }
        

        found=-1;
        while(found==-1){
            file >> str;
            found=str.find("PCV");
        }

        for(int index=0; index < (((24*3600)/its_data.time_slot_sp3)-1); index++)
        {
            found=-1;
            while(found==-1)
            {
                file >> str;
                found=str.find("*");
                if (found != -1){

                    file >> str;
                    file >> str;
                    file >> str;
                    file >> heure; 
                    file >> minutes;

                    if(!deb){
                        temps=heure*3600+minutes*60;
                        deb=1;
                    //}else if(deb && heure==0 && minutes ==0){
                    //    temps=24*3600;
                    }else{
                        temps=heure*3600+minutes*60;
                    }
                }
            }

            j=0;
            while(j < its_data.nb_SVs)
            {
                found=-1;
                while(found==-1)
                {
                    file >> str;
                    found=str.find("PE");
                }

                file>>x;
                file>>y;
                file>>z;
                file >>clk;
                
                int borne_inf;
                if(temps==0 && i==0){
                    borne_inf=temps/60;
                }else{
                    borne_inf=temps/60-its_data.time_slot_sp3/2/60;
                }
                int borne_sup=temps/60+its_data.time_slot_sp3/2/60+1;
                                   // cout <<index<<"  "<<borne_inf<<"  " <<borne_sup<<endl;

                if(borne_sup>its_data.horizon_c/60){
                    borne_sup=its_data.horizon_c/60;
                }
                if(borne_inf<its_data.horizon_c/60){
                    for(int k=borne_inf; k < borne_sup; k++){
                        trackSat[j][k+(i*1440)]={x,y,z};
                    }
                }         
                j++;
            }
        }
    }
    
    

    cout << "while fini" << endl;

    cout <<"----------------end parse eph----------------------------"<<endl;
}

float ele_a_20(v_3 sat , v_3 on_earth){

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

void get_visi(vector < vector <v_3> > &trackSat, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <int> > > &u_or_aToSat){
    vector <vector <int> > temp;
    vector <int> temp2;

    for(int j=0; j < coordsOnEarth.size(); j++){
        u_or_aToSat.push_back(temp);
        for(int k=0; k< its_data.nb_SVs; k++){
            u_or_aToSat[j].push_back(temp2);
            for(int i=0; i < its_data.horizon_c/60; i++){
                v_3 a=trackSat[k][i];
                v_3 b=coordsOnEarth[j];

                if(ele_a_20(a,b)){
                    u_or_aToSat[j][k].push_back(1);
                }else{
                    u_or_aToSat[j][k].push_back(0);
                }
            }
        }
        //cout <<"coucou"<<j<<endl;
    }
    cout <<"-------------------------get visi fin--------------------------------"<<endl;

}