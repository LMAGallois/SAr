#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <fstream>
#include <limits.h>
#include "parser_sites.h"
#include "parser.h"

using namespace std;    

void parse_contacts_sites(vector< vector < vector <float> > > &contacts, defined_data its_data)
{
    list<float> contacts_temp;
    const char* output_file = "in_out/contacts.txt";

    string str;
    int c;
    int nbSat=-1;
    int nbContact;
    int sat;
    float s;
    float e=0;
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
    contacts_temp.pop_front();
    contacts_temp.sort();
    // contcats_temp comprend tous les temps
    
    int a;
    int b;

    int i=0;
    int j=0;
    int k=0;

    float memo600=0;
    float memo60=0;

    int al1=0;
    int al2=0;
    while(contacts_temp.size()!=0){

        a=contacts_temp.front();

        contacts_temp.pop_front();

        al1=0;
        al2=0;
        if(k==0){
            vector < vector < float > > struct_temp;
            contacts.push_back(struct_temp);
            vector < float > list_temp;
            contacts[0].push_back(list_temp);
            memo600=a;
            memo60=a;
            
        }
        if(a-memo600 >= 600){
            vector < vector < float > > struct_temp;
            contacts.push_back(struct_temp);
            i++;
            vector < float > list_temp;
            contacts[i].push_back(list_temp);
            j=0;
            contacts[i][j].push_back(a);
            memo600=a;
            al1=1;
        } 
        if(!al1 && a-memo60 >=60){
            vector < float > list_temp;
            contacts[i].push_back(list_temp);
            j++;
            contacts[i][j].push_back(a);
            memo60=a;
            al2=1;

        }
        if(!al1 && !al2){
            contacts[i][j].push_back(a);
            k++;
        }
    }

//ecriture dans le fichier in_out/contacts
    ofstream output(output_file);

    taille=contacts.size();
    for(int it=0; it < taille ; it++){
        int taille2=contacts[it].size();

        for(int it2=0; it2 < taille2 ; it2++){
            int taille3=contacts[it][it2].size();
            
            for(int it3=0; it3 < taille3 ; it3++){
                float temp= contacts[it][it2][it3];
                output << temp << endl;
            }
        }
    }

    cout << "------ end parsing ---------" << endl;
}

void parse_matrix_u(vector < vector <vector <float> > > &contacts ,vector< vector < vector <int> > > &userToSat, defined_data its_data){
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

                    if(contacts[i][0][0] >= s && contacts[i+1][0][0] <= e){
                        userToSat[0][sat][i]=1;
                    }
                }
            }
        }

        cout << "user "<< c <<endl;
        c++;
    }
}

void parse_matrix_s(vector < vector <vector <float> > > &contacts, vector< vector < vector <int> > > &antennaToSat, defined_data its_data){
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
                    
                    if(contacts[j][0][0] >= s && contacts[j+1][0][0] <= e){
                        
                        for(int k=0; k < its_data.nbAntennas; k++){
                            antennaToSat[i+k][sat][j]=1;
                        }
                    }
                }
            }
            
        }
        cout << "------ end reading ---------" << endl;
    }
}

void parse_matrix_s_precise(vector < vector <vector <float> > > &contacts, vector< vector < vector <int> > > &antennaToSat, defined_data its_data){
    vector <int> c_t;
    vector < vector <int> > sat_t;

    for(int i=0; i < its_data.nbSites*its_data.nbAntennas ; i++){
        antennaToSat.push_back(sat_t);
        for(int j=0; j < its_data.nbSatellites; j++){
            antennaToSat[i].push_back(c_t);
            for (int k =0; k < 2854; k++){
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
                
                for (int j=0; j < 2854-1; j++ ){
                for (int l=0; l < 2854-1; l++ ){
                for (int m=0; m < 2854-1; m++ ){

                    if(contacts[j][l][m] >= s && contacts[j+1][l][m] <= e){
                        
                        for(int k=0; k < its_data.nbAntennas; k++){
                            antennaToSat[i+k][sat][j]=1;
                        }
                    }
                }
                }
                }
            }
            
        }
        cout << "------ end reading ---------" << endl;
    }
}

void get_parser_sites (vector< vector < vector<float> > > &contacts_sites,vector< vector < vector <int> > > &userToSat,vector< vector < vector <int> > >&antennaToSat, vector< vector < vector <int> > >&antennaToSatP, defined_data its_data ){
    its_data=get_data();
    parse_contacts_sites(contacts_sites, its_data);
    parse_matrix_s(contacts_sites, antennaToSat, its_data);
    parse_matrix_u(contacts_sites, userToSat, its_data);
    parse_matrix_s_precise(contacts_sites, antennaToSatP, its_data);
}
    
