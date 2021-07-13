//a partir d'un plan d'ordonnancement, on regarde sur chaque slot de temps le % d'utilisateurs couverts, on retient le min , le max et la moyenne
//les temps du plan correspondent aux temps repertoriés dans les contacts

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <fstream>
#include "parser.h"
#include <limits.h>

using namespace std;

void check(vector <float> contacts, vector < vector < vector < int> > > userToSat, defined_data its_data){
    const char* cfile = "in_out/plan_checker.txt";
    ifstream file(cfile);
    if ( !file ) {
        cerr << "No such file: " << cfile << endl;
        throw(-1);
    }

    int taille;
    file >>taille;
    cout << taille << endl;
    vector < vector < schedule> > plan;

    int sat;
    float s;
    float e;
    
    float s_1=-1;

    for(int i =0; i < taille; i++){
        file >> sat;
        file >> s;
        file >> e;

        schedule slot={sat, s, e};

        if(s_1!=-1){
            if(s_1!=s){
               	vector <schedule> tempo;
		plan.push_back(tempo);
	        plan.back().push_back(slot);

            }else{
	      plan.back().push_back(slot);

	    }

        }else{
	  vector <schedule> tempo;
	  plan.push_back(tempo);
	  plan.back().push_back(slot);
	}

        s_1=s;
    }
    taille=plan.size();
    int nb=0;
    int min=INT_MAX;
    int max=INT_MIN;
    float average=0;

    for(int i=0; i <taille; i ++ ){
        nb=0;
        vector <int> people(its_data.nbUsers);
        for(int j=0; j < plan[i].size(); j++){
            
            for(int k=0; k < its_data.nbUsers; k++){
                schedule temp=plan[i][j];
                if(people[k]==0){
                    if(userToSat[k][temp.sat][temp.s]==1){
                        nb++;
                        people[k]=-1;
                    }
                }
            }


        }
        if(nb>max){
            max=nb;
        }
	if(nb < min){
            min=nb;
        }

        average+=nb;
    }

    average= average/taille;
    
    cout << "max" << max << endl;
    cout << "min "<< min << endl;
    cout <<"moyenne "<< average << endl;
    cout << "soit " << average/its_data.nbUsers *100 << "%"<<endl;

}
