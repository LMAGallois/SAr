#include "solver.h"
#include <iostream>
#include <string>

using namespace std;

typedef IloArray<IloNumVarArray> NumVarMatrix;
typedef IloArray<NumVarMatrix> NumVar3Matrix;
typedef IloArray<IloNumArray> NumMatrix;

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

void solveMIP(vector< vector < vector <int> > > userToSat, vector< vector < vector <int> > > antennaToSat, vector< vector <schedule> > creneaux, vector<float> contacts, defined_data its_data, list<schedule> res)
{
    IloEnv env;
    try {
        
        IloModel model(env);
        
        NumVar3Matrix x(env, its_data.nbUsers);
        NumVar3Matrix g(env, its_data.nbSites*its_data.nbAntennas);
        NumVarMatrix z(env, its_data.nbUsers);

        for(int i=0; i<its_data.nbSites*its_data.nbAntennas; i++){
            g[i]=NumVarMatrix(env, its_data.nbSatellites);

            for(int j=0; j< its_data.nbSatellites; j++){
                g[i][j]=IloNumVarArray(env, its_data.nb_contacts, 0, 1, ILOINT);

                for (int k=0; k < its_data.nb_contacts; k++){                            

                    if(antennaToSat[i][j][k]==0){
                        model.add(g[i][j][k]==0);
                    }
                }
            }
            cout << "passe g n°" << i << endl;
        }

        for(int i=0; i<its_data.nbUsers; i++){
            x[i]=NumVarMatrix(env, its_data.nbSatellites);
            z[i]=IloNumVarArray(env, its_data.nb_contacts);

            for(int j=0; j< its_data.nbSatellites; j++){
                x[i][j]=IloNumVarArray(env, its_data.nb_contacts, 0, 1, ILOINT);

                for (int k=0; k < its_data.nb_contacts; k++){
                        
                        if(userToSat[i][j][k]==0){
                            //cout << "solver 0" << endl;
                            model.add(x[i][j][k]==0);
                            //cout << "solver 0 end" << endl;
                        }
                        
                        z[i][k]=IloNumVar(env, 0,IloInfinity , ILOINT);                    
                }
            cout << "passe x et z n°" << i << endl;
            }
        }

        /*for (int k=0; k < its_data.nb_contacts; k++){
            
            for(int j=0; j< its_data.nbSatellites; j++){
                
                for(int i=0; i<its_data.nbUsers; i++){
                    
                    x[i]=NumVarMatrix(env, its_data.nbSatellites);
                    x[i][j]=IloNumVarArray(env, its_data.nb_contacts, 0, 1, ILOINT);

                    if(userToSat[i][j][k]==0){
                            //cout << "solver 0" << endl;
                            model.add(x[i][j][k]==0);
                            //cout << "solver 0 end" << endl;
                    }
                    
                    z[i]=IloNumVarArray(env, its_data.nb_contacts);                        
                    z[i][k]=IloNumVar(env, 0,IloInfinity , ILOINT);  
                }

                for(int i=0; i<its_data.nbSites*its_data.nbAntennas; i++){
                    
                    g[i]=NumVarMatrix(env, its_data.nbSatellites);
                    g[i][j]=IloNumVarArray(env, its_data.nb_contacts, 0, 1, ILOINT);
                    
                    if(antennaToSat[i][j][k]==0){
                        model.add(g[i][j][k]==0);
                    }
                }
            }
        }*/

        cout << "nbcontacts : " << its_data.nb_contacts << endl;
        cout << "nbUsers : " << its_data.nbUsers << endl;
        cout << "nbSatellites : " << its_data.nbSatellites << endl;
        cout << "nbAntennes total : " << its_data.nbSites * its_data.nbAntennas << endl;
        cout << "dimensions des variables" << endl;
        cout << "dimensions de x : " << its_data.nbUsers << "*"<< its_data.nbSatellites << "*" << its_data.nb_contacts <<endl;
        cout << "dimensions de g : " << its_data.nbSites * its_data.nbAntennas << "*"<< its_data.nbSatellites<< "*" << its_data.nb_contacts <<endl;
        cout << "dimensions de z : " << its_data.nbUsers << "*"<< its_data.nb_contacts<< endl;
        cout << "total nb variables : " << its_data.nbUsers* its_data.nb_contacts + its_data.nbSites * its_data.nbAntennas *its_data.nbSatellites*its_data.nb_contacts + its_data.nbUsers*its_data.nbSatellites*its_data.nb_contacts << endl;
        /////////////////////////////constraints
        cout << "adding constraints to solver" << endl;

        //somme sur a de g(a,sat,c) >= x(u,sat,c) 

        for (int a=0; a< its_data.nbSatellites; a++){
            
            for (int j=0; j < its_data.nb_contacts; j++){
                IloNumExpr temp(env);

                for (int k=0; k < its_data.nbSites*its_data.nbAntennas ; k++){
                    
                    temp += g[k][a][j];
                    //model.add(temp += g[k][a][j]);
                    //cout << "solver C1" << endl;
                }

                for (int o=0; o < its_data.nbUsers; o++){
                    model.add(temp >= x[o][a][j]);
                    //cout << "solver C1b" << endl;
                }
                temp.end();
            }
        }
        
        //somme sur sat de g(a,sat,c) <= 1
        for (int a=0; a< its_data.nbSites*its_data.nbAntennas; a++){
            
            for (int j=0; j < its_data.nb_contacts; j++){
                IloNumExpr temp1(env);

                for (int k=0; k < its_data.nbSatellites ; k++){
                
                    temp1= temp1 + g[a][k][j];
                }
                
                model.add(temp1 <= 1);
                temp1.end();
            }
        }

        //somme sur sat de x(u,sat,c) >= 2 - z(u,c)

        for (int a=0; a< its_data.nbUsers; a++){
            
            for (int j=0; j < its_data.nb_contacts; j++){
                IloNumExpr temp2(env);

                for (int k=0; k < its_data.nbSatellites ; k++){
                
                    temp2 = temp2 + x[a][k][j];
                }
                
                model.add(temp2 >= 2-z[a][j]);
                temp2.end();
            }
        }

        //somme sur u et sur c de z(u,c)
        IloNumExpr temp3(env);
        for (int i=0; i< its_data.nbUsers; i++){
            
            for (int j =0; j < its_data.nb_contacts; j++){

                temp3 = temp3 + z[i][j];
            }
        }

        model.add (IloMinimize(env, temp3 ));
        temp3.end();




        
        cout << "solver ready" << endl;

        // Optimize
        IloCplex cplex(model);
        // time out
        cplex.setParam(IloCplex::TiLim, 200);
        // duality gap set to 2%    
        //cplex.setParam(IloCplex::EpGap, 0.01);     
        
        cplex.solve();
        
        
        env.out() << "Obj : " << cplex.getObjValue() << endl;

        for (int i=0; i< its_data.nbUsers; i++){
            
            for (int j =0; j < its_data.nbSatellites; j++){

                int k=0;
                while (k < its_data.nb_contacts){

                    if(cplex.getValue(x[i][j][k]) == 1){
                        schedule slot;
                        slot.sat=j;
                        slot.s=get_start_t(k, contacts);

                        while(cplex.getValue(x[i][j][k]) == 1){
                            k++;
                        }
                        slot.e=get_end_t(k, contacts);
                        res.push_back(slot);
                        //cout << "get res values " <<  endl;
                        
                    }else{
                        k++;
                    }
  
                }
            }
        }  
//on enleve les doublons
        vector <schedule> res_v(res.size());

        int taille=res.size();
        for(int i=0; i < taille; i++){
            schedule temp= res.front();
            res_v[i]=temp;
            res.pop_front();
            res.push_back(temp);
        }

        int taille_ss=res_v.size();
        for(int i=0; i < taille; i ++){
            schedule temp=res.front();
            res.pop_front();
            int j=i+1;
            int not_found=1;

            while(j < taille_ss && not_found){
                if(temp.e==res_v[j].e && temp.s==res_v[j].s && temp.sat==res_v[j].sat){
                    not_found=0;
                }
                j++;
            }

            if(not_found){
                res.push_back(temp);
            }            
        }

        //on ordonne par date de debut croissante
       vector <schedule> res_t(res.size());

        taille=res.size();
        for(int i=0; i < taille; i++){
            schedule temp= res.front();
            res_t[i]=temp;
            res.pop_front();
            //res.push_back(temp);
        }

        //quicksort
        quickSort(res_t,0, taille-1);
        
        for(int i=0; i < taille; i++){
            res.push_back(res_t[i]);
        }

        cout << "MIP end" <<endl;
        
    }
    catch (IloException& ex) {
        cerr << "Error: " << ex << endl;
    }
    catch (...) {
        cerr << "Error" << endl;
    }
    env.end();
}

void solveMIP_basic(vector< vector < vector <int> > > userToSat, vector< vector < vector <int> > > antennaToSat, vector< vector <schedule> > creneaux, vector<float> contacts, defined_data its_data,list<schedule> res)
{
    IloEnv env;
    try {
        
        IloModel model(env);
        IloCplex cplex(model);

        NumVar3Matrix x(env, its_data.nbUsers);
        NumVar3Matrix g(env, its_data.nbSites*its_data.nbAntennas);

        for(int i=0; i<its_data.nbUsers; i++){
            x[i]=NumVarMatrix(env, its_data.nbSatellites);

            for(int j=0; j< its_data.nbSatellites; j++){
                x[i][j]=IloNumVarArray(env, its_data.nb_contacts,0, 1, ILOINT);

                for (int k=0; k < its_data.nb_contacts; k++){
                        //cout << "solver " << endl;
                        
                        if(userToSat[i][j][k]==0){
                            //cout << "solver 0" << endl;
                            model.add(x[i][j][k]==0);
                            //cout << "solver 0 end" << endl;

                        }
                  
                }

            }
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
        

        /*for (int k=0; k < its_data.nb_contacts; k++){
            
            for(int j=0; j< its_data.nbSatellites; j++){
                
                for(int i=0; i<its_data.nbUsers; i++){
                    
                    x[i]=NumVarMatrix(env, its_data.nbSatellites);

                    x[i][j]=IloNumVarArray(env, its_data.nb_contacts,0, 1, ILOINT);

                    if(userToSat[i][j][k]==0){
                            //cout << "solver 0" << endl;
                            model.add(x[i][j][k]==0);
                            //cout << "solver 0 end" << endl;
                    }
                
                }

                for(int i=0; i<its_data.nbSites*its_data.nbAntennas; i++){

                    g[i]=NumVarMatrix(env, its_data.nbSatellites);
                    g[i][j]=IloNumVarArray(env, its_data.nb_contacts, 0, 1, ILOINT);

                    if(antennaToSat[i][j][k]==0){
                        model.add(g[i][j][k]==0);
                    }
                }
            }
        }*/

        cout << "nbcontacts : " << its_data.nb_contacts << endl;
        cout << "nbUsers : " << its_data.nbUsers << endl;
        cout << "nbSatellites : " << its_data.nbSatellites << endl;
        cout << "nbAntennes total : " << its_data.nbSites * its_data.nbAntennas << endl;
        cout << "dimensions des variables" << endl;
        cout << "dimensions de x : " << its_data.nbUsers << "*"<< its_data.nbSatellites << "*" << its_data.nb_contacts <<endl;
        cout << "dimensions de g : " << its_data.nbSites * its_data.nbAntennas << "*"<< its_data.nbSatellites<< "*" << its_data.nb_contacts <<endl;
        cout << "total nb variables : " <<  its_data.nbSites * its_data.nbAntennas *its_data.nbSatellites*its_data.nb_contacts + its_data.nbUsers*its_data.nbSatellites*its_data.nb_contacts << endl;

        /////////////////////////////constraints
        cout << "adding constraints to solver" << endl;

        //somme sur a de g(a,sat,c) >= x(u,sat,c) 

        for (int a=0; a< its_data.nbSatellites; a++){
            
            for (int j=0; j < its_data.nb_contacts; j++){
                IloNumExpr temp(env);
                for (int k=0; k < its_data.nbSites*its_data.nbAntennas ; k++){
                            //cout << g[k][a][j] << endl;

                    temp += g[k][a][j];
                    //model.add(temp += antennaToSat[k][a][j]);
                    //cout << "solver C1" << endl;
                }

                for (int o=0; o < its_data.nbUsers; o++){
                    model.add(temp >= x[o][a][j]);
                    //cout << "solver C1b" << endl;
                    
                }
                temp.end();

            }
        }
                cout << "adding constraints to solver" << endl;

        //somme sur sat de g(a,sat,c) <= 1
        for (int a=0; a< its_data.nbSites*its_data.nbAntennas; a++){
            
            for (int j=0; j < its_data.nb_contacts; j++){
                IloNumExpr temp1(env);
                for (int k=0; k < its_data.nbSatellites ; k++){
                
                    temp1= temp1 + g[a][k][j];
                }
                
                model.add(temp1 <= 1);
                temp1.end();
            }
        }

        //somme sur sat de x(u,sat,c) <= 2
        cout << "adding constraints to solver" << endl;

        for (int a=0; a< its_data.nbUsers; a++){
            
            for (int j=0; j < its_data.nb_contacts; j++){
                IloNumExpr temp2(env);
                for (int k=0; k < its_data.nbSatellites ; k++){
                
                    temp2 = temp2 + x[a][k][j];
                }
                
                model.add(temp2 <= 2);
                temp2.end();
            }
        }
        cout << "adding constraints to solver" << endl;

        IloNumExpr temp3(env);
        //max de somme sur sat sur c et sur u de x(u,s,c)
        for (int a=0; a< its_data.nbUsers; a++){
            
            for (int j=0; j < its_data.nb_contacts; j++){
                
                for (int k=0; k < its_data.nbSatellites ; k++){
                    /*if(j+1 < its_data.nb_contacts){
                        temp3 = temp3 + (x[a][k][j] *(contacts[j+1] - contacts[j]));
                    }
                    */
                    temp3=temp3+x[a][k][j];
                }                
            }
        }

        model.add (IloMaximize(env, temp3 ));
        temp3.end();
        
        cout << "solver ready" << endl;

        // Optimize
        //IloCplex cplex(model);
        // time out
        cplex.setParam(IloCplex::TiLim, 200);
        // duality gap set to 2%    
        //cplex.setParam(IloCplex::EpGap, 0.01);     
        if(cplex.solve()){
            cout << "feasible" << endl;
        }else{
            cout << "cplex status : " << cplex.getStatus() << endl;
        }
        cplex.exportModel("model.lp");
        

        env.out() << "Obj : " << cplex.getObjValue() << endl;

        for (int i=0; i< its_data.nbUsers; i++){
            
            for (int j =0; j < its_data.nbSatellites; j++){
                
                int k=0;
                while (k < its_data.nb_contacts){

                    if(cplex.getValue(x[i][j][k]) == 1){
                        schedule slot;
                        slot.sat=j;
                        slot.s=get_start_t(k, contacts);

                        while(cplex.getValue(x[i][j][k]) == 1){
                            k++;
                        }
                        slot.e=get_end_t(k, contacts);
                        res.push_back(slot);
                        //cout << "get res values " <<  endl;
                        
                    }else{
                        k++;
                    }
  
                }
            }
        }  
//on élimine les doublons
        vector <schedule> res_v(res.size());

        int taille=res.size();
        for(int i=0; i < taille; i++){
            schedule temp= res.front();
            res_v[i]=temp;
            res.pop_front();
            res.push_back(temp);
        }

        int taille_ss=res_v.size();
        for(int i=0; i < taille; i ++){
            schedule temp=res.front();
            res.pop_front();
            int j=i+1;
            int not_found=1;

            while(j < taille_ss && not_found){
                if(temp.e==res_v[j].e && temp.s==res_v[j].s && temp.sat==res_v[j].sat){
                    not_found=0;
                }
                j++;
            }

            if(not_found){
                res.push_back(temp);
            }            
        }

        //on ordonne par date de debut croissante
       vector <schedule> res_t(res.size());

        taille=res.size();
        for(int i=0; i < taille; i++){
            schedule temp= res.front();
            res_t[i]=temp;
            res.pop_front();
            //res.push_back(temp);
        }

        //quicksort
        quickSort(res_t,0, taille-1);
        
        for(int i=0; i < taille; i++){
            res.push_back(res_t[i]);
        }

        cout << "MIP end" <<endl;
        
    }
    catch (IloException& ex) {
        cerr << "Error: " << ex << endl;
    }
    catch (...) {
        cerr << "Error" << endl;
    }
    env.end();
}

void solveMIP_time(vector< vector < vector <int> > > userToSat, vector< vector < vector <int> > > antennaToSat, vector< vector <schedule> > creneaux, vector<float> contacts, defined_data its_data,list<schedule> res)
{
    IloEnv env;
    try {
        
        IloModel model(env);
        IloCplex cplex(model);

        NumVar3Matrix x(env, its_data.nbUsers);
        NumVar3Matrix g(env, its_data.nbSites*its_data.nbAntennas);

        NumVarMatrix y(env, its_data.nbUsers);

        for(int i=0; i<its_data.nbUsers; i++){
            x[i]=NumVarMatrix(env, its_data.nbSatellites);
            y[i]=IloNumVarArray(env, its_data.nb_contacts);

            for(int j=0; j< its_data.nbSatellites; j++){
                x[i][j]=IloNumVarArray(env, its_data.nb_contacts,0, IloInfinity, ILOINT);

                for (int k=0; k < its_data.nb_contacts; k++){
                        //cout << "solver " << endl;
                        
                        if(userToSat[i][j][k]==0){
                            //cout << "solver 0" << endl;
                            model.add(x[i][j][k]==0);
                            //cout << "solver 0 end" << endl;

                        }
                y[i][k]=IloNumVar(env, 0, IloInfinity, ILOINT);
                }

            }
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
        

        cout << "nbcontacts : " << its_data.nb_contacts << endl;
        cout << "nbUsers : " << its_data.nbUsers << endl;
        cout << "nbSatellites : " << its_data.nbSatellites << endl;
        cout << "nbAntennes total : " << its_data.nbSites * its_data.nbAntennas << endl;
        cout << "dimensions des variables" << endl;
        cout << "dimensions de x : " << its_data.nbUsers << "*"<< its_data.nbSatellites << "*" << its_data.nb_contacts <<endl;
        cout << "dimensions de g : " << its_data.nbSites * its_data.nbAntennas << "*"<< its_data.nbSatellites<< "*" << its_data.nb_contacts <<endl;
        cout << "dimensions de y : " << its_data.nbUsers << "*"<< its_data.nb_contacts <<endl;
        cout << "total nb variables : " << its_data.nbUsers* its_data.nb_contacts + its_data.nbSites * its_data.nbAntennas *its_data.nbSatellites*its_data.nb_contacts + its_data.nbUsers*its_data.nbSatellites*its_data.nb_contacts << endl;

        /////////////////////////////constraints
        cout << "adding constraints to solver" << endl;

        //somme sur a de g(a,sat,c)*(e-s) >= x(u,sat,c) 

 //////////  
        for (int j=0; j < its_data.nb_contacts; j++){

            for (int a=0; a< its_data.nbSatellites; a++){

                IloNumExpr temp(env);
                for (int k=0; k < its_data.nbSites*its_data.nbAntennas ; k++){


                    if(j+1 < its_data.nb_contacts){
                        temp += g[k][a][j] * (contacts[j+1] - contacts[j]);
                    }
                    //model.add(temp += antennaToSat[k][a][j]);
                    //cout << "solver C1" << endl;
                }

                for (int o=0; o < its_data.nbUsers; o++){
                    model.add(temp >= x[o][a][j]);
                    //cout << "solver C1b" << endl;
                    
                }
                temp.end();

            }
        }
                cout << "adding constraints to solver" << endl;

        //somme sur sat de g(a,sat,c) <= 1
//////
         for (int j=0; j < its_data.nb_contacts; j++){

            for (int a=0; a< its_data.nbSites*its_data.nbAntennas; a++){

                IloNumExpr temp1(env);
                for (int k=0; k < its_data.nbSatellites ; k++){
                
                    temp1= temp1 + g[a][k][j];
                }
                
                model.add(temp1 <= 1);
                temp1.end();
            }
        }

        // y(u,c) <= 2*(e-s)
        cout << "adding constraints to solver" << endl;
//////
            
        for (int j=0; j < its_data.nb_contacts; j++){      
           
            if(j+1 < contacts.size()){

                for (int a=0; a< its_data.nbUsers; a++){ 
        
                    model.add(y[a][j] <= 2*(contacts[j+1]-contacts[j]));
                }   
            }
        }


        //somme sur sat de x(u,sat,c) >= y(u,c)
        cout << "adding constraints to solver" << endl;

////////

        for (int j=0; j < its_data.nb_contacts; j++){

            for (int a=0; a< its_data.nbUsers; a++){


                IloNumExpr temp2(env);

                for (int k=0; k < its_data.nbSatellites ; k++){
                
                    temp2 = temp2 + x[a][k][j];
                }
                
                model.add(temp2 >= y[a][j]);
                                        temp2.end();

            }
        }



        cout << "adding constraints to solver" << endl;

        IloNumExpr temp3(env);
        //max de somme sur c et sur u de y(u,c)

        for (int j=0; j < its_data.nb_contacts; j++){
            for (int a=0; a< its_data.nbUsers; a++){


                temp3=temp3+y[a][j];
                                
            }
        }

        model.add (IloMaximize(env, temp3 ));
        temp3.end();
        
        cout << "solver ready" << endl;

        // Optimize
        //IloCplex cplex(model);
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

        for (int i=0; i< its_data.nbUsers; i++){
            
            for (int j =0; j < its_data.nbSatellites; j++){
                
                int k=0;
                while (k < its_data.nb_contacts){

                    if(cplex.getValue(x[i][j][k]) == 1){
                        schedule slot;
                        slot.sat=j;
                        slot.s=get_start_t(k, contacts);

                        while(cplex.getValue(x[i][j][k]) == 1){
                            k++;
                        }
                        slot.e=get_end_t(k, contacts);
                        res.push_back(slot);
                        //cout << "get res values " <<  endl;
                        
                    }else{
                        k++;
                    }
  
                }
            }
        }    

        //on enlève les doublons 

        vector <schedule> res_v(res.size());

        int taille=res.size();
        for(int i=0; i < taille; i++){
            schedule temp= res.front();
            res_v[i]=temp;
            res.pop_front();
            res.push_back(temp);
        }

        int taille_ss=res_v.size();
        for(int i=0; i < taille; i ++){
            schedule temp=res.front();
            res.pop_front();
            int j=i+1;
            int not_found=1;

            while(j < taille_ss && not_found){
                if(temp.e==res_v[j].e && temp.s==res_v[j].s && temp.sat==res_v[j].sat){
                    not_found=0;
                }
                j++;
            }

            if(not_found){
                res.push_back(temp);
            }            
        }
//on ordonne par date de debut croissante
       vector <schedule> res_t(res.size());

        taille=res.size();
        for(int i=0; i < taille; i++){
            schedule temp= res.front();
            res_t[i]=temp;
            res.pop_front();
            //res.push_back(temp);
        }

        //quicksort
        quickSort(res_t,0, taille-1);
        
        for(int i=0; i < taille; i++){
            res.push_back(res_t[i]);
        }



        cout << "MIP end" <<endl;
    get_plan(res);

    }
    catch (IloException& ex) {
        cerr << "Error: " << ex << endl;
    }
    catch (...) {
        cerr << "Error" << endl;
    }
    env.end();
}

//ajouter contact dans param
void solveMIP_ssc(vector< vector < vector <int> > > userToSat, vector< vector < vector <int> > > antennaToSat, vector< vector <schedule> > creneaux, vector<float> contacts, defined_data its_data,int contact)
{
    IloEnv env;
    try {
        
        IloModel model(env);
        IloCplex cplex(model);

        NumMatrix t(env, its_data.nbUsers);
        NumVarMatrix g(env, its_data.nbSites*its_data.nbAntennas);

        IloNumVarArray tau(env, its_data.nbUsers, 0, 2, ILOFLOAT);

        for(int i=0; i<its_data.nbUsers; i++){
            t[i]=IloNumArray(env, its_data.nbSatellites, 0, 1, ILOFLOAT);

            for(int j=0; j < its_data.nbSatellites; j++){

                if(userToSat[i][j][contact]==0){
                    t[i][j]=0;
                }else{
                    t[i][j]=1;
                }
            }
        }

        for(int i=0; i<its_data.nbSites*its_data.nbAntennas; i++){
                g[i]=IloNumVarArray(env, its_data.nbSatellites, 0, 1, ILOINT);
            
            for(int j=0; j< its_data.nbSatellites; j++){                
                        //cout << "solver " << endl;

                    //cout << "solver 2" << endl;
                    if(antennaToSat[i][j][contact]==0){
                        model.add(g[i][j]==0);
                                      
                    }

                
            }
        cout << "passe n°" << i << endl;

        }
        

        cout << "nbUsers : " << its_data.nbUsers << endl;
        cout << "nbSatellites : " << its_data.nbSatellites << endl;
        cout << "nbAntennes total : " << its_data.nbSites * its_data.nbAntennas << endl;
        cout << "dimensions des variables" << endl;
        cout << "dimensions de t : " << its_data.nbUsers << "*"<< its_data.nbSatellites<<endl;
        cout << "dimensions de g : " << its_data.nbSites * its_data.nbAntennas << "*"<< its_data.nbSatellites<< endl;
        cout << "dimensions de tau : " << its_data.nbUsers << endl;
        cout << "total nb variables : " << its_data.nbUsers+ its_data.nbSites * its_data.nbAntennas *its_data.nbSatellites << endl;

        /////////////////////////////constraints
             int a;
        cin>> a;
                cout << "adding constraints to solver" << endl;

        //somme sur sat de g(a,sat) <= 1
//////
            for (int k=0; k < its_data.nbSatellites ; k++)
            {

                IloExpr temp1(env);
                for (int a=0; a< its_data.nbSites*its_data.nbAntennas; a++){
                
                    temp1= temp1 + g[a][k];
                }
                
                model.add(temp1 <= 1);
                temp1.end();
            }
        
  
        // tau(u) <= 2
        cout << "adding constraints to solver" << endl;
//////          

        cin>> a;

                for (int a=0; a< its_data.nbUsers; a++){ 
        
                    model.add(tau[a] <= 2);
                }   
        



        // tau(u) <= somme sur sat et sur a de g(a,s) * t(u,s)
        cout << "adding constraints to solver" << endl;

////////


        cin>> a;

            for (int a=0; a< its_data.nbUsers; a++){
                IloExpr temp2(env);

                for(int i=0; i < its_data.nbSites*its_data.nbAntennas; i++){


                    for (int k=0; k < its_data.nbSatellites ; k++){
                    
                        temp2 = temp2 + g[i][k] * t[a][k];
                        cout <<" a, " << a << "i " << i << "k" << k<<endl;
                    }
                }
                model.add(tau[a] <= temp2);
                                temp2.end();

            }
        



        cout << "adding constraints to solver" << endl;

        /*IloNumExpr temp3(env);
        //max de somme sur u des tau de u 

            for (int a=0; a< its_data.nbUsers; a++){


                temp3=temp3+tau[a];
                                
            }
        */

        model.add (IloMaximize(env, IloSum(tau) ));
        //temp3.end();
        
        cout << "solver ready" << endl;

        cplex.exportModel("model.lp");

        // Optimize
        //IloCplex cplex(model);
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
        

        env.out() << "Obj : " << cplex.getObjValue() << endl;
        vector<int> res;
        for (int i=0; i< its_data.nbSites*its_data.nbAntennas; i++){
            
            for (int j =0; j < its_data.nbSatellites; j++){
                

                    if(cplex.getValue(g[i][j]) == 1){
                        res.push_back(j);

  
                }
            }
        }    

        cout << "MIP end" <<endl;

        for(int i=0; i < res.size(); i++){
            cout << res[i] <<endl;
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