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

void display_2Dmatrix(vector< vector <schedule> > &matrice){
    for (int i=0; i < matrice.size(); i++){
        cout << "satellite n° " << i << endl; 
        
        for (int k=0; k < matrice[i].size(); k++){
            cout << matrice[i][k].s << "---";
            cout << matrice[i][k].e << "  ";
        }
        cout << endl;
    }
}

void display_3Dmatrix(vector< vector < vector<int> > > &matrice){
    for (int i=0; i < matrice.size(); i++){
        cout << " utilisateur/antenne n° " << i << endl;

        for (int j=0; j < matrice[i].size(); j++){
            cout << "satellite n° " << j << endl; 
            
            for (int k=0; k < matrice[i][j].size(); k++){
                cout << matrice[i][j][k] << " ";
            }
            cout << endl;
        }
        cout << endl << endl;
    }
}

defined_data parse_contacts(list<float> &contacts, vector< vector <schedule> > &creneaux)
{
        /*
    cout << "please, specify your satellite constellation" << endl;
    cout << "horizon (maximum time slot): " << endl;
    cin >> horizon_c_in;
    cout << "number of users: " << endl;
    cin >> nbUsers_in;
    cout << "number of stations: " << endl;
    cin >> nbSites_in;
    cout << "number of satellites: " << endl;
    cin >> nbSatellites_in;
    cout << "number of antennas: " << endl;
    cin >> nbAntennas_in;*/

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
        //cout << "ligne " << str << endl;
        found=str_temp.find("horizon_c");
        //cout << found << endl;
    }
    //cout << str_temp << endl;
        file >> horizon_c_in;
    file >> str_temp;
        file >> nbUsers_in;
    //cout << nbUsers_in << endl;

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


    vector <schedule> tab;
    
    for (int i =0; i < its_data.nbSatellites; i++){
        
        creneaux.push_back(tab);

    }

    list<float> contacts_temp;
    const char* cfile_u = "data/cov20.cvaa";
    const char* output_file = "in_out/contacts.txt";

    string str;
    int c;
    int i;

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


    found=-1;
    while(found==-1)
    {
        file2 >> str;
        //cout << "ligne " << str << endl;
        found=str.find("NumberOfAssets");
        //cout << found << endl;
    }

    file2 >> nbSat ;
    nbSat--;
    cout << "nbsat " << nbSat <<endl;

    found=-1;
    while(found==-1)
    {
        file2 >> str;
        //cout << "ligne " << str << endl;
        found=str.find("NumberOfRegions");
        //cout << found << endl;
    }

    ////////////////////////////////////////

    cout << "---------------" << endl;

    found=-1;

    c=0;

    while(c < its_data.nbUsers)
    {

        found=-1;
        while(found==-1)
        {
            file2 >> str;
            //cout << "ligne1 " << str << endl;
            found=str.find("PointNumber");
            //cout << found << endl;
        }

        found=-1;
        while(found==-1)
        {
            file2 >> str;
            //cout << "ligne " << str << endl;
            found=str.find("NumberOfAccesses");
            //cout << found << endl;
        }
        file2 >> nbContact ;
        cout << "nbContact " << nbContact <<endl;

        s=0;
        while(s < its_data.horizon_c)
        {
            //cout << "contact n° "<< i <<endl;
            file2>>sat;
            file2>>s;
            file2>>e;
            
            if(s < its_data.horizon_c && sat < its_data.nbSatellites){
                if(e > its_data.horizon_c){
                    e = its_data.horizon_c; 
                }
                contacts_temp.push_back(s);
                contacts_temp.push_back(e);
                contacts_temp.sort();
                //cout << sat << endl;
                //cout << s << endl;
                //cout << e << endl;
                sat=sat-1; //décalage dans le fichier d'origine
                schedule slot=schedule{sat, s, e};
                creneaux[sat].push_back(slot);

                //cout << slot.sat << endl;
                //cout << slot.s << endl;
                //cout << slot.e << endl;
            }
            
            //cout << "contact : "<< e <<endl;
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
            //cout << "ligne " << sat << endl;
            file >> s;
            //cout << "ligne " << s << endl;
            file >> e;
            //cout << "ligne " << e << endl;
            file >> d;
            //cout << "ligne " << d << endl;

            if (s < its_data.horizon_c && sat < its_data.nbSatellites){
                if(e > its_data.horizon_c){
                    e = its_data.horizon_c;
                }
                contacts_temp.push_back(float(s));
                contacts_temp.push_back(float(e));
                contacts_temp.sort();
                
                
                schedule slot=schedule{sat, s, e};
                creneaux[sat].push_back(slot);
                //cout << slot.sat << endl;
                //cout << slot.s << endl;
                //cout << slot.e << endl;
            }
            
        }
        cout << "------ end reading ---------" << endl;

    }
    // si un sat n'a pas de creneaux alors on initialise à -1
    for(int i=0; i < creneaux.size(); i ++){
        if(creneaux[i].empty() ){
                creneaux[i].push_back(schedule{-1,-1,-1});
        }
    }

    //on enleve les doublons avant d ecrire dans le fichier
    int taille = contacts_temp.size();

    for(int it=0; it < taille ; it++){
        
        float temp1=contacts_temp.front();
        contacts_temp.pop_front();
        float temp2=contacts_temp.front();
        if(temp1!=temp2){
            contacts_temp.push_back(temp1);
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

    //cout << creneaux[sat].front().s << endl;
    //cout << creneaux[sat].front().e << endl;
    //cout << creneaux[sat].back().s << endl;
    //cout << creneaux[sat].back().e << endl;
its_data.nb_contacts=contacts.size();
    return its_data;

}



void parse_matrix_u(vector<float> contacts, vector< vector < vector <int> > > &userToSat, defined_data its_data){

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
        //cout << "ligne " << str << endl;
        found=str.find("NumberOfAssets");
        //cout << found << endl;
    }

    file >> nbSat ;
    nbSat--;
    cout << "nbsat " << nbSat <<endl;

    found=-1;
    while(found==-1)
    {
        file >> str;
        //cout << "ligne " << str << endl;
        found=str.find("NumberOfRegions");
        //cout << found << endl;
    }

    ////////////////////////////////////////

    cout << "---------------" << endl;

    found=-1;

    c=0;

    while(c < its_data.nbUsers)
    {
        found=-1;
        while(found==-1)
        {
            file >> str;
            //cout << "ligne1 " << str << endl;
            found=str.find("PointNumber");
            //cout << found << endl;
        }

        found=-1;
        while(found==-1)
        {
            file >> str;
            //cout << "ligne " << str << endl;
            found=str.find("NumberOfAccesses");
            //cout << found << endl;
        }
        file >> nbContact ;
        //cout << "nbContact " << nbContact <<endl;

        s=0;
        while(s < its_data.horizon_c)
        {
            //cout << "contact n° "<< i <<endl;
            file>>sat;
            file>>s;
            file>>e;
            //cout << "contact : "<< e <<endl;
            
            sat=sat-1;
            if(s < its_data.horizon_c && sat < its_data.nbSatellites){

                for (int i=0; i < contacts.size()-1; i++ ){
                    //cout << "coucou" << endl;
                    if(contacts[i] >= s && contacts[i] < e){
                        //cout << "update matrix" << endl;
                        userToSat[c][sat][i]=1;
                    }
                }
            }
            
        }


        cout << "user "<< c <<endl;
        c++;
    }
}



void parse_matrix_s(vector<float> contacts, vector< vector < vector <int> > > &antennaToSat, defined_data its_data){
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
            //cout << "ligne " << sat << endl;
            file >> s;
            //cout << "ligne " << s << endl;
            file >> e;
            //cout << "ligne " << e << endl;
            file >> d;
            //cout << "ligne " << d << endl;

            if(s < its_data.horizon_c && sat < its_data.nbSatellites){
                for (int j=0; j < contacts.size(); j++ ){
                    if(contacts[j] >= s && contacts[j] < e){
                        for(int k=0; k < its_data.nbAntennas; k++){
                            antennaToSat[i+k][sat][j]=1;
                            //cout << "update matrix" << endl;
                        }
                    }
                }
            }
            
        }
        cout << "------ end reading ---------" << endl;
    }
}












