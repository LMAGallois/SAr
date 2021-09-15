#include "column.h"
#include <iostream>
#include <string>

using namespace std;

typedef IloArray<IloNumVarArray> NumVarMatrix;
typedef IloArray<NumVarMatrix> NumVar3Matrix;

ILOSTLBEGIN

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition (vector <schedule> &arr, int low, int high)
{
    int pivot = arr[high].s;    // pivot
    int i = (low - 1);  // Index of smaller element
 
    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j].s <= pivot)
        {
            i++;    // increment index of smaller element
            schedule temp_i=arr[i];
            arr[i]=arr[j];
            arr[j]=temp_i;
        }
    }
    schedule temp=arr[i+1];
    arr[i+1]=arr[high];
    arr[high]=temp;

    return (i + 1);
}
 
/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(vector <schedule> &arr, int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);
 
        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);

    }
}

void get_plan(list<schedule> plan){
    //cout << "coucou" << endl;
    const char* output_file = "in_out/plan.txt";
    ofstream output(output_file);

    int taille=plan.size();
    for(int it=0; it < taille ; it++){
        schedule temp= plan.front();
        output << "sat n°" << temp.sat << endl;
        output << "temps début :" << temp.s << endl;
        output << "temps fin : " << temp.e << endl;
        output << "-----------------------------------" << endl;
        plan.pop_front();
    }

    /*while(plan.size() != 0){
        cout << "sat n°" << plan.front().sat << endl;
        cout << "temps début :" << plan.front().s << endl;
        cout << "temps fin : " << plan.front().e << endl;
        cout << "-----------------------------------" << endl;
        plan.pop_front();
    }*/
}

int max(int a, int b, int c){
    int res;

    if(a>=b>=c){
        res=a;
    }else if(b>=a>=c){
        res=b;
    }else if(a>=c>=b){
        res=a;
    }else if(b>=c>=a){
        res=b;
    }else if(c>=b>=a){
        res=c;
    }else if(c>=a>=b){
        res=c;
    }

    return res;
}

float get_start_t(float s, vector<float> contacts){
    float res;
    if(contacts[s]){
        res=contacts[s];
    }else if(s!=0){
        res=-1;
    }else{
        res=0;
    }
    return res;
}

float get_end_t( float s, vector<float> contacts){
    float res;
    if(contacts[s]){
        res=contacts[s];
    }else if(s!= 0){
        res=-1;
    }else{
        res=0;
    }
    return res;
}

void solveMIP(vector< vector < vector <int> > > userToSat, vector< vector < vector <int> > > antennaToSat, vector< vector <schedule> > creneaux, vector<float> contacts, defined_data its_data,list<schedule> res)
{
    IloEnv env;
    try {
        int cond_arret_false=1;

        int nbEnsembles=1;
            for(int i=48; i > 28; i--){
                nbEnsembles=nbEnsembles*i;
            }

    //init
    
    //ensemble des ensembles de satellites repéré par un nombre de taille max nbEnsembles
    vector<int> S;
    //nb d'ensembles ajoutés à chaque fois que la cond est violée
    int taille_S= ;

    
        while(cond_arret_false){
            IloModel model(env);
            IloCplex cplex(model);

            taille_S=S.size();
    //connexion entre une antenne et un satellite lors d'un contact c
            NumVar3Matrix g(env, its_data.nbSites*its_data.nbAntennas);
    //indique si l'ensemble de satellites E est actif durant le contact c
            NumVarMatrix z(env, taille_S);
    //indique si un staellite est présent dans l'ensemble E
            IloNumVarArray a(env, its_data.nbAntennas*its_data.nbSites, 0, 1, ILOINT);
    //somme sur tous les utilisateurs des portions de contacts c de connexion satellite - utilisateur (nb sat borné à 2 par utilisateur)
            NumVarMatrix delta(env, taille_S);

    //contraintes
            IloRangeArray contr_1(env);
            IloRangeArray contr_2(env);
            IloRangeArray contr_3(env);

    //objectif function
            IloNumExpr obj(env);
            for(int i=0; i < nb_contacts; i++){
                for(int j=0; j < taille_S; j++){
                    obj=obj+delta[j][i]*z[j][i];
                }
            }
            model.add(IloMaximize(env, obj));
            obj.end();

            for(int i=0; i < taille_S; i++){
                delta[i]=IloNumVarArray(env, its_data.nb_contacts, 0, 2800, ILOFLOAT);
                z[i]=IloNumVarArray(env, its_data.nb_contacts, 0, 1, ILOINT);
            }

            for(int i=0; i<its_data.nbSites*its_data.nbAntennas; i++){
                    g[i]=NumVarMatrix(env, its_data.nbSatellites);
                
                for(int j=0; j< its_data.nbSatellites; j++){
                        g[i][j]=IloNumVarArray(env, its_data.nb_contacts, 0, 1, ILOINT);
                    

                    for (int k=0; k < its_data.nb_contacts; k++){
                            //cout << "solver " << endl;

                        //cout << "solver 2" << endl;
                        if(antennaToSat[i][j][k]==0){
                            model.add(g[i][j][k]==0);
                                        
                        }

                    }
                }
            cout << "passe n°" << i << endl;

            }
            
            cout << "dimensions des variables" << endl;
            cout << "dimensions de z : " << taille_S <<endl;
            cout << "dimensions de g : " << its_data.nbSites * its_data.nbAntennas << "*"<< its_data.nbSatellites<< "*" << its_data.nb_contacts <<endl;
            cout << "dimensions de a : " << its_data.nbAntennas*its_data.nbSites << endl;
            cout << "dimensions de delta : " << taille_S << "*" << its_data.nb_contacts << endl;
            int nb_var=taille_S + its_data.nbSites * its_data.nbAntennas*its_data.nbSatellites*its_data.nb_contacts + its_data.nbAntennas*its_data.nbSites+taille_S*its_data.nb_contacts;
            cout << "total nb variables : " << nb_var << endl;

            if(nb_var < 621160){
                 //constraints
                cout << "adding constraints to solver" << endl;

                // - g(a,s,c) <= - somme sur E(de S) de a(s,E) * z(E,c) pour tout (a,s,c)

                for (int i=0; i < its_data.nbAntennas* its_data.nbSites; i++){

                    for (int j=0; j < its_data.nbSatellites; j++){

                        for (int l=0; l< its_data.nb_contacts; l++){

                            IloNumExpr temp(env);
                            for (int k=0; k < taille_S ; k++){
                                temp= temp+ a[j][k] * z[k][l]
                            }
                            temp=-temp;
                            contr_1.add(- g[i][j][l] <= temp );
                            temp.end();

                        }
                    }
                }
                model.add(contr_1);

                        cout << "adding constraints to solver" << endl;

                //somme sur s de g(a,s,c) <= 1 pour tout (a,c)

                for (int j=0; j < its_data.nb_contacts; j++){

                    for (int a=0; a< its_data.nbSites*its_data.nbAntennas; a++){

                        IloNumExpr temp1(env);
                        for (int k=0; k < its_data.nbSatellites ; k++){
                        
                            temp1= temp1 + g[a][k][j];
                        }
                        
                        contr_2.add(temp1 <= 1);
                        temp1.end();
                    }
                }
                model.add(contr_2);

                //somme sur E(de S) de z(E,c) <= 1 pour tout c
                cout << "adding constraints to solver" << endl;
                    
                for (int i=0; i< its_data.nb_contacts; i++){
                    
                    IloNumExpr temp2(env);
                    for (int j=0; j < taille_S; j++){      
                        temp2=temp2+z[j][i];
                    }

                    contr_3.add(temp2 <= 1);
                }
                model.add(contr_3);
                
                cout << "solver ready" << endl;

                // Optimize
                IloCplex cplex(model);
                // time out
                //cplex.setParam(IloCplex::TiLim, 200);
                // duality gap set to 2%    
                cplex.setParam(IloCplex::EpGap, 0.0000001);     
                if(cplex.solve()){
                    //cout << "feasible" << endl;
                    cout << "cplex status : " << cplex.getStatus() << endl;
                }else{
                    cout << "cplex status : " << cplex.getStatus() << endl;
                }
                //cplex.exportModel("model.lp");

                env.out() << "Obj : " << cplex.getObjValue() << endl;

            //get the duals
                list<double> duals_1;
                int nb_duals_1=  its_data.nbAntennas*its_data.nbSites*its_data.nb_contacts*its_data.nbSatellites;

                for(int i=0; i< nb_duals_1; i++){
                    duals_1.push_back(cplex.getDual(contr_1[i]));
                }

                list<double> duals_2;
                int nb_duals_2=  its_data.nbAntennas*its_data.nbSites*its_data.nb_contacts;

                for(int i=0; i< nb_duals_2; i++){
                    duals_2.push_back(cplex.getDual(contr_2[i]));
                }

                list<double> duals_3;
                int nb_duals_3= its_data.nb_contacts;

                for(int i=0; i< nb_duals_3; i++){
                    duals_3.push_back(cplex.getDual(contr_3[i]));
                }

            //
            //taille à définir
                vector<int> newS;

                output sub_pb_output = sub_pb(duals_1,duals_2, duals_3, newS);
                
                for(int i=0; i < its_data.nb_contacts; i++){
                
                    if(sub_pb_output.obj > sub_pb_output.gamma[i]){
                        //condition violée donc on ajoute des variables, ici, des ensembles de satellites possibles
                        
                    }else{
                        //on s'arrête (on branche pour aller jusqu'à 10jours)
                        cond_arret_false=0;
                    }

                }

            }else{
                cond_arret_false=0;
                cout << "nombre de variables trop grand" << endl;
            }
   
        }
    }
    catch (IloException& ex) {
        cerr << "Error: " << ex << endl;
    }
    catch (...) {
        cerr << "Error" << endl;
    }
    env.end();
}