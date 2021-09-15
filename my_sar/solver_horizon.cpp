#include "solver_horizon.h"
#include "solver_sites.h"
#include <iostream>
#include <string>

using namespace std;

typedef IloArray<IloNumVarArray> NumVarMatrix;
typedef IloArray<NumVarMatrix> NumVar3Matrix;
typedef IloArray<IloNumArray> NumMatrix;
typedef IloArray<NumMatrix> Num3Matrix;

ILOSTLBEGIN

void get_plan_checker_horizon(list<schedule_ssc> plan){
    //cout << "coucou" << endl;
    const char* output_file = "in_out/plan_checker_horizon.txt";
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

void solveMIP_ssc_horizon(vector< vector < vector<float> > > contacts_sites, vector< vector < vector <float> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, defined_data its_data)
{
    IloEnv env;
    try {
       
        IloModel model(env);
        IloCplex cplex(model);

        Num3Matrix t(env, its_data.nbUsers);

        NumVar3Matrix g_1(env, its_data.nbSites*its_data.nbAntennas);
        NumVar3Matrix d(env, its_data.nbSites*its_data.nbAntennas);
        NumVar3Matrix g(env, its_data.nbSites*its_data.nbAntennas);

        NumVarMatrix tau(env, its_data.nbUsers);

        for(int k=0; k< its_data.nbUsers; k++){
            tau[k]=IloNumVarArray(env, contacts_sites.size(), 0, 2, ILOFLOAT);
        }

        for(int i=0; i<its_data.nbUsers; i++){

            t[i]=NumMatrix(env, its_data.nbSatellites);

            for(int j=0; j < its_data.nbSatellites; j++){
                t[i][j]=IloNumArray(env, contacts_sites.size(), 0, 1, ILOFLOAT);

                for(int k=0; k< contacts_sites.size(); k++){

                    if(userToSat[i][j][k]==0){
                        t[i][j][k]=0;
                    }else{
                        t[i][j][k]=1;
                    }
                }
            }
        }

        for(int i=0; i<its_data.nbSites*its_data.nbAntennas; i++){
            g[i]=NumVarMatrix(env, its_data.nbSatellites);
            
            for(int j=0; j< its_data.nbSatellites; j++){                
                g[i][j]=IloNumVarArray(env, contacts_sites.size() , 0, 1, ILOINT);

                for(int k=0; k< contacts_sites.size(); k++){

                    if(antennaToSat[i][j][k]==0){
                        model.add(g[i][j][k]==0);               
                    }
                }
            }
        }

        for(int i=0; i<its_data.nbSites*its_data.nbAntennas; i++){
            g_1[i]=NumVarMatrix(env, its_data.nbSatellites);
            
            for(int j=0; j< its_data.nbSatellites; j++){                
                g_1[i][j]=IloNumVarArray(env, contacts_sites.size() , 0, 1, ILOINT);

                for(int k=0; k< contacts_sites.size(); k++){
                    
                    if(k!=0){
                        model.add(g_1[i][j][k]==g[i][j][k-1]);               
                    }                
                }
            }
        }
        
        for(int i=0; i<its_data.nbSites*its_data.nbAntennas; i++){
            d[i]=NumVarMatrix(env, its_data.nbSatellites);
        cout <<"coucou"  <<i<<endl;

            for(int j=0; j< its_data.nbSatellites; j++){   
                d[i][j]=IloNumVarArray(env,contacts_sites.size(), 0, 1, ILOINT);

                for(int k=0; k< contacts_sites.size(); k++){          
                
                    if(k!=0){
                        model.add(d[i][j][k]==g[i][j][k]*g_1[i][j][k]);
                    }
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

        //somme sur sat de g(a,sat,c) <= 1
        for(int l=0; l< contacts_sites.size(); l++){
            for (int a=0; a< its_data.nbSites*its_data.nbAntennas; a++){
                IloExpr temp1(env);
                for (int k=0; k < its_data.nbSatellites ; k++){
                        temp1= temp1 + g[a][k][l];
                    
                }
            
                model.add(temp1 <= 1);
                temp1.end();
            }
        }

        //somme sur a de g(a,sat,c) <= 1
        for(int l=0; l< contacts_sites.size(); l++){
            for (int a=0; a< its_data.nbSatellites ; a++){

                IloExpr temp3(env);
                for (int k=0; k < its_data.nbSites*its_data.nbAntennas ; k++){
                    temp3= temp3 + g[k][a][l];
                }
            
                model.add(temp3 <= 1);
                temp3.end();
            }
        }

        // tau(u,c) <= 2
        cout << "adding constraints to solver" << endl;
        for(int l=0; l< contacts_sites.size(); l++){

            for (int a=0; a< its_data.nbUsers; a++){ 
                model.add(tau[a][l] <= 2);
            }  
        } 
        // tau(u,c) <= somme sur sat et sur a de g(a,s,c) * t(u,s,c)
        cout << "adding constraints to solver" << endl;
        for(int l=0; l< contacts_sites.size(); l++){
            for (int a=0; a< its_data.nbUsers; a++){
                IloExpr temp2(env);

                for(int i=0; i < its_data.nbSites*its_data.nbAntennas; i++){


                    for (int k=0; k < its_data.nbSatellites ; k++){
                    
                        temp2 = temp2 + g[i][k][l] * t[a][k][l];
                    }
                }
                model.add(tau[a][l] <= temp2);
                temp2.end();

            }
        }
            cout << "adding constraints to solver" << endl;

        float alpha =0.5;
        float beta=0.5;
        IloExpr temp4(env);
        for(int l=0; l< contacts_sites.size(); l++){
            for(int i=0; i<its_data.nbSites*its_data.nbAntennas; i++){

                for(int j=0; j< its_data.nbSatellites; j++){                
                    temp4=temp4+d[i][j][l];
                }
            }
        }   

        IloExpr temp5(env);
        for(int l=0; l< contacts_sites.size(); l++){

            for (int a=0; a< its_data.nbUsers; a++){ 
                temp5=temp5+tau[a][l];
            }  
        }

        model.add (IloMaximize(env, ((alpha*temp5) + (beta*temp4)) ));
        temp4.end();
        temp5.end();
        cout << "solver ready" << endl;
        
        //cplex.exportModel("model.lp");
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
        for(int l=0; l< contacts_sites.size(); l++){
            for (int i=0; i< its_data.nbSites*its_data.nbAntennas; i++){
                
                for (int j =0; j < its_data.nbSatellites; j++){          

                        if(cplex.getValue(g[i][j][l]) == 1){
                            schedule_ssc slot;
                            slot.sat=j;
                            slot.site=i/4 ;
                            slot.antenna =i%4;
                            slot.contact1=l;
                            slot.contact11=0;
                            slot.contact111=0;
                            slot.s=contacts_sites[l][0][0];
                            slot.e=contacts_sites[l+1][0][0];
                            res.push_back(slot);
                    }
                }
            }
        }    
        cout << "MIP end" <<endl;
        get_plan_checker_horizon(res);
    }
    catch (IloException& ex) {
        cerr << "Error: " << ex << endl;
    }
    catch (...) {
        cerr << "Error" << endl;
    }
    env.end();

}