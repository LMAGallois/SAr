#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "parser.h"
#include "solver.h"
#include "checker.h"
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


void f_wrapper( vector< vector < vector <int> > > userToSat, vector< vector < vector <int> > > antennaToSat,vector<float> contacts, defined_data its_data, int contact1, int contact2)
{
    std::mutex m;
        std::condition_variable cv;

    std::thread t([&](){
        solveMIP_time(userToSat, antennaToSat, contacts, its_data);
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
    defined_data its_data; 
    vector< vector < vector<int> > > userToSat;
    vector< vector < vector<int> > > antennaToSat;
    clock_t t; 
    int taille;

    //its_data=parse_contacts_sites(contacts_l);
        its_data=parse_contacts(contacts_l);
   if (  contacts_l.front() != 0 ) {
        cerr << " " << endl;
        ret=1;
    }
    vector<float> contacts(contacts_l.size());    

    taille=contacts_l.size();

    for( int i=0; i < taille; i ++){

        contacts[i]=contacts_l.front();
        contacts_l.pop_front();

    }
    //display_array(contacts);
    
    parse_matrix_u(contacts, userToSat, its_data);
    cout << "######################matrice userToSat" << endl << endl;
    //display_3Dmatrix(userToSat);
   
    parse_matrix_s(contacts, antennaToSat, its_data);
    cout << "########################matrice antennaToSat" << endl << endl;
    //display_3Dmatrix(antennaToSat);
    
    cout <<"THE END"<<endl;

    //pour solveMIP_ssc 
      int contact1=contacts.front();
     int contact2=contacts[1];
    
    t=clock();
//    solveMIP_basic(userToSat, antennaToSat, contacts, its_data);
f_wrapper(userToSat, antennaToSat, contacts, its_data, contact1, contact2);

    t=clock()-t;
    cout << "temps d'execution du solveur " << t/CLOCKS_PER_SEC << endl;

    check(contacts, userToSat, its_data);

    return ret;
}
