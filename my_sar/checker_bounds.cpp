//dans un 1er temps, sans les sites on regarde si 24 sat couvrent tous les u
//dans un 2e temps, on regarde sans les sites pour 2 sat connectés à un u

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <fstream>
#include <limits.h>
#include "checker_bounds.h"
#include "parser.h"

using namespace std;

float check_bounds( vector < vector < vector < int> > > &userToSat, defined_data its_data, vector<float> &contacts, int nbSatCov){
    int res=0;
    const char* cfile2 = "in_out/checker_bounds.txt";
    ofstream file2(cfile2, ios::app);

    int taille = contacts.size();
    vector < int> nbSatVisi(taille);
    
    for (int i=0; i < taille; i++){

        for(int j=0; j < its_data.nbSatellites; j++){
            
            if( userToSat[0][j][i]==1){
                nbSatVisi[i]++;
            } 
        }
    }
    float temps=0;
    for(int i=0; i < taille-1; i++){
        if(nbSatVisi[i]<nbSatCov){
            res++;
            temps+=contacts[i+1]-contacts[i];
        }
    }

    file2<< res<<endl;
    file2<< temps<<endl;

    return temps;

}

void parse_contacts_user(vector<float> &contacts, int user, defined_data its_data)
{
    list<float> contacts_temp;
    const char* cfile_u = "data/cov20.cvaa";
    const char* output_file = "in_out/contacts_user.txt";

    string str;
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
    int int_temp;

    while(found ==-1)
    {

        found=-1;
        while(found==-1)
        {
            file2 >> str;
            found=str.find("PointNumber");
        }
        file2 >> int_temp;
        if(int_temp==user){
            found==1;
        }
    }

        found=-1;
        while(found==-1)
        {
            file2 >> str;
            found=str.find("NumberOfAccesses");
        }
        file2 >> nbContact ;
        cout << "nbContact " << nbContact <<endl;

        for(int i=0; i < nbContact; i++){
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
            //cout <<"coucou"<< s<< " "<< e<< endl;
        }

        cout << "user "<< user <<endl;
    // end of while(c<its_data.nbUsers)

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

    ofstream output(output_file);

   taille=contacts_temp.size();
    for(int it=0; it < taille ; it++){
        float temp= contacts_temp.front();
        output << temp << endl;
        contacts.push_back(temp);
        contacts_temp.pop_front();
    }    

}

void parse_matrix_u(vector < float > &contacts ,vector< vector < vector <int> > > &userToSat, defined_data its_data, int user){
    vector <int> c_t;
    vector < vector <int> > sat_t;

    userToSat.push_back(sat_t);
    for(int j=0; j <its_data.nbSatellites; j++){
        userToSat[0].push_back(c_t);
        for (int k =0; k < contacts.size(); k++){
            userToSat[0][j].push_back(0);
        }
    }
    
    const char* cfile_u = "data/cov20.cvaa";
    string str;
    int found;
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
    int int_temp;

    while(found ==-1){
        found=-1;
        
        while(found==-1){
            file >> str;
            found=str.find("PointNumber");
        }
        file >> int_temp;

        if(int_temp==user){
            found==1;
        }
    }
        found=-1;
        
        while(found==-1){
            file >> str;
            found=str.find("NumberOfAccesses");
        }

        file >> nbContact ;

        for(int j=0; j < nbContact; j++){
            file>>sat;
            file>>s;
            file>>e;
            
            sat=sat-1;
            if(s < its_data.horizon_c && sat < its_data.nbSatellites){

                for (int i=0; i < contacts.size()-1; i++ ){

                    if(contacts[i] >= s && contacts[i+1] <= e){
                        userToSat[0][sat][i]=1;
                    }
                }
            }
        }
}

void get_upper_bound (int nbSatCov) {
    vector<float> contacts;
    defined_data its_data; 
    vector< vector < vector<int> > > userToSat;
    list<schedule> plan;
    int taille;
    float nv=0;
    its_data=get_data();

    for(int i=0; i < its_data.nbUsers; i++){
        parse_contacts_user(contacts, i, its_data);
        //    display_array(contacts);

        parse_matrix_u(contacts, userToSat, its_data, i);
           // display_3Dmatrix(userToSat);
           //nb sat limite --> 5 (4-> ca passe, tjrs couverts mais 5 en parti, et 6 sans doute jamais couverts ou encore moins )
        float temp_nv=check_bounds( userToSat, its_data, contacts, nbSatCov);
        cout << temp_nv<<endl;
        nv=nv+temp_nv;

        contacts.clear();
        userToSat.clear();
    }
    cout << nv<<endl;
    float couv= (((its_data.nbUsers*its_data.horizon_c )- nv)*100) /(its_data.nbUsers*its_data.horizon_c );
    cout << "taux de couverture : "<< couv <<"%"<<endl;
    
}