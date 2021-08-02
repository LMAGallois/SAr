#include "solver_sites_60.h"
#include <iostream>
#include <string>

using namespace std;

typedef IloArray<IloNumVarArray> NumVarMatrix;
typedef IloArray<NumVarMatrix> NumVar3Matrix;
typedef IloArray<IloNumArray> NumMatrix;

ILOSTLBEGIN

void get_plan_checker_60(list<schedule_ssc> plan){
    //cout << "coucou" << endl;
    const char* output_file = "in_out/plan_checker_60.txt";
    ofstream output(output_file, ios::app);

    int taille=plan.size();
    //output << taille <<endl;
    for(int it=0; it < taille ; it++){
        schedule_ssc temp= plan.front();
        output << temp.s << endl;
        output << temp.e << endl;
        output << temp.sat << endl;
        output << temp.site << endl;
        output << temp.antenna << endl;
        output << temp.contact1 << endl;
        output << temp.contact11 << endl;
        output << temp.contact111 << endl;

        plan.pop_front();
    }
}

vector<schedule_ssc> solveMIP_ssc_60(vector< vector <float> > contacts_sites, vector< vector < vector <float> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, defined_data its_data,int contact1, int contact2)
{
    vector<schedule_ssc> ret;
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
                if(userToSat[i][j][contact1]==0){
                    t[i][j]=0;
                }else{
                    t[i][j]=1;
                }
            }
        }

        for(int i=0; i<its_data.nbSites*its_data.nbAntennas; i++){
            g[i]=IloNumVarArray(env, its_data.nbSatellites, 0, 1, ILOINT);
            
            for(int j=0; j< its_data.nbSatellites; j++){                

                if(antennaToSat[i][j][contact1]==0){
                    model.add(g[i][j]==0);
                                    
                }
            }
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
        cout << "adding constraints to solver" << endl;

        //somme sur sat de g(a,sat) <= 1
        for (int a=0; a< its_data.nbSites*its_data.nbAntennas; a++){

            IloExpr temp1(env);
            for (int k=0; k < its_data.nbSatellites ; k++){
                temp1= temp1 + g[a][k];
            }
        
            model.add(temp1 <= 1);
            temp1.end();
        }

        //somme sur a de g(a,sat) <= 1
        for (int a=0; a< its_data.nbSatellites ; a++){

            IloExpr temp3(env);
            for (int k=0; k < its_data.nbSites*its_data.nbAntennas ; k++){
                temp3= temp3 + g[k][a];
            }
        
            model.add(temp3 <= 1);
            temp3.end();
        }

        // tau(u) <= 2
        cout << "adding constraints to solver" << endl;

        for (int a=0; a< its_data.nbUsers; a++){ 
            model.add(tau[a] <= 2);
        }   
        // tau(u) <= somme sur sat et sur a de g(a,s) * t(u,s)
        cout << "adding constraints to solver" << endl;

        for (int a=0; a< its_data.nbUsers; a++){
            IloExpr temp2(env);

            for(int i=0; i < its_data.nbSites*its_data.nbAntennas; i++){


                for (int k=0; k < its_data.nbSatellites ; k++){
                
                    temp2 = temp2 + g[i][k] * t[a][k];
                }
            }
            model.add(tau[a] <= temp2);
            temp2.end();

        }

        /*IloExpr temp4(env);
            for(int i=0; i < its_data.nbSites*its_data.nbAntennas; i++){

                for(int j=0; j< its_data.nbSatellites; j++){
                    d[i][j]
                }
            }


        temp4.end();

        (1400/(1400+400)*IloSum(tau)) + (400/(1400+400)*)*/
        cout << "adding constraints to solver" << endl;
        model.add (IloMaximize(env, IloSum(tau) ));
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
        list<schedule_ssc> res;
        for (int i=0; i< its_data.nbSites*its_data.nbAntennas; i++){
            
            for (int j =0; j < its_data.nbSatellites; j++){          

                    if(cplex.getValue(g[i][j]) == 1){
                        schedule_ssc slot;
                        slot.sat=j;
                        slot.site=i/4 ;
                        slot.antenna =i%4;
                        slot.contact1=contact1;
                        slot.contact11=0;
                        slot.contact111=0;
                        slot.s=contacts_sites[contact1][0];
                        slot.e=contacts_sites[contact2][0];
                        res.push_back(slot);
                        ret.push_back(slot);
                }
            }
        }    
        cout << "MIP end" <<endl;
        get_plan_checker_60(res);
    }
    catch (IloException& ex) {
        cerr << "Error: " << ex << endl;
    }
    catch (...) {
        cerr << "Error" << endl;
    }
    env.end();

    return ret;
}