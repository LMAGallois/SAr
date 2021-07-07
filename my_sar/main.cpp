#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "parser.h"
#include "solver.h"
#include <chrono>
#include <future>
#include <thread>
#include <condition_variable>
#include <time.h>

//using namespace std::chrono;
//using namespace std::literals;
using namespace std::literals::chrono_literals;
//using namespace std::chrono_literals;
using namespace std;


void f_wrapper( vector< vector < vector <int> > > userToSat, vector< vector < vector <int> > > antennaToSat, vector< vector <schedule> > creneaux,vector<float> contacts, defined_data its_data, list<schedule> plan, int contact)
{
    std::mutex m;
        std::condition_variable cv;

    std::thread t([&](){
        solveMIP_ssc(userToSat, antennaToSat, creneaux, contacts, its_data, contact);
        cv.notify_one();

    }) ;

    t.detach();

    {
        std::unique_lock<std::mutex> l(m);
        if(cv.wait_for(l, 50s) == std::cv_status::timeout) 
            throw std::runtime_error("Timeout");
    }    
}

int main(){
    int ret=0;
     

    list<float> contacts_l;
    vector< vector <schedule> > creneaux;
    defined_data its_data; 
    vector< vector < vector<int> > > userToSat;
    vector< vector < vector<int> > > antennaToSat;
    list<schedule> plan;
    clock_t t; 
    int taille;

    its_data=parse_contacts(contacts_l, creneaux);
    if (  contacts_l.front() != 0 ) {
        cerr << " " << endl;
        ret=1;
    }
    vector<float> contacts(contacts_l.size());

    display_2Dmatrix(creneaux);
    

    taille=contacts_l.size();

    for( int i=0; i < taille; i ++){
            cout << "veuillez entrer un chiffre" << endl; 

        contacts[i]=contacts_l.front();
        contacts_l.pop_front();

    }
    display_array(contacts);
    
    //taille=contacts.size();
    
    /*for( int i=0; i < taille; i ++){
        cout << contacts.back() << endl;
        contacts.pop_back();
        //cout << "errCF " << endl;

    }*/
    
    parse_matrix_u(contacts, userToSat, its_data);

    parse_matrix_s(contacts, antennaToSat, its_data);

            int a;
        cin>> a;

    cout << "######################matrice userToSat" << endl << endl;
    //display_3Dmatrix(userToSat);

    /*for (int j=0; j < contacts_l.size(); j++ ){
        for(int j=0; j < nbSatellites; j++){
            for(int k=0; k < nbSites*nbAntennas; k++){
                if(antennaToSat[k][j][j]==1){
                    cout << "coucou matrix" << endl;

                }
            }
        }
    }*/

    cout << "########################matrice antennaToSat" << endl << endl;
    //display_3Dmatrix(antennaToSat);
    
    //pour solveMIP_ssc 
    int contact=0;
    
    t=clock();
//    solveMIP_basic(userToSat, antennaToSat, creneaux, contacts, its_data, plan);
f_wrapper(userToSat, antennaToSat, creneaux, contacts, its_data ,plan ,contact);

    t=clock()-t;
    cout << "temps d'execution du solveur " << t/CLOCKS_PER_SEC << endl;


    return ret;
}