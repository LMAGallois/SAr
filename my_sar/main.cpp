/*
Chaque fichier h trouvera sa description. S'il n'y a pas de fichier h, alors la description se trouvera directement dans le fichier .cpp
Tous les fichiers parser_... servent à parser les données d'entrée
Tous les fichiers solver_... servent à lancer un MIP

Mettre dans /data le fichier cov20 pour les coordonnées des utilisateurs, 
les fichiers de donnée de Thalès quand on opère avec les contacts de Thalès (visiKOU, visiNOU, visiREU, visiTHT, visiSVA),
les éphémérides des satellites sinon (ex : eph_23_12_20)

pour changer nombre de satellites utilisés, modifier le fichier in_out/config.txt

pour changer l'angle d'élévation, modifier le fichier parser_instance....cpp (suivant quel mip on veut lancer)
dans la fonction ele_a_20, modifier la ligne " if (res <= (90.0-20.0)){ " et modifier "20.0" par la valeur voulue

pour modifier l'interpolation, modifier le fichier parser_instance.cpp
dans la fonction get_trackSatInterpolate, modifier les "60" avec la valeur voulue (période d'échantillonnage)
on peut aussi modifier le nombre de points connus qui sera considéré pour l'interpolation lagrangienne (paramètre n de la fonction interpolate() dans le fichier langrange.cpp)

to run the code :
make clean
make 
./sched_Cband
*/

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "parser.h"
#include "solver.h"
#include "solver_horizon.h"
#include "solver_lh.h"
#include "parser_sites.h"
#include "parser_sites_continu.h"
#include "parser_sites_60.h"
#include "solver_sites.h"
#include "solver_sites_60.h"
#include "solver_sites_h.h"
#include "solver_sites_continu.h"
#include "parser_instance.h"
#include "parser_instance_continu.h"
#include "parser_instance_60.h"
#include "solver_sites_60_h.h"
#include "checker.h"
#include "checker_bounds.h"
#include <chrono>
#include <future>
#include <thread>
#include <condition_variable>
#include <time.h>
#include "checker_all.h"

//using namespace std::chrono;
//using namespace std::literals;
using namespace std::literals::chrono_literals;
//using namespace std::chrono_literals;
using namespace std;

// à utiliser pour fixer un timer quand le mip est lancé sur tout l'horizon (pas essayé encore, le MIP sur chaque slot suffit)
//Un MIP sur tout l'horizon pourrait servir à réduire davantage les handovers en essayant les divers chemins possibles mais très demandeur en puissance de calcul
void f_wrapper_horizon( vector< vector < vector<float> > > contacts_sites, vector< vector < vector <float> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, defined_data its_data)
{
    std::mutex m;
    std::condition_variable cv;

    std::thread t([&](){
        solveMIP_ssc_horizon(contacts_sites, userToSat, antennaToSat, its_data);
        cv.notify_one();

    }) ;

    t.detach();

    {
        std::unique_lock<std::mutex> l(m);
        if(cv.wait_for(l, 120s) == std::cv_status::timeout) 
            throw std::runtime_error("Timeout");
    }

}

//pour fixer un timer pour la résolution du MIP
vector<schedule_ssc> f_wrapper( vector< vector < vector<float> > > contacts_sites, vector< vector < vector <float> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, defined_data its_data, int contact1, int contact2, vector<vector<int> > &g_1, float alpha)
{
    vector<schedule_ssc> res;
    std::mutex m;
        std::condition_variable cv;

    std::thread t([&](){
        res= solveMIP_ssc_h(contacts_sites, userToSat, antennaToSat, its_data, contact1, contact2, g_1, alpha);
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

//pour fixer un timer pour la résolution du MIP sur des slots inférieurs à 600s et supérieurs ou égaux à 60s
vector<schedule_ssc> f_wrapper_60( vector< vector<float> > contacts_sites, vector< vector < vector <float> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, defined_data its_data, int contact1, int contact2, vector<vector<int> > &g_1)
{
    vector<schedule_ssc> res;
    std::mutex m;
        std::condition_variable cv;

    std::thread t([&](){
        //res= solveMIP_ssc_60(contacts_sites, userToSat, antennaToSat, its_data, contact1, contact2);
        res= solveMIP_ssc_60_h(contacts_sites, userToSat, antennaToSat, its_data, contact1, contact2, g_1);
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

int main(int argc, char *argv[]){
    int ret=0;
    if(argc != 3){
        cerr<<"nombre de paramètres invalides"<<endl;
    }
        const char* path=argv[1];
        float alpha=atof(argv[2]);

// borne supérieure de couverture
    //get_upper_bound(2);

//1ers modèles (moins performants et moins justes)
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

            //pour instance thales
            //décommenter un bloc de code suivant quel mip on veut lancer

    //pour MIP_ssc_fin ou MIp_ssc ou MIP_ssc_h
    //vector< vector < vector <vector< vector<int> > > > > antennaToSatP;
    //vector< vector < vector<float> > > contacts_sites;
    //get_parser_sites(contacts_sites, userToSat, antennaToSat, antennaToSatP,its_data);
        
    //pour solveMIP_ssc_continu
    //vector< vector < vector <vector< vector<int> > > > > antennaToSatP;
    //vector< vector < vector<float> > > contacts_sites;
    //get_parser_sites_continu(contacts_sites, userToSat, antennaToSat, antennaToSatP,its_data);

    //pour solveMIP_ssc_60
    //vector< vector < vector < vector<int> > > >  antennaToSatP;
    //vector < vector<float> > contacts_sites;
    //get_parser_sites_60(contacts_sites, userToSat, antennaToSat, antennaToSatP,its_data);

    //display_3Dmatrix(antennaToSat);
    //display_3Dmatrix_f(userToSat);

            //pour nouvelle instance avec contacts thales
            //decommenter un bloc de code suivant quel mip on veut lancer

    //pour  MIP_ssc ou MIP_ssc_h
    /*vector <v_3> coorSite;
    vector<v_3> userCoords(its_data.nbUsers);
    vector< vector <v_3> > trackSat;
    vector< vector < vector<float> > > contacts_sites;
    vector< vector < vector <vector< vector<int> > > > > antennaToSatP;
    parse_contacts_sites(contacts_sites, its_data);
    parse_coords_u(userCoords, its_data);
    parse_site(coorSite, its_data);
    parse_eph(trackSat, its_data);
    get_visi_u(trackSat, userCoords, its_data, userToSat, contacts_sites);
    get_visi_a(trackSat, coorSite, its_data, antennaToSat, contacts_sites);*/
    //get_visi_aP(trackSat, coorSite, its_data, antennaToSatP, contacts_sites);

    //display_3Dmatrix_f(userToSat);
    //display_matrix_h(trackSat);

    //avec contacts thales pour MIP_ssc_60
    /*vector <v_3> coorSite;
    vector<v_3> userCoords(its_data.nbUsers);
    vector< vector <v_3> > trackSat;
    vector < vector<float> > contacts_sites;
    parse_coords_u(userCoords, its_data);
    parse_site(coorSite, its_data);
    parse_eph(trackSat, its_data);
    parse_contacts_sites_60(contacts_sites, its_data);
    get_visi_u_60(trackSat, userCoords, its_data, userToSat, contacts_sites);
    get_visi_a_60_thales(trackSat, userCoords, its_data, antennaToSat, contacts_sites);*/

    // avec contacts thales MIP_ssc_continu
    /*vector <v_3> coorSite;
    vector<v_3> userCoords(its_data.nbUsers);
    vector< vector <v_3> > trackSat;
    vector< vector < vector<float> > > contacts_sites;
    parse_coords_u(userCoords, its_data);
    parse_site(coorSite, its_data);
    parse_eph(trackSat, its_data);
    parse_contacts_sites(contacts_sites, its_data);
    get_visi_u_c(trackSat, userCoords, its_data, userToSat, contacts_sites);
    get_visi_a(trackSat, userCoords, its_data, antennaToSat, contacts_sites);*/

        // nouvell einstance avec nouveaux contacts
        //decommenter un bloc de code suivant quel mip on veut lancer

    //pour  MIP_ssc ou MIP_ssc_h
    vector <v_3> coorSite;
    vector<v_3> userCoords(its_data.nbUsers);
    vector< vector <v_3> > trackSat;
    vector< vector <v_3> > trackSatInterpolate;
    vector< vector < vector<float> > > contacts_sites;
    vector< vector < vector <vector< vector<int> > > > > antennaToSatP;
    vector< vector < vector<int> > > antennaToSatT;
    vector< vector < vector<float> > > userToSatC;
    vector< vector < vector<float> > > userToSatAll;
    parse_coords_u(userCoords, its_data);
    parse_site(coorSite, its_data);
    parse_eph(trackSat, its_data, path);
    get_trackSatInterpolate(trackSat, trackSatInterpolate, its_data);
    get_visi_a(trackSat, coorSite, its_data, antennaToSatT);
    parse_contacts_sites_i(antennaToSatT, contacts_sites, its_data);
    get_visi_u(trackSat, userCoords, its_data, userToSat, contacts_sites);
    reconstruct_visi_a(antennaToSatT, contacts_sites, antennaToSat, its_data);
    get_visi_u_all(trackSatInterpolate, userCoords, its_data, userToSatAll);
    //get_visi_u_c(trackSat, userCoords, its_data, userToSatC, contacts_sites);

    // pour contacts arbitraires
    /*vector <v_3> coorSite;
    vector<v_3> userCoords(its_data.nbUsers);
    vector< vector <v_3> > trackSat;
    vector< vector <v_3> > trackSatInterpolate;
    vector< vector < vector<float> > > contacts_sites;
    vector< vector < vector <vector< vector<int> > > > > antennaToSatP;
    vector< vector < vector<int> > > antennaToSatT;
    vector< vector < vector<float> > > userToSatC;
    vector< vector < vector<float> > > userToSatAll;
    parse_coords_u(userCoords, its_data);
    parse_site(coorSite, its_data);
    parse_eph(trackSat, its_data, path);
    get_trackSatInterpolate(trackSat, trackSatInterpolate, its_data);
    get_contacts(contacts_sites, its_data);
    get_visi_a_thales(trackSat, coorSite, its_data, userToSat, contacts_sites )
    get_visi_u(trackSat, userCoords, its_data, userToSat, contacts_sites);
    reconstruct_visi_a(antennaToSatT, contacts_sites, antennaToSat, its_data);
    get_visi_u_all(trackSatInterpolate, userCoords, its_data, userToSatAll);*/

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

    //pour lancer le mip sur tout l'horizon (jamais utilisé encore, demande beaucoup de puissance)
    //f_wrapper_horizon(contacts_sites, userToSat, antennaToSat,its_data);
    //solveMIP_ssc_horizon(contacts_sites, userToSat, antennaToSat, its_data);
    //solveMIP_ssc_lh(contacts_sites, userToSat, antennaToSat, its_data);

    //pour lancer le mip slot par slot
    /*for (int i=0; i < taille2-1; i++){
        contact1=i;
        contact2=i+1;
        cout << i<<endl;

        //alloc=f_wrapper_60(contacts_sites, userToSat, antennaToSat,its_data, contact1, contact2, g_1);
        alloc=f_wrapper(contacts_sites, userToSat, antennaToSat,its_data, contact1, contact2, g_1, alpha);

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
       
        --------------------------------------------------debut code pour mip fin---------------------------------------------------
        /*if(alloc.size()!=its_data.nbAntennas*its_data.nbSites){
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
                        const char* output_file = "in_out/plan_checker_1fin.txt";
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
        --------------------------------------------------end code pour mip fin---------------------------------------------------

    }*/

    //checker
    check_2sat_handover_all( userToSatAll, its_data);
    check_2sat_all( userToSatAll, its_data);
    check_1sat_all(userToSatAll, its_data);
    check_1sat_handover_all(userToSatAll, its_data);
    cout <<"THE END"<<endl;

    return ret;
}
