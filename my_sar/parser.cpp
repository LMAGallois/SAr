#include "parser.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <list>

using namespace std;

void display_array(vector<float> &matrice){
    for (int i=0; i < matrice.size(); i++){
        cout << "contacts n° " << i << "--"<<matrice[i]<<  endl; 
        cout << endl;
    }
}

void display_3Dmatrix(vector< vector < vector<int> > > &matrice){
    for (int i=0; i < matrice.size(); i++){
        cout << " utilisateur/antenne n° " << i << endl;

        for (int j=0; j < matrice[i].size(); j++){
            cout << "satellite n° " << j << endl; 
            
            for (int k=987; k < 988; k++){
                cout << matrice[i][j][k] << " ";
            }
            cout << endl;
        }
        cout << endl << endl;
    }
}

defined_data get_data (){
    int horizon_c_in;
    int nbUsers_in;
    int nbSites_in;
    int nbSatellites_in;
    int nbAntennas_in;

    const char* cfile_conf = "in_out/config.txt";
    ifstream file(cfile_conf);
    if ( !file ) {
        cerr << "No such file: " << cfile_conf << endl;
        throw(-1);
    }
    
    string str_temp;

    int found=-1;
    while(found==-1)
    {
        file >> str_temp;
        found=str_temp.find("horizon_c");
    }
    file >> horizon_c_in;
    file >> str_temp;
    file >> nbUsers_in;
    file >> str_temp;
    file >> nbSites_in;
    file >> str_temp;
    file >> nbSatellites_in;
    file >> str_temp;
    file >> nbAntennas_in;

    defined_data its_data;
    its_data.horizon_c=horizon_c_in;
    its_data.nbUsers= nbUsers_in;
    its_data.nbSatellites=nbSatellites_in;
    its_data.nbSites= nbSites_in;
    its_data.nbAntennas= nbAntennas_in;

    return its_data;
}

void parse_contacts(vector<float> &contacts, defined_data its_data)
{
    list<float> contacts_temp;
    const char* cfile_u = "data/cov20.cvaa";
    const char* output_file = "in_out/contacts.txt";

    string str;
    int c;
    int nbSat=-1;
    int nbContact;
    int sat;
    float s;
    float e=0;

    ifstream file2(cfile_u);
    if ( !file2 ) {
        cerr << "No such file: " << cfile_u << endl;
        throw(-1);
    }
    
    cout << "------ start reading ---------" << endl;
    int found=-1;

    while(found==-1)
    {
        file2 >> str;
        found=str.find("NumberOfAssets");
    }

    file2 >> nbSat ;
    nbSat--;
    cout << "nbsat " << nbSat <<endl;

    found=-1;
    while(found==-1)
    {
        file2 >> str;
        found=str.find("NumberOfRegions");
    }

    ////////////////////////////////////////

    cout << "---------------" << endl;
    found=-1;
    c=0;

    while(c < 20)
    {
        found=-1;
        while(found==-1)
        {
            file2 >> str;
            found=str.find("PointNumber");
        }
        found=-1;
        
        while(found==-1)
        {
            file2 >> str;
            found=str.find("NumberOfAccesses");
        }

        file2 >> nbContact ;
        cout << "nbContact " << nbContact <<endl;

        for(int i=0; i < nbContact; i++)
        {
            file2>>sat;
            file2>>s;
            file2>>e;
            sat--;
            if(s < its_data.horizon_c && sat < its_data.nbSatellites){
                if(e > its_data.horizon_c){
                    e = its_data.horizon_c; 
                }
                contacts_temp.push_back(s);
                contacts_temp.push_back(e);
                contacts_temp.sort();
            }
        }

        cout << "user "<< c <<endl;
        c++;

    }// end of while(c<its_data.nbUsers)
/////////////////parse fenetres visibilités satellites et sites
    float d;
    const char* cfile;
    cout << "------ start reading site ---------" << endl;
    
    for (int i=0; i<its_data.nbSites;i++)
    {   
        switch (i) {
            case 0:
                cfile = "data/visiSVA.txt";
                break;
            case 1:
                cfile = "data/visiKOU.txt";
                break;
            case 2:
                cfile = "data/visiTHT.txt";
                break;
            case 3:
                cfile = "data/visiREU.txt";
                break;
            case 4:
                cfile = "data/visiNOU.txt";
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
        
        while(file >> sat)
        {
            file >> s;
            file >> e;
            file >> d;

            if (s < its_data.horizon_c && sat < its_data.nbSatellites){
                if(e > its_data.horizon_c){
                    e = its_data.horizon_c;
                }
                contacts_temp.push_back(float(s));
                contacts_temp.push_back(float(e));
                contacts_temp.sort();
            }
            
        }
        cout << "------ end reading ---------" << endl;

    }

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

    contacts_temp.sort();
    ofstream output(output_file);
    taille=contacts_temp.size();

    for(int it=0; it < taille ; it++){
        float temp= contacts_temp.front();
        output << temp << endl;
        contacts.push_back(temp);
        contacts_temp.pop_front();
    }
}

void parse_matrix_u(vector < float > &contacts ,vector< vector < vector <int> > > &userToSat, defined_data its_data){
    vector <int> c_t;
    vector < vector <int> > sat_t;

    for(int i=0; i < its_data.nbUsers ; i++){
        userToSat.push_back(sat_t);
        for(int j=0; j <its_data.nbSatellites; j++){
            userToSat[i].push_back(c_t);
            for (int k =0; k < contacts.size(); k++){
                userToSat[i][j].push_back(0);
            }
        }
    }
    
    const char* cfile_u = "data/cov20.cvaa";
    string str;
    int found=-1;
    int c=0;
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
    
    while(c < its_data.nbUsers){
        found=-1;
        
        while(found==-1){
            file >> str;
            found=str.find("PointNumber");
        }
        found=-1;
        
        while(found==-1){
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
            if(s < its_data.horizon_c && sat < its_data.nbSatellites){

                for (int i=0; i < contacts.size()-1; i++ ){
                            //cout << "coucou "<< contacts[i][0][0] <<endl;

                    if(contacts[i] >= s && contacts[i+1] <= e){
                        userToSat[c][sat][i]=1;
                    }
                }
            }
        }

        cout << "user "<< c <<endl;
        c++;
    }
}

void parse_matrix_s(vector < float> &contacts, vector< vector < vector <int> > > &antennaToSat, defined_data its_data){
    vector <int> c_t;
    vector < vector <int> > sat_t;

    for(int i=0; i < its_data.nbSites*its_data.nbAntennas ; i++){
        antennaToSat.push_back(sat_t);
        for(int j=0; j < its_data.nbSatellites; j++){
            antennaToSat[i].push_back(c_t);
            for (int k =0; k < contacts.size(); k++){
                antennaToSat[i][j].push_back(0);
            }
        }
    }

    string str;
    int found;
    int c;
    int i;
    int nbSat=-1;
    int nbContact;
    int sat;
    float s;
    float e;
    float d;
    const char* cfile;
    
    cout << "------ start reading site ---------" << endl;
    
    for (i=0; i<its_data.nbSites;i++)
    {
            
        switch (i) {
            case 0:
                cfile = "data/visiSVA.txt";
                break;
            case 1:
                cfile = "data/visiKOU.txt";
                break;
            case 2:
                cfile = "data/visiTHT.txt";
                break;
            case 3:
                cfile = "data/visiREU.txt";
                break;
            case 4:
                cfile = "data/visiNOU.txt";
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
        
        while(file >> sat)
        {
            file >> s;
            file >> e;
            file >> d;

            if(s < its_data.horizon_c && sat < its_data.nbSatellites){
                
                for (int j=0; j < contacts.size()-1; j++ ){
                    
                    if(contacts[j] >= s && contacts[j+1] <= e){
                        
                        for(int k=0; k < its_data.nbAntennas; k++){
                            antennaToSat[i*4+k][sat][j]=1;
                        }
                    }
                }
            }       
        }
        cout << "------ end reading ---------" << endl;
    }
}












