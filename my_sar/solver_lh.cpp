#include "solver_lh.h"
#include <iostream>
#include <string>

using namespace std;

typedef IloArray<IloNumVarArray> NumVarMatrix;
typedef IloArray<NumVarMatrix> NumVar3Matrix;

ILOSTLBEGIN

void get_plan_checker_lh(list<schedule_ssc> plan){
    //cout << "coucou" << endl;
    const char* output_file = "in_out/plan_checker_lh.txt";
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

vector<schedule_ssc> solveMIP_ssc_lh(vector< vector < vector<float> > > contacts_sites, vector <vector < vector <float> > > &userToSat, vector< vector < vector <int> > > &antennaToSat, defined_data its_data)
{
    vector<schedule_ssc> ret;
    IloEnv env;
    try {
        
        IloModel model(env);
        IloCplex cplex(model);

        IloArray<IloNumArray> w(env, its_data.nbSatellites);
        NumVar3Matrix x(env, its_data.nbSatellites);

        for(int i=0; i<its_data.nbSatellites; i++){
            w[i]=IloNumArray(env, contacts_sites.size(), 0, 1, ILOINT);


            for(int j=0; j < contacts_sites.size(); j++){
                int count=0;
                
                for(int k=0; k< its_data.nbUsers; k++){
                
                    if(userToSat[k][i][j]==1){
                        count++;
                    }
                }
                w[i][j]=count;
            }
        }
        for(int i=0; i<its_data.nbSatellites; i++){
            x[i]=NumVarMatrix(env,contacts_sites.size());
            
            for(int j=0; j< contacts_sites.size(); j++){                
                x[i][j]=IloNumVarArray(env, its_data.nbSites, 0, 1, ILOINT);

                for(int k=0; k<its_data.nbSites ; k++){
                    int temp=k*4;
                    if(antennaToSat[temp][i][j]==0){
                        model.add(x[i][j][k]==0);             
                    }
                }  
            }
        }

        cout << "dimensions des variables" << endl;
        cout << "dimensions de w : " <<  contacts_sites.size()  << "*"<< its_data.nbSatellites<<endl;
        cout << "dimensions de x : " << its_data.nbSites << "*"<< its_data.nbSatellites << "*"<< contacts_sites.size() <<endl;
        cout << "total nb variables : " << its_data.nbSites *  contacts_sites.size()  *its_data.nbSatellites +  contacts_sites.size() * its_data.nbSatellites << endl;

        /////////////////////////////constraints
        cout << "adding constraints to solver" << endl;

        //somme sur sat de x(sat, c, site) <= 4
        for (int b=0; b< contacts_sites.size(); b++){

            for (int a=0; a< its_data.nbSites; a++){
                IloExpr temp1(env);

                for (int k=0; k < its_data.nbSatellites ; k++){
                    temp1= temp1 + x[k][b][a];
                }
            
                model.add(temp1 <= 4);
                temp1.end();
            }
        }

        //somme sur site de g(sat, c, site) <= 1
        for (int b=0; b< contacts_sites.size(); b++){

            for (int a=0; a< its_data.nbSatellites ; a++){
                IloExpr temp3(env);

                for (int k=0; k < its_data.nbSites ; k++){
                    temp3= temp3 + x[a][b][k];
                }
            
                model.add(temp3 <= 1);
                temp3.end();
            }
        }

        /////////////////////////////objectif
        
        IloExpr temp4(env);

        for (int b=0; b< contacts_sites.size(); b++){

            for (int a=0; a< its_data.nbSatellites ; a++){

                for (int k=0; k < its_data.nbSites ; k++){
                    temp4= temp4 + w[a][b] * x[a][b][k];
                } 
            }
        }

        cout << "adding constraints to solver" << endl;
        model.add (IloMaximize(env, temp4 ));
        temp4.end();
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
        vector<int> antennas(its_data.nbAntennas);

        for (int b=0; b< contacts_sites.size()-1; b++){
            antennas.clear();

            for (int i=0; i< its_data.nbSites; i++){
                
                for (int j =0; j < its_data.nbSatellites; j++){          
                    if(cplex.getValue(x[j][b][i]) == 1){
                        schedule_ssc slot;
                        slot.sat=j;
                        slot.site=i ;
                        slot.contact1=b;
                        slot.contact11=0;
                        slot.contact111=0;
                        slot.s=contacts_sites[b][0][0];
                        slot.e=contacts_sites[b+1][0][0];
                        res.push_back(slot);
                        ret.push_back(slot);

                        int index=0;
                        while(index < antennas.size() && antennas[index]==-1){
                            index++;
                        }

                        if(antennas[index]==0){
                            slot.antenna =index;
                        }   
                    
                    }
                }
            } 
        }   
        cout << "MIP end" <<endl;
        get_plan_checker_lh(res);
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