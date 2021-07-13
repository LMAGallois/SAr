#include "parser_alone.h"
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

void parse_contacts()
{
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

cout << "nb users " << its_data.nbUsers << endl;
cout << "nb sat " << its_data.nbSatellites << endl;
cout << "nb sites " << its_data.nbSites << endl;
cout << "nb ant " << its_data.nbAntennas << endl;
cout << "horizon" << its_data.horizon_c << endl;
    
    vector< vector <schedule> > creneaux;
    vector <schedule> tab;
    
    for (int i =0; i < its_data.nbSatellites; i++){
        
        creneaux.push_back(tab);

    }

    list<float> contacts_temp;
    const char* cfile_u = "data/cov20.cvaa";
    const char* output_file = "in_out/contacts.txt";
    const char* output_file2 = "in_out/creneaux.txt"; 

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
    ofstream output2(output_file2);

    int taille_contacts=contacts_temp.size();
    for(int it=0; it < taille_contacts ; it++){
        float temp= contacts_temp.front();
        output << temp << endl;
        contacts_temp.pop_front();
    }
    output << taille_contacts << endl;

    cout << "nb contacts" << taille_contacts << endl;

    int taille_cr=creneaux.size();
    for(int it=0; it < taille_cr ; it++){
        for (int j=0; j < creneaux[it].size(); j ++){
            schedule temp= creneaux[it][j];
            output2 << temp.sat << endl;
            output2 << temp.s << endl;
            output2 << temp.e << endl;
        } 
    }

}



void parse_matrix_u(){
vector< vector < vector <int> > > userToSat;

 defined_data its_data;
int horizon_c_in;
    int nbUsers_in;
    int nbSites_in;
    int nbSatellites_in;
    int nbAntennas_in;

    const char* cfile_conf = "in_out/config.txt";
    ifstream file1(cfile_conf);
    if ( !file1 ) {
        cerr << "No such file: " << cfile_conf << endl;
        throw(-1);
    }
    
    string str_temp;

    int found=-1;
    while(found==-1)
    {
        file1 >> str_temp;
        //cout << "ligne " << str << endl;
        found=str_temp.find("horizon_c");
        //cout << found << endl;
    }
    //cout << str_temp << endl;
        file1 >> horizon_c_in;
    file1 >> str_temp;
        file1 >> nbUsers_in;
    //cout << nbUsers_in << endl;

    file1 >> str_temp;
        file1 >> nbSites_in;

    file1 >> str_temp;
    file1 >> nbSatellites_in;
    file1 >> str_temp;
    file1 >> nbAntennas_in;

        its_data.horizon_c=horizon_c_in;
        its_data.nbUsers= nbUsers_in;
        its_data.nbSatellites=nbSatellites_in;
        its_data.nbSites= nbSites_in;
        its_data.nbAntennas= nbAntennas_in;

        vector<float> contacts;

const char* cfile_contacts = "in_out/contacts.txt";
    ifstream file2(cfile_contacts);
    if ( !file2 ) {
        cerr << "No such file: " << cfile_contacts << endl;
        throw(-1);
    }
    float temp;
while(file2 >> temp){
    contacts.push_back(temp);

}
    float nb_contacts=contacts.back();
    contacts.pop_back();
    its_data.nb_contacts=nb_contacts;

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

    const char* output_file = "in_out/userToSat.txt"; 
    ofstream output(output_file);
int taille_userToSat=userToSat.size();
    for(int it=0; it < taille_userToSat ; it++){
        for(int j =0; j < userToSat[it].size(); j++){
            for (int k=0; k < userToSat[it][j].size(); k ++){
                int temp= userToSat[it][j][k];
                output << temp << endl;
            }
        }
        
    }

}



void parse_matrix_s(){
vector<float> contacts;
vector< vector < vector <int> > > antennaToSat;
 defined_data its_data;

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

        its_data.horizon_c=horizon_c_in;
        its_data.nbUsers= nbUsers_in;
        its_data.nbSatellites=nbSatellites_in;
        its_data.nbSites= nbSites_in;
        its_data.nbAntennas= nbAntennas_in;

const char* cfile_contacts = "in_out/contacts.txt";
    ifstream file2(cfile_contacts);
    if ( !file2 ) {
        cerr << "No such file: " << cfile_contacts << endl;
        throw(-1);
    }
    float temp;
while(file2 >> temp){
    contacts.push_back(temp);

}
    float nb_contacts=contacts.back();
    contacts.pop_back();
    its_data.nb_contacts=nb_contacts;

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
    
    for ( i=0; i<its_data.nbSites;i++)
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
        
        
        ifstream file1(cfile);
        if ( !file1 ) {
            cerr << "No such file: " << cfile << endl;
            throw(-1);
        }
        
        
        file1.ignore(256,'\n');
        file1.ignore(256,'\n');
        file1.ignore(256,'\n');
        
        while(file1 >> sat)
        {
            //cout << "ligne " << sat << endl;
            file1 >> s;
            //cout << "ligne " << s << endl;
            file1 >> e;
            //cout << "ligne " << e << endl;
            file1 >> d;
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

const char* output_file = "in_out/antennaToSat.txt"; 
    ofstream output(output_file);
int taille_antennaToSat=antennaToSat.size();
    for(int it=0; it < taille_antennaToSat ; it++){
        for(int j =0; j < antennaToSat[it].size(); j++){
            for (int k=0; k < antennaToSat[it][j].size(); k ++){
                int temp= antennaToSat[it][j][k];
                output << temp << endl;
            }
        }
        
    }

}

int main(){
    parse_contacts();

    parse_matrix_u();

    parse_matrix_s();

    return 0;
}












