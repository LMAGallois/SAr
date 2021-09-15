/*
Puisque nous détenons seulement les coordonnées des satellites à intervalle de 5 min, 
interpolate sert à déterminer les coordonnées des satellites à intervalle plus réduit, 
en se basant sur les positions déjà connues
*/

#ifndef lagrange_h
#define lagrange_h

#include<bits/stdc++.h>
#include <vector>

using namespace std;

// To represent a data point corresponding to x and y = f(x)
struct Data
{
    int x;
    float y;
};

// function to interpolate the given data points using Lagrange's formula
// xi corresponds to the new data point whose value is to be obtained
// n represents the number of known data points
float interpolate(vector <Data> f, int xi, int n);

#endif