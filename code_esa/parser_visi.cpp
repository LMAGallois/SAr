#include "parser_visi.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <list>

using namespace std;

#define nbUsers 1400


void display_array(vector<v_3> &matrice){
    for (int i=0; i < matrice.size(); i++){
        cout << "contacts n° " << i << "--"<<matrice[i].x<<" , "<<matrice[i].y<<" , "<<matrice[i].z<<  endl; 
        cout << endl;
    }
}

void display_3Dmatrix(vector< vector < vector<int> > > &matrice){
    for (int i=0; i < matrice.size(); i++){

        for (int j=0; j < matrice[i].size(); j++){
            
            for (int k=0; k < matrice[i][j].size(); k+=10){
                cout << matrice[i][j][k] << "   ";
            }
        }
        cout << endl;
    }
}

void display_matrix(vector< vector<v_3> > &matrice){
    for (int i=0; i < matrice.size(); i++){
        cout << " satellite n°  " << i << endl;

        for (int j=0; j < matrice[i].size(); j++){
            cout << "temps" << j << endl; 
            
            cout << matrice[i][j].x << " , "<<matrice[i][j].y<<" , "<<matrice[i][j].z<<endl;
        }
        cout << endl ;
    }
}

int sat_present(int sat, vector<int> SVs_identity){
    int res=-1;
    int i=0;
    int not_found=1;

    while(i < SVs_identity.size() && not_found){
        if (sat==SVs_identity[i]){
            res=i;
            not_found=0;
        }
        i++;
    }
    
    return res;
}

defined_data parse_config( )
{
    string str_temp;
    int temp;
    int found=-1;
    
    int nb_Antennas;
    int nb_ULS_s;
    int horizon_c;
    int nb_SVs;
    vector<int> SVs_identity;
    int time_slot_sp3;

    const char* cfile_conf = "data/config.txt";
    ifstream file(cfile_conf);
    if ( !file ) {
        cerr << "No such file: " << cfile_conf << endl;
        throw(-1);
    }

    while(found==-1)
    {
        file >> str_temp;
        //cout << "ligne " << str << endl;
        found=str_temp.find("nb_Antennas");
        //cout << found << endl;
    }
    //cout << str_temp << endl;
    file >> nb_Antennas;
    file >> str_temp;
    file >> nb_ULS_s;
    file >> str_temp;
    file >> nb_SVs;
    file >> str_temp;
    file >> horizon_c;
    file >> str_temp;
    file >> time_slot_sp3;
    file >> str_temp;

    for(int i=0; i < nb_SVs; i++){
        file >> temp;
        SVs_identity.push_back(temp);
    }

    defined_data its_data;
    its_data.nb_Antennas=nb_Antennas;
    its_data.nb_ULS_s=nb_ULS_s;
    its_data.horizon_c=horizon_c;
    its_data.nb_SVs=nb_SVs;
    its_data.SVs_identity=SVs_identity;
    its_data.time_slot_sp3=time_slot_sp3;    

    return its_data;

}

void parse_matrix_s(vector< vector < vector <int> > > &antennaToSat, defined_data its_data){
    vector <int> c_t;
    vector < vector <int> > sat_t;

    for(int i=0; i < its_data.nb_ULS_s*its_data.nb_Antennas ; i++){
        antennaToSat.push_back(sat_t);
        for(int j=0; j < its_data.nb_SVs; j++){
            antennaToSat[i].push_back(c_t);
            for (int k =0; k < its_data.horizon_c/60; k++){
                antennaToSat[i][j].push_back(0);
            }
        }
    }

    string str;
    int found;
    int c;
    int sat;
    int i;
    float s;
    float e;
    float d;
    const char* cfile;
    
    cout << "------ start reading site ---------" << endl;
    
    for ( i=0; i<its_data.nb_ULS_s;i++)
    {
            
        switch (i) {
            case 0:
                cfile = "data/visiKOU.txt";
                break;
            case 1:
                cfile = "data/visiNOU.txt";
                break;
            case 2:
                cfile = "data/visiREU.txt";
                break;
            case 3:
                cfile = "data/visiSVA.txt";
                break;
            case 4:
                cfile = "data/visiTHT.txt";
                break;
                
                
            default:
                break;
        }
        
        
        ifstream file(cfile);
        if ( !file ) {
            cerr << "No such file: " << cfile << endl;
            throw(-1);
        }
        
        
        file.ignore(256,'\n');
        file.ignore(256,'\n');
        file.ignore(256,'\n');
        int l;
        while(file >> sat && sat <= its_data.SVs_identity.back())
        {
            file >> s;
            file >> e;
            file >> d;

            l=sat_present(sat, its_data.SVs_identity);
            if(s < its_data.horizon_c && l!=-1){
                for (int j=0; j < its_data.horizon_c/60; j++ ){
                    if( s<= j*60 && e >= (j+1)*60 ){
                        for(int k=0; k < its_data.nb_Antennas; k++){
                            int site=i*4;
                            antennaToSat[site+k][l][j]=1;
                        }
                    }
                }
            }

        }

        cout << "------ end reading ---------" << endl;
    }
    
    const char* cfile_conf = "data/antennaToSat.txt";
    ofstream file(cfile_conf);

    for(int i=0; i < its_data.nb_ULS_s*its_data.nb_Antennas ; i++){
        for(int j=0; j < its_data.nb_SVs; j++){
            for (int k =0; k < its_data.horizon_c/60; k++){
                file << antennaToSat[i][j][k];
            }
            file  << endl;       
        }
            file << endl<< endl;
    }    
}

void parse_matrix_u(vector< vector < vector <int> > > &userToSat, defined_data its_data){

    vector <int> c_t;
    vector < vector <int> > sat_t;

    for(int i=0; i < nbUsers ; i++){
        userToSat.push_back(sat_t);
        for(int j=0; j <its_data.nb_SVs; j++){
            userToSat[i].push_back(c_t);
            for (int k =0; k < its_data.horizon_c/60; k++){
                userToSat[i][j].push_back(0);
            }
        }
    }
    const char* cfile_u = "data/cov20.cvaa";
    string str;
    int found;
    int c;
    int i;

    int nbSat=-1;
    int nbContact;

    int sat;
    float s;
    float e;
    ifstream file(cfile_u);

    if ( !file ) {
        cerr << "No such file: " << cfile_u << endl;
        throw(-1);
    }

      found=-1;
    while(found==-1)
    {
        file >> str;
        found=str.find("NumberOfAssets");
    }

    file >> nbSat ;

    found=-1;
    while(found==-1)
    {
        file >> str;
        found=str.find("NumberOfRegions");
    }

    cout << "---------------" << endl;

    found=-1;

    c=0;   
    int l;
    while(c < nbUsers)
    {

        found=-1;
        while(found==-1)
        {
            file >> str;
            found=str.find("PointNumber");
        }


        found=-1;
        while(found==-1)
        {
            file >> str;
            found=str.find("NumberOfAccesses");
        }
        file >> nbContact ;

        for(int j=0; j < nbContact; j++)
        {
            file>>sat;
            file>>s;
            file>>e;
            sat=sat-1;
            l=sat_present(sat, its_data.SVs_identity);

            if(l!=-1 && s < its_data.horizon_c ){

                for (int j=0; j < its_data.horizon_c/60; j++ ){
                    if( s<= j*60 && e >= (j+1)*60 ){
                        userToSat[c][l][j]=1;
                    }
                }
            }

        }

        //cout << "user "<< c <<endl;
        c++;
    }
}










