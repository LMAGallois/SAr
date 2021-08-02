#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "parser.h"
#include "solver.h"
#include "parser_sites.h"
#include "parser_sites_continu.h"
#include "parser_sites_60.h"
#include "solver_sites.h"
#include "solver_sites_60.h"
#include "solver_sites_h.h"
#include "solver_sites_continu.h"
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


vector<schedule_ssc> f_wrapper( vector< vector < vector<float> > > contacts_sites, vector< vector < vector <float> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, defined_data its_data, int contact1, int contact2, vector<vector<int> > &g_1)
{
    vector<schedule_ssc> res;
    std::mutex m;
        std::condition_variable cv;

    std::thread t([&](){
        res= solveMIP_ssc_h(contacts_sites, userToSat, antennaToSat, its_data, contact1, contact2, g_1);
        //res= solveMIP_ssc(contacts_sites, userToSat, antennaToSat, its_data, contact1, contact2);
        //res= solveMIP_ssc_continu(contacts_sites, userToSat, antennaToSat, its_data, contact1, contact2);
        cv.notify_one();

    }) ;

    t.detach();

    {
        std::unique_lock<std::mutex> l(m);
        if(cv.wait_for(l, 10s) == std::cv_status::timeout) 
            throw std::runtime_error("Timeout");
    }

    return res;   
}

vector<schedule_ssc> f_wrapper_60( vector< vector<float> > contacts_sites, vector< vector < vector <float> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, defined_data its_data, int contact1, int contact2)
{
    vector<schedule_ssc> res;
    std::mutex m;
        std::condition_variable cv;

    std::thread t([&](){
        res= solveMIP_ssc_60(contacts_sites, userToSat, antennaToSat, its_data, contact1, contact2);
        cv.notify_one();

    }) ;

    t.detach();

    {
        std::unique_lock<std::mutex> l(m);
        if(cv.wait_for(l, 10s) == std::cv_status::timeout) 
            throw std::runtime_error("Timeout");
    }

    return res;   
}

int main(){
    int ret=0;

// borne supérieure de couverture
    //get_upper_bound(2);

//1ers modèles 
   /* defined_data its_data;
    vector<float> contacts;
    vector< vector < vector<int> > > userToSat;
    vector< vector < vector<int> > > antennaToSat;
    list<schedule> plan;
    clock_t t;

    its_data=get_data(); 
    parse_contacts(contacts, its_data);
    // display_array(contacts);
    
    parse_matrix_u(contacts, userToSat, its_data);
    cout << "######################matrice userToSat" << endl << endl;
    display_3Dmatrix(userToSat);

    //parse_matrix_s(contacts, antennaToSat, its_data);
    cout << "########################matrice antennaToSat" << endl << endl;
    //display_3Dmatrix(antennaToSat);
    
    //check_2sat_handover( userToSat, its_data);
    //check_2sat( userToSat, its_data);

    t=clock();
   // solveMIP_basic(userToSat, antennaToSat, contacts, its_data, plan);
    t=clock()-t;
    cout << "temps d'execution du solveur " << t/CLOCKS_PER_SEC << endl;


    cout <<"THE END"<<endl;*/

//pour solveMIP_ssc
    
    defined_data its_data;
    vector< vector < vector<int> > > antennaToSat;
    vector< vector < vector<float> > > userToSat;
    its_data=get_data();

    //pour MIP_ssc_fin ou MIp_ssc
    vector< vector < vector <vector< vector<int> > > > > antennaToSatP;
    vector< vector < vector<float> > > contacts_sites;
    get_parser_sites(contacts_sites, userToSat, antennaToSat, antennaToSatP,its_data);

    //pour solveMIP_ssc_continu
    //vector< vector < vector <vector< vector<int> > > > > antennaToSatP;
    //vector< vector < vector<float> > > contacts_sites;
    //get_parser_sites_continu(contacts_sites, userToSat, antennaToSat, antennaToSatP,its_data);

    //pour solveMIP_ssc_60
    //vector< vector < vector < vector<int> > > >  antennaToSatP;
    //vector < vector<float> > contacts_sites;
    //get_parser_sites_60(contacts_sites, userToSat, antennaToSat, antennaToSatP,its_data);

    //display_3Dmatrix(antennaToSat);
    //display_3Dmatrix(userToSat);

    int contact1;
    int contact2;
    int taille2=contacts_sites.size();
    vector <int> antenna_locked(its_data.nbAntennas*its_data.nbSites);
    vector <int> sat_locked(its_data.nbSatellites);
    vector <schedule_ssc> extra_alloc;
    vector <schedule_ssc> alloc; 
    vector<vector<int> > g_1(its_data.nbAntennas*its_data.nbSites);
    vector <int> tempo(its_data.nbSatellites);
    for(int i=0; i < its_data.nbAntennas*its_data.nbSites; i++){
        g_1[i]=(tempo);
    }

    /*for (int i=0; i < taille2; i++){
        contact1=i;
        contact2=i+1;
        cout << i<<endl;
        // on résout le mip pour avoir une allocation sur le slot >= à 600s 
        //alloc=f_wrapper_60(contacts_sites, userToSat, antennaToSat,its_data, contact1, contact2);
        alloc=f_wrapper(contacts_sites, userToSat, antennaToSat,its_data, contact1, contact2, g_1);
        for(int i=0; i < its_data.nbAntennas*its_data.nbSites; i++){
            for(int j=0; j< its_data.nbSatellites; j++){
                g_1[i][j]=0;
            }
        }
        for(int i=0; i < alloc.size(); i++){
            schedule_ssc temp=alloc[i];
            g_1[temp.site*4+temp.antenna][temp.sat]=1;
        }
        extra_alloc.clear();


        if(alloc.size()!=its_data.nbAntennas*its_data.nbSites){
            antenna_locked.clear();

            for(int k=0; k < alloc.size(); k++){
                antenna_locked[alloc[k].site*4+alloc[k].antenna]=1;
                sat_locked[alloc[k].sat]=1;
            }
    // on regarde parmi les antennes libres celles qu on peut allouées pour gagner en couv
            
            for(int j=0; j < its_data.nbSites * its_data.nbAntennas; j ++){

                if(antenna_locked[j]!=1){

                    int max=0;
                    int chosen_sat=-1;
                    int time=0;
                    int time2=0;
                    for(int l=0; l < its_data.nbSatellites ; l++){
                        if(sat_locked[l]!=1){
                            int time=contact1;
                            int time2=0;

                            // pour MIP_ssc_60
                            //while(antennaToSatP[j][l][time][time2]==1 && time2 < contacts_sites[contact1].size()){
                            //    time2++;
                            //}

                            //pour tous les autres
                            while(antennaToSatP[j][l][time][time2][0]==1 && time2 < contacts_sites[contact1].size()){
                                time2++;
                            }

                            if(time > max){
                                max=contact1-time;
                                chosen_sat=l;
                            }
                        }
                        
                    }
                    
                    if(chosen_sat!=-1){
                        sat_locked[chosen_sat]=1;
                        schedule_ssc temp;
                        temp.sat=chosen_sat;
                        temp.s=contacts_sites[contact1][0][0];
                        temp.e=contacts_sites[contact1][time2][0];
                        temp.site=j/4;
                        temp.contact1=contact1;
                        temp.contact11=time2;
                        temp.contact111=0;
                        temp.antenna=j%4;
                        const char* output_file = "in_out/plan_checker_h.txt";
                        ofstream output(output_file, ios::app);
                        output << temp.s << endl;
                        output << temp.e << endl;
                        output << temp.sat << endl;
                        output << temp.site << endl;
                        output << temp.antenna << endl;
                        output << temp.contact1 << endl;
                        output << temp.contact11 << endl;
                        output << temp.contact111 << endl;
                        extra_alloc.push_back(temp);
                        antenna_locked[j]=1;
                    }
                }
            }       
        } 
    }*/
    check_2sat_handover( userToSat, its_data);
    check_2sat( userToSat, its_data);
    check_1sat(userToSat, its_data);
    check_1sat_handover(userToSat, its_data);
    cout <<"THE END"<<endl;

    return ret;
}