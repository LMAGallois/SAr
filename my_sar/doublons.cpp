/*
fonction pour tester la suppression des doubles dans les contacts (fichiers précisés dans le code)
*/

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <fstream>
#include <map>

using namespace std;

int main(){

    const char* cfile_conf = "in_out/contacts_user.txt";
    ifstream file(cfile_conf);
    
    if ( !file ) {
    cerr << "No such file: " << cfile_conf << endl;
    throw(-1);
    }
    const char* cfile = "in_out/contacts_users3.txt";

    ofstream file_output(cfile);
    float temp;
    list<float> contacts_temp;

    for(int i=0; i < 788073; i++){
        file >> temp;
        contacts_temp.push_back(temp);
    }

   int taille=contacts_temp.size();    
    float memo1=-1;
    
    for(int it=0; it < taille ; it++){   
        float temp1=contacts_temp.front();
        contacts_temp.pop_front();
        float temp2=contacts_temp.front();
        
        if(temp1==temp2){
            
            if(temp1!=memo1){
                contacts_temp.push_back(temp1);
                memo1=temp1;
            }
        }else{
            
            if(temp1!=memo1){
                contacts_temp.push_back(temp1);
                contacts_temp.push_back(temp2);
            }else{
                contacts_temp.push_back(temp2);
            }
            memo1=temp2;
        }
    }
    contacts_temp.pop_front();
    contacts_temp.sort();
    taille=contacts_temp.size();

for(int i=0; i < taille; i++){
        file_output << contacts_temp.front() <<endl;
        contacts_temp.pop_front();
    }
    return 0;
}

