#include "parser_eph2.h"
#include "parser_eph.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <math.h>

using namespace std;

#define nbUsers 1400

#define PI 3.14159265

v_3 trans_coords_u (float lat, float longi, float alt){
    // (lat, lon) in WSG-84 degrees and altitude in meters
    int R = 6378137;
    /*float f_inv = 298.257224;
    float f = 1.0 / f_inv;

    float cosLat = cos(lat * PI / 180);
    float sinLat = sin(lat * PI / 180);
    float cosLong = cos(longi * PI / 180);
    float sinLong = sin(longi * PI / 180);

    float c = 1 / sqrt(cosLat * cosLat + (1 - f) * (1 - f) * sinLat * sinLat);
    float s = (1 - f) * (1 - f) * c;

    float x = (R*c + alt) * cosLat * cosLong;
    float y = (R*c + alt) * cosLat * sinLong;
    float z = (R*s + alt) * sinLat;
    v_3 point={x,y,z};*/
    float Lat=lat;
    float Long= longi;
    float x=R*cos(Lat)*cos(Long);
    float y=R*cos(Lat)*sin(Long);
    float z = R*sin(Lat);

    v_3 point={x,y,z};
    
    return point;   
}

void parse_coords_u(vector<v_3> &userCoords, defined_data its_data){

    const char* cfile_u = "data/cov20.cvaa";
    string str;
    int found;
    int nbSat;
    int c;
    float x;
    float y;
    float z;
    ifstream file(cfile_u);

    if ( !file ) {
        cerr << "No such file: " << cfile_u << endl;
        throw(-1);
    }

    found=-1;

    while(found==-1){
        file >> str;
        found=str.find("NumberOfAssets");
    }

    file >> nbSat ;
    found=-1;

    while(found==-1){
        file >> str;
        found=str.find("NumberOfRegions");
    }

    found=-1;
    c=-1;   
    int l;

    while(c < nbUsers-1){
        found=-1;
        while(found==-1)
        {
            file >> str;
            found=str.find("PointNumber");
        }
        file>>c;
        file>>str,
        file>>x;
        file>>str;
        file>>y;
        file>>str;
        file>>z;
        v_3 point=trans_coords_u(x,y,z);
        userCoords[c]=point;

        //cout << "user "<< c <<endl;
    }
            cout << "end of parse_user_coords " <<endl;
}

void get_coords_sat(vector< vector <v_3> > &trackSat, vector<v_3> &userCoords, vector< vector < vector <int> > > &userToSat, defined_data its_data){
    for(int i=0; i<its_data.horizon_c/60; i++){
        
        for(int j=0; j <its_data.nb_SVs;j++){
            float x_av=0;
            float y_av=0;
            float z_av=0;
            int nb=0;
            for(int k=0; k< nbUsers; k++){

                if(userToSat[k][j][i]==1){
                    x_av+=userCoords[k].x;
                    y_av+=userCoords[k].y;
                    z_av+=userCoords[k].z;
                    nb++;
                }
            }

            float x=x_av/nb;
            float y=y_av/nb;
            float z=z_av/nb;
            v_3 coords={x,y,z};
            trackSat[j][i]=coords;

        }
    }
}
