#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "parser.h"
#include "solver.h"
#include "parser_sites.h"
#include "solver_sites.h"
#include "checker.h"
#include "checker_bounds.h"
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


void f_wrapper( vector< vector < vector <int> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, defined_data its_data, int contact1, int contact2)
{
    //vector<schedule_ssc> res;
    std::mutex m;
        std::condition_variable cv;

    std::thread t([&](){
        solveMIP_ssc(userToSat, antennaToSat, its_data, contact1, contact2);
        cv.notify_one();

    }) ;

    t.detach();

    {
        std::unique_lock<std::mutex> l(m);
        if(cv.wait_for(l, 10s) == std::cv_status::timeout) 
            throw std::runtime_error("Timeout");
    }

    //return res;   
}

int main(){
    int ret=0;

// borne supérieure de couverture
    //get_upper_bound(2);

//1ers modèles 
    /*defined_data its_data;
    vector<float> contacts;
    vector< vector < vector<int> > > userToSat;
    vector< vector < vector<int> > > antennaToSat;
    list<schedule> plan;
    clock_t t;

    its_data=get_data(); 
    parse_contacts(contacts);
    // display_array(contacts);
    
    // parse_matrix_u(contacts, userToSat, its_data, 0);
    cout << "######################matrice userToSat" << endl << endl;
    //display_3Dmatrix(userToSat);
   
    //parse_matrix_s(contacts, antennaToSat, its_data);
    cout << "########################matrice antennaToSat" << endl << endl;
    //display_3Dmatrix(antennaToSat);
    
    //check_2sat_handover( userToSat, its_data);
    //check_2sat( userToSat, its_data);

    t=clock();
    solveMIP_basic(userToSat, antennaToSat, contacts, its_data, plan);
    t=clock()-t;
    cout << "temps d'execution du solveur " << t/CLOCKS_PER_SEC << endl;
*/

    cout <<"THE END"<<endl;

//pour solveMIP_ssc
    
    defined_data its_data;
    vector< vector < vector<int> > > antennaToSat;
    vector< vector < vector<int> > > antennaToSatP;
    vector< vector < vector<int> > > userToSat;
    vector< vector < vector<float> > > contacts_sites;
    get_parser_sites(contacts_sites, userToSat, antennaToSat, antennaToSatP, its_data);
    display_3Dmatrix(antennaToSat);
//display_3Dmatrix(userToSat);

    cout <<"THE END"<<endl;
    int contact1;
    int contact2;
    int taille2=contacts_sites.size();

    for (int i=0; i < 1; i++){
        contact1=i;
        contact2=i+1;
        cout << i<<endl;
        // on résout le mip pour avoir une allocation sur le slot >= à 600s 
        f_wrapper(userToSat, antennaToSat,its_data, contact1, contact2);
        // on regarde parmi les antennes libres celles qu on peut allouées pour gagner en couv
        for(int j=0; j < its_data.nbSites * its_data.nbAntennas; j ++){
        // on regarde par site quelles antennes est dispo, ie n est pas deconnectee de moins de 60s

        }
    }
    

    return ret;
}