#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <fstream>
#include <limits.h>
#include "parser_sites.h"
#include "parser.h"
#include "parser_instance.h"
#include <math.h>
#include "lagrange.h"


using namespace std;
#define PI 3.14159265

void display_array_h(vector<v_3> &matrice){
    for (int i=0; i < matrice.size(); i++){
        cout << "contacts n° " << i << "--"<<matrice[i].x<<" , "<<matrice[i].y<<" , "<<matrice[i].z<<  endl; 
        cout << endl;
    }
}

void display_3Dmatrix_h(vector< vector < vector<int> > > &matrice, defined_data its_data){
    for (int k=0; k < 10; k++){

        for (int i=0; i < matrice.size(); i+=4){

            for (int j=0; j < matrice[i].size(); j++){
                
                    cout << matrice[i][j][k] << " ";
                
            }
            cout << endl;
        }
    cout << endl<<endl;
    }
}

void display_3Dmatrix_hf(vector< vector < vector<float> > > &matrice, defined_data its_data){
    for (int i=0; i < matrice.size(); i++){

        for (int j=0; j < its_data.nbSatellites; j++){

            for (int k=1; k < 2; k++){
                cout << matrice[i][j][k] << "   ";
            }
        }
        cout << endl;
    }
}

void display_matrix_h(vector< vector<v_3> > &matrice){
    for (int i=0; i < matrice.size(); i++){
        cout << " satellite n°  " << i << endl;

        for (int j=0; j < matrice[i].size(); j++){
            cout << "temps" << j << endl; 
            
            cout << matrice[i][j].x << " , "<<matrice[i][j].y<<" , "<<matrice[i][j].z<<endl;
        }
        cout << endl ;
    }
}

void display_4Dmatrix_h(vector <vector< vector < vector<v_3> > > > &matrice){
    
    for (int i=0; i < matrice.size(); i++){
        cout << " sat n° " << i << endl;

        for (int j=0; j < matrice[i].size(); j++){
            
            for (int k=0; k < matrice[i][j].size(); k++){
                
                for (int l=0; l < matrice[i][j][k].size(); l++){
                    cout << "contact n° " << j << " "<<k<<" "<<l<<endl; 
                    cout << matrice[i][j][k][l].x << " "<<matrice[i][j][k][l].y << " "<<matrice[i][j][k][l].z<<endl;;
                }
            }
            cout << endl;
        }
        cout << endl << endl;
    }
}

v_3 trans_coords_u (float lat, float longi, float alt){
    // (lat, lon) in WSG-84 degrees and altitude in meters
    int R = 6378137;
   /* float f_inv = 298.257224;
    float f = 1.0 / f_inv;

    float cosLat = cos(lat * PI / 180);
    float sinLat = sin(lat * PI / 180);
    float cosLong = cos(longi * PI / 180);
    float sinLong = sin(longi * PI / 180);

    float c = 1 / sqrt(cosLat * cosLat + (1 - f) * (1 - f) * sinLat * sinLat);
    float s = (1 - f) * (1 - f) * c;

    float x = (R*c + alt) * cosLat * cosLong;
    float y = (R*c + alt) * cosLat * sinLong;
    float z = (R*s + alt) * sinLat;*/
    
    //if alt=0.0
    float Lat=lat;
    float Long= longi;
    float x=R*cos(Lat)*cos(Long);
    float y=R*cos(Lat)*sin(Long);
    float z = R*sin(Lat);

    v_3 point={x,y,z};


    
    return point;   
}

void parse_coords_u(vector<v_3> &userCoords, defined_data its_data){

    const char* cfile_u = "data/cov20.cvaa";
    const char* cfile_o = "in_out/users.txt";
    ofstream o_file(cfile_o);
    string str;
    int found;
    int nbSat;
    int c;
    float x;
    float y;
    float z;
    ifstream file(cfile_u);

    if ( !file ) {
        cerr << "No such file: " << cfile_u << endl;
        throw(-1);
    }

    found=-1;

    while(found==-1){
        file >> str;
        found=str.find("NumberOfAssets");
    }

    file >> nbSat ;
    found=-1;

    while(found==-1){
        file >> str;
        found=str.find("NumberOfRegions");
    }

    found=-1;
    c=-1;   
    int l;

    while(c < its_data.nbUsers-1){
        found=-1;
        while(found==-1)
        {
            file >> str;
            found=str.find("PointNumber");
        }
        file>>c;
        file>>str,
        file>>x;
        file>>str;
        file>>y;
        file>>str;
        file>>z;
        v_3 point=trans_coords_u(x,y,z);
        o_file<< point.x<<";"<<point.y<<";"<<point.z<<endl;

        userCoords[c]=point;

        //cout << "user "<< c <<endl;
    }
            cout << "end of parse_user_coords " <<endl;
}

void parse_site(vector<v_3> &coorSite, defined_data its_data){
    const char* cfile_u = "data/sites.txt";
    ifstream file(cfile_u);
    int found=-1;
    string str_temp;
    float x;
    float y;
    float z;

    if ( !file ) {
        cerr << "No such file: " << cfile_u << endl;
        throw(-1);
    }

    while(found==-1)
    {
        file >> str_temp;
        found=str_temp.find("coordinates");
    }

    for (int i=0; i < its_data.nbSites; i ++){
        file >> str_temp;
        file >> x;
        file >> y;
        file >> z;
        coorSite.push_back({x,y,z});
    }
            cout << "end of parse_sites " <<endl;

}

void parse_eph(vector < vector <v_3> > &trackSat, defined_data its_data, const char* path){
    string str;
    int found;
    float x=0;
    float y=0;
    float z=0;
    float clk=0;
    float heure;
    float minutes;
    int temps;
    int indexT=0;
    vector<v_3> s;
    vector <float> last_x(its_data.nbSatellites);
    vector <float> last_y(its_data.nbSatellites);
    vector <float> last_z(its_data.nbSatellites);

    for(int j=0; j < its_data.nbSatellites; j++){
        trackSat.push_back(s);

        for(int i=0; i < its_data.horizon_c/its_data.time_slot_sp3; i++){
            trackSat[j].push_back({0,0,0});
            
        }
    }
    const char* cfile_o="in_out/tracksat.txt";
    ofstream output(cfile_o);
    const char* cfile;
    int sat;
    char chemin[100];
    for(int day=0; day< 11; day++){
        for(int i=0; i< 100; i++){
            chemin[i]='\0';
        }
        
        switch (day) {
            case 0:
                strcpy(chemin, path);
                strcat(chemin, "/0.SP3");
                cfile =chemin;
                //cout <<chemin<<endl;
                //cout <<path<<endl;
                break;
            case 1:
                strcpy(chemin, path);
                strcat(chemin, "/1.SP3");
                //cout <<chemin<<endl;
                cfile =  chemin;
                break;
            case 2:
                strcpy(chemin, path);
                strcat(chemin, "/2.SP3");
                cfile =  chemin;
                break;
            case 3:
                strcpy(chemin, path);
                strcat(chemin, "/3.SP3");
                cfile =  chemin;
                break;
            case 4:
                strcpy(chemin, path);
                strcat(chemin, "/4.SP3");
                cfile =  chemin;
                break;
            case 5:
                strcpy(chemin, path);
                strcat(chemin, "/5.SP3");
                cfile = chemin;
                break;
            case 6:
                strcpy(chemin, path);
                strcat(chemin, "/6.SP3");
                cfile = chemin;
                break;
            case 7:
                strcpy(chemin, path);
                strcat(chemin, "/7.SP3");
                cfile =  chemin;
                break;
            case 8:
                strcpy(chemin, path);
                strcat(chemin, "/8.SP3");
                cfile =  chemin;
                break;
            case 9:
                strcpy(chemin, path);
                strcat(chemin, "/9.SP3");
                cfile = chemin;
                break;
            case 10:
                strcpy(chemin, path);
                strcat(chemin, "/10.SP3");
                cfile =  chemin;
                break;

                
            default:
                break;
        }

            ifstream file(cfile);
        if ( !file ) {
            cerr << "No such file: " << cfile << endl;
            throw(-1);
        }
        

        found=-1;
        while(found==-1){
            file >> str;
            found=str.find("PCV");
        }

        for(int index=0; index < (((24*3600)/its_data.time_slot_sp3)-1); index++)
        {

            found=-1;
            while(found==-1)
            {
                file >> str;
                found=str.find("*");
                if (found != -1){

                    file >> str;
                    file >> str;
                    file >> str;
                    file >> heure; 
                    file >> minutes;

                    temps=heure*3600+minutes*60;   

                }
            }
            sat=0;
            while(sat < its_data.nbSatellites)
            {
                found=-1;
                while(found==-1)
                {
                    file >> str;
                    found=str.find("PE");
                }

                file>>x;
                file>>y;
                file>>z;
                file >>clk;
                
                last_x[sat]=x;
                last_y[sat]=y;
                last_z[sat]=z;
                
                sat++;
            }

            indexT=day*86400+temps;        

            if(indexT < its_data.horizon_c){
                int time_slot=round(indexT/its_data.time_slot_sp3);
                for(int i=0; i < its_data.nbSatellites; i++){
                    trackSat[i][time_slot]={last_x[i],last_y[i],last_z[i]};
                    output<<last_x[i]<<";"<<last_y[i]<<";"<<last_z[i]<<endl;
                    //cout <<"coucou"<<last_x[i]<<" "<<last_y[i]<<" "<<last_z[i]<<endl; 
                }
                //cout <<time_slot<<endl;
            }          
        }
    }    

   // cout << "while fini" << endl;

    cout <<"----------------end parse eph----------------------------"<<endl;
}

float ele_a_20(v_3 sat , v_3 on_earth){

    float res;
    v_3 a={-sat.x, -sat.y, -sat.z};

    v_3 b={on_earth.x-sat.x, on_earth.y-sat.y, on_earth.z-sat.z};

    float nume=a.x*b.x+a.y*b.y+a.z*b.z;

    float deno=sqrt(a.x*a.x+a.y*a.y+a.z*a.z )*sqrt(b.x*b.x+b.y*b.y+b.z*b.z);

    res=acos(nume/deno) * 180.0 / PI;

    if (res <= (90.0-20.0)){
        res=1;
    }else{
        res=0;
    }
    return res;
}
//tracksat avec un max de valeurs et on compare avec les slots de contacts_sites comme ca on peut faire un modele avec userToSat continu 
void get_visi_u(vector < vector <v_3> > &trackSat, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <float> > > &u_or_aToSat, vector< vector < vector<float> > > contacts_sites){
    vector <vector <float> > temp;
    vector <float> temp2;
    float time_slot;

    for(int j=0; j < coordsOnEarth.size(); j++){
        u_or_aToSat.push_back(temp);
        for(int k=0; k< its_data.nbSatellites; k++){
            u_or_aToSat[j].push_back(temp2);
            for(int i=0; i < contacts_sites.size(); i++){
                v_3 b=coordsOnEarth[j];
                time_slot=round(contacts_sites[i][0][0]/its_data.time_slot_sp3);
                v_3 a=trackSat[k][time_slot];

                if(ele_a_20(a,b)){
                    u_or_aToSat[j][k].push_back(1);
                }else{
                    u_or_aToSat[j][k].push_back(0);
                }
            }
        }
    }
    cout <<"-------------------------get visi fin--------------------------------"<<endl;

}

void get_trackSatInterpolate (vector < vector <v_3> > &trackSat, vector < vector <v_3> > &trackSatInterpolate, defined_data its_data){
    vector<v_3> temp0;
    int ordre=4;
    float x;
    float y;
    float z;
    int index=0;
    const char* cfile_u = "in_out/trackSatInterpolate.txt";
    ofstream file(cfile_u);
    
    for(int i=0; i< its_data.nbSatellites; i++){
        trackSatInterpolate.push_back(temp0);
        for(int j=0; j < (its_data.horizon_c/60); j++){
            if( ((j*60) % 300) != 0 ){
                //rassemblement des 10 points connus 
                vector <Data> f(10);
                int g=j*60;
                g=g+ (300- (g%300));
                int k=0;

                while (g < its_data.horizon_c && k<4){
                    Data temp;
                    temp.x=g;
                    temp.y=trackSat[i][g/300].y;
                    f[k]=temp;
                    g=g+300;
                    k++;
                }
                ordre=k+1;
                y =interpolate( f, (j*60), ordre);

                ordre=4;
                g=j*60;
                g=g+ (300- g%300);
                k=0;
                while (g < its_data.horizon_c && k<4){
                    Data temp;
                    temp.x=g;
                    temp.y=trackSat[i][g/300].x;
                    f[k]=temp;
                    g=g+300;
                    k++;
                }
                ordre=k+1;

                x =interpolate( f, (j*60), ordre);

                ordre=4;
                g=j*60;
                g=g+ (300- g%300);
                k=0;
                while (g < its_data.horizon_c && k<4){
                    Data temp;
                    temp.x=g;
                    temp.y=trackSat[i][g/300].z;
                    f[k]=temp;
                    g=g+300;
                    k++;
                }
                ordre=k+1;

                z =interpolate( f, (j*60), ordre);

            }else{
                //cout <<"coucou"<<x<<" "<<y<<" "<<z<<endl;
                index=(j*60)/300;
                //cout<<j<<endl;

                y=trackSat[i][index].y;
                x=trackSat[i][index].x;
                z=trackSat[i][index].z;

            }
        v_3 coords={x,y,z};
        trackSatInterpolate[i].push_back(coords);
        file<<coords.x<<";"<<coords.y<<";"<<coords.z<<endl;
        }
    }

}

void get_visi_u_all(vector < vector <v_3> > &trackSatInterpolate, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <float> > > &u_or_aToSat){
    vector <vector <float> > temp;
    vector <float> temp2;
    float time_slot;

    for(int j=0; j < coordsOnEarth.size(); j++){
        u_or_aToSat.push_back(temp);
        
        for(int k=0; k< its_data.nbSatellites; k++){
            u_or_aToSat[j].push_back(temp2);

            for(int i=0; i < trackSatInterpolate[k].size(); i++){
                v_3 b=coordsOnEarth[j];
                v_3 a=trackSatInterpolate[k][i];

                if(ele_a_20(a,b)){
                    u_or_aToSat[j][k].push_back(1);
                }else{
                    u_or_aToSat[j][k].push_back(0);
                }
            }
        }
    }
    cout <<"-------------------------get visi fin--------------------------------"<<endl;

}

void get_visi_a(vector < vector <v_3> > &trackSat, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <int> > > &u_or_aToSat){
    vector <vector <int> > temp;
    vector <int> temp2;
    float time_slot;

    for(int j=0; j < coordsOnEarth.size(); j++){
        for(int t=0; t< its_data.nbAntennas; t++){
            u_or_aToSat.push_back(temp);
            for(int k=0; k< its_data.nbSatellites; k++){
                u_or_aToSat[j*4+t].push_back(temp2);
                for(int i=0; i < trackSat[k].size(); i++){
                    v_3 b=coordsOnEarth[j];
                    v_3 a=trackSat[k][i];

                    if(ele_a_20(a,b)){
                        u_or_aToSat[j*4+t][k].push_back(1);
                    }else{
                        u_or_aToSat[j*4+t][k].push_back(0);
                            
                    }
                }
            }
        }        
    }
    cout <<"-------------------------get visi fin--------------------------------"<<endl;

}

void get_contacts (vector< vector < vector <float> > > &contacts_sites, defined_data its_data){
    vector<float> temp(1);
    vector< vector <float> > temp2(1);

    for(int i=0; i < its_data.horizon_c/600; i++){
        contacts_sites.push_back(temp2);
        for(int j=0; j < 1; j++){
            contacts_sites[i][j]=temp;
            for(int k =0; k < 1; k++){
                contacts_sites[i][j][k]=float(i*600);
            }
        }
    }
}

void parse_contacts_sites_i(vector< vector < vector <int> > > &antennaToSat, vector< vector < vector <float> > > &contacts, defined_data its_data)
{
    list<float> contacts_temp;
    const char* output_file = "in_out/contacts_sites_h.txt";

    string str;
    int c;
    int nbSat=-1;
    int nbContact;
    int sat;
    float s;
    float e=0;
    float d;
    int contact=0;
    cout << "------ start reading site ---------" << endl;
    for(int i=0; i< antennaToSat.size(); i++){
        for(int j=0; j< antennaToSat[i].size(); j++){
            for(int k=0; k< antennaToSat[i][j].size(); k++){
                if(k*its_data.time_slot_sp3 <= its_data.horizon_c){
                    if(antennaToSat[i][j][k]==1 && contact==0){
                        contact=1;
                        contacts_temp.push_back(float(k*its_data.time_slot_sp3));
                        contacts_temp.sort();
                    }
                    if(antennaToSat[i][j][k]==0 && contact==1){
                        contact=0;
                        contacts_temp.push_back(float(k*its_data.time_slot_sp3));
                        contacts_temp.sort();  
                    }
                }
            }
        }
    }

    cout << "------ end reading ---------" << endl;
    
    //on enleve les doublons avant d ecrire dans le fichier
    int taille = contacts_temp.size();
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
    // contcats_temp comprend tous les temps
    
    int a;
    int b;

    int i=0;
    int j=0;
    int k=0;

    float memo600=0;
    float memo60=0;

    int al1=0;
    int al2=0;
    while(contacts_temp.size()!=0){

        a=contacts_temp.front();

        contacts_temp.pop_front();

        al1=0;
        al2=0;
        if(k==0){
            vector < vector < float > > struct_temp;
            contacts.push_back(struct_temp);
            vector < float > list_temp;
            contacts[0].push_back(list_temp);
            memo600=a;
            memo60=a;
            
        }
        if(a-memo600 >= 600){
            vector < vector < float > > struct_temp;
            contacts.push_back(struct_temp);
            i++;
            vector < float > list_temp;
            contacts[i].push_back(list_temp);
            j=0;
            contacts[i][j].push_back(a);
            memo600=a;
            al1=1;
        } 
        if(!al1 && a-memo60 >=60){
            vector < float > list_temp;
            contacts[i].push_back(list_temp);
            j++;
            contacts[i][j].push_back(a);
            memo60=a;
            al2=1;

        }
        if(!al1 && !al2){
            contacts[i][j].push_back(a);
            k++;
        }
    }

//ecriture dans le fichier in_out/contacts
    ofstream output(output_file);

    taille=contacts.size();
    for(int it=0; it < taille ; it++){
        int taille2=contacts[it].size();

        for(int it2=0; it2 < taille2 ; it2++){
            int taille3=contacts[it][it2].size();
            
            for(int it3=0; it3 < taille3 ; it3++){
                float temp= contacts[it][it2][it3];
                output << temp << endl;
            }
        }
    }

    cout << "------ end parsing ---------" << endl;
}

void parse_contacts_sites_lh(vector< vector < vector <int> > > &antennaToSat, vector< vector < vector <float> > > &contacts, defined_data its_data)
{
    list<float> contacts_temp;
    const char* output_file = "in_out/contacts_sites_lh.txt";

    string str;
    int c;
    int nbSat=-1;
    int nbContact;
    int sat;
    float s;
    float e=0;
    float d;
    int contact=0;
    cout << "------ start reading site ---------" << endl;
    for(int i=0; i< antennaToSat.size(); i++){
        for(int j=0; j< antennaToSat[i].size(); j++){
            for(int k=0; k< antennaToSat[i][j].size(); k++){
                if(k*its_data.time_slot_sp3 <= its_data.horizon_c){
                    if(antennaToSat[i][j][k]==1 && contact==0){
                        contact=1;
                        contacts_temp.push_back(float(k*its_data.time_slot_sp3));
                        contacts_temp.sort();
                    }
                    if(antennaToSat[i][j][k]==0 && contact==1){
                        contact=0;
                        contacts_temp.push_back(float(k*its_data.time_slot_sp3));
                        contacts_temp.sort();  
                    }
                }
            }
        }
    }

    cout << "------ end reading ---------" << endl;
    
    //on enleve les doublons avant d ecrire dans le fichier
    int taille = contacts_temp.size();
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
    // contcats_temp comprend tous les temps
    
    int a;
    int b;

    int i=0;
    int j=0;
    int k=0;

    float memo600=0;
    float memo60=0;

    int al1=0;
    int al2=0;
    while(contacts_temp.size()!=0){

        a=contacts_temp.front();

        contacts_temp.pop_front();

        al1=0;
        al2=0;
        if(k==0){
            vector < vector < float > > struct_temp;
            contacts.push_back(struct_temp);
            vector < float > list_temp;
            contacts[0].push_back(list_temp);
            memo600=a;
            memo60=a;
            
        }
        if(a-memo600 >= 600){
            vector < vector < float > > struct_temp;
            contacts.push_back(struct_temp);
            i++;
            vector < float > list_temp;
            contacts[i].push_back(list_temp);
            j=0;
            contacts[i][j].push_back(a);
            memo600=a;
            al1=1;
        } 
        if(!al1 && a-memo60 >=60){
            vector < float > list_temp;
            contacts[i].push_back(list_temp);
            j++;
            contacts[i][j].push_back(a);
            memo60=a;
            al2=1;

        }
        if(!al1 && !al2){
            contacts[i][j].push_back(a);
            k++;
        }
    }

//ecriture dans le fichier in_out/contacts
    ofstream output(output_file);

    taille=contacts.size();
    for(int it=0; it < taille ; it++){
        int taille2=contacts[it].size();

        for(int it2=0; it2 < taille2 ; it2++){
            int taille3=contacts[it][it2].size();
            
            for(int it3=0; it3 < taille3 ; it3++){
                float temp= contacts[it][it2][it3];
                output << temp << endl;
            }
        }
    }

    cout << "------ end parsing ---------" << endl;
}

void reconstruct_visi_a(vector< vector < vector <int> > > &antennaToSat, vector< vector < vector <float> > > &contacts, vector< vector < vector <int> > > &antennaToSatF, defined_data its_data){
    vector<vector<int> > temp1;
    vector<int> temp2;
    int index=0;

    for(int i=0; i< antennaToSat.size(); i++){
        antennaToSatF.push_back(temp1);

        for(int j=0; j< antennaToSat[i].size(); j++){
            antennaToSatF[i].push_back(temp2);
            index=0;

            for(int k=0; k< contacts.size(); k++){

                if(antennaToSat[i][j][index]==1 ){
                    antennaToSatF[i][j].push_back(1);
                }

                if(antennaToSat[i][j][index]==0 ){
                    antennaToSatF[i][j].push_back(0);
                }

                for(int l=0; l< contacts[k].size(); l++){

                    for(int m=0; m< contacts[k][l].size(); m++){  
                        index++;
                    }
                }
            }
        }
    }
    cout <<"end reconstruct visi a"<<endl;
}

void construct_visi_aP(vector< vector < vector <int> > > &antennaToSat, vector< vector < vector <float> > > &contacts, vector<vector <vector< vector < vector <int> > > > > &antennaToSatF, defined_data its_data){
    vector < vector <vector<vector<int> > > > temp1;
    vector < vector<vector<int> > > temp2;    
    vector<vector<int> > temp3;
    vector<int> temp4;
    int index=0;
    for(int i=0; i< antennaToSat.size(); i++){
        antennaToSatF.push_back(temp1);
        for(int j=0; j< antennaToSat[i].size(); j++){
            antennaToSatF[i].push_back(temp2);
            index=0;
            for(int k=0; k< contacts.size(); k++){
                antennaToSatF[i][j].push_back(temp3);
                for(int l=0; l< contacts[k].size(); l++){
                    antennaToSatF[i][j][k].push_back(temp4);
                    for(int m=0; m< contacts[k][l].size(); m++){
                        
                        if(antennaToSat[i][j][index]==1 ){
                            antennaToSatF[i][j][k][l].push_back(1);
                        }
                        if(antennaToSat[i][j][index]==0 ){
                            antennaToSatF[i][j][k][l].push_back(0);
                        }
                        index++;
                    }
                }
            }
        }
    }
}

void get_visi_a_thales(vector < vector <v_3> > &trackSat, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <int> > > &u_or_aToSat, vector< vector < vector<float> > > contacts_sites){
    vector <vector <int> > temp;
    vector <int> temp2;
    float time_slot;

    for(int j=0; j < coordsOnEarth.size(); j++){
        for(int t=0; t< its_data.nbAntennas; t++){
            u_or_aToSat.push_back(temp);
            for(int k=0; k< its_data.nbSatellites; k++){
                u_or_aToSat[j*4+t].push_back(temp2);
                for(int i=0; i < contacts_sites.size(); i++){
                    v_3 b=coordsOnEarth[j];
                    time_slot=round(contacts_sites[i][0][0]/its_data.time_slot_sp3);
                    v_3 a=trackSat[k][time_slot];

                    if(ele_a_20(a,b)){
                        u_or_aToSat[j*4+t][k].push_back(1);
                    }else{
                        u_or_aToSat[j*4+t][k].push_back(0);
                            
                    }
                }
            }
        }        
    }
    cout <<"-------------------------get visi fin--------------------------------"<<endl;

}

void get_visi_aP(vector <vector <v_3> > &trackSat, vector<v_3> &coordsOnEarth, defined_data its_data, vector< vector < vector <vector< vector<int> > > > > &u_or_aToSat, vector< vector < vector<float> > > contacts_sites){
    vector <vector <vector<vector <int> > > > temp0;
    vector <vector<vector <int> > >  temp1;
    vector <vector <int> > temp2;
    vector <int> temp3;
    float time_slot;
    int down=0;

    for(int j=0; j < coordsOnEarth.size(); j++){
        v_3 b=coordsOnEarth[j];
        for(int t=0; t< its_data.nbAntennas; t++){
            u_or_aToSat.push_back(temp0);
            for(int k=0; k< its_data.nbSatellites; k++){
                u_or_aToSat[j*4+t].push_back(temp1);
            
                for(int i=0; i < contacts_sites.size(); i++){
                    u_or_aToSat[j*4+t][k].push_back(temp2);

                    for(int l=0; l< contacts_sites[i].size(); l++){
                        u_or_aToSat[j][k][i].push_back(temp3);

                        for(int m=0; m < contacts_sites[i][l].size(); m++){
                            if(!down){
                                if(contacts_sites[i][l][0]== its_data.horizon_c){
                                    down=1;
                                }

                                    //cout <<"coucou"<<contacts_sites[i][l][m]<<endl;

                                time_slot=round(contacts_sites[i][l][m]/its_data.time_slot_sp3);
                                v_3 a=trackSat[k][time_slot];
                                if(ele_a_20(a,b)){
                                    u_or_aToSat[j*4+t][k][i][l].push_back(1);
                                }else{
                                    u_or_aToSat[j*4+t][k][i][l].push_back(0);    
                                }
                            }
                            
                        }
                        
                        
                    }   
                }
            }
        }
    }
    cout <<"-------------------------get visi fin--------------------------------"<<endl;

}