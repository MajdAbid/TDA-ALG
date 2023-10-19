
/**
 * @file barcode_filtration.cpp
 * @brief This file contains the implementation of the barcode_filtration function, which computes the persistence barcodes of a given filtration.
 * 
 * A filtration is a sequence of simplicial complexes, where each complex is obtained by adding simplices to the previous one. 
 * The persistence barcodes of a filtration represent the birth and death times of the topological features (connected components, holes, voids, etc.) that appear and disappear as the filtration is built.
 * 
 * The barcode_filtration function takes a vector of simplices representing a filtration as input, and returns a vector of barcodes representing the persistence barcodes of the filtration.
 * 
 * The main function in this file reads a filtration from a file, computes its persistence barcodes using the barcode_filtration function, and prints the barcodes to the console.
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <limits>
#include "../include/read_filtration.hpp"

using namespace std;

#define DEBUG true

typedef struct barcode{
    int dim;
    float birth;
    float death;
}barcode;

vector<barcode> barcode_filtration(vector<simplex> F){

    // Declaring variables
    vector<barcode> persistence_barcodes;
    int N = F.size(); //Number if simplices

    // Sorting the filtration according to val
    sort(F.begin(), F.end(), [](simplex a, simplex b) {return (a.val < b.val || a.dim < b.dim);});

    // Getting Max dimension
    int max_dim = -1;
    for (const auto& simplex : F) {
        max_dim = max(max_dim, simplex.dim);
    }

    // 
    vector<int> cycle(max_dim,-1); // cycle[i] = number of i-cycles
    // Debugging the sorted filtration
    if (DEBUG) {
        cout << "Sorted Filtration:" << endl;
        for(int i = 0; i < F.size(); i++) {
            cout << "Simplex " << i << ": dim=" << F[i].dim << ", val=" << F[i].val << ", vert=[ ";
            for (auto vit = F[i].vert.begin(); vit != F[i].vert.end(); vit++) {
                cout << *vit << " ";

            }
            cout << "]" << endl;
        }
    }

    //Computing boundary matrix
    vector<vector<bool>> boundary_matrix(N, vector<bool>(N, false)); // N x N matrix of bools initialized to false
    for(int j = 0; j != N; j++){
        for(int l = 0; (l != N || F[l].val > F[j].val ); l++){
            if( (F[l].dim == F[j].dim - 1) && includes(F[j].vert.begin(),F[j].vert.end(), F[l].vert.begin(), F[l].vert.end())){
                boundary_matrix[l][j] = true;
            }
        }
    }
    //Computing low vector of boundary matrix
    vector<int> low(N, -1); 

    for(int j = 0; j != N; j++){
        for(int i = 0; i != N; i++){
            if(boundary_matrix[i][j] == true)low[j] = i;
        }
        cout<< "low = " << low[j] << ", j = " << j << endl;

    }

    //Reducing boundary matrix
    for(int j = 0; j != N; j++){
        bool is_reducable = (low[j] != -1);
        while (is_reducable){
            is_reducable = false;
            for(int l = 0; l != j; l++){
                if(F[l].dim != F[j].dim) continue;
                if(low[j] == low[l]){
                    boundary_matrix[low[j]][j] = false;
                    for(int i = 0; i != low[l]; i++){
                        boundary_matrix[i][j] = boundary_matrix[i][j] ^ boundary_matrix[i][l]; //XOR since we work in Z/2Z
                        if(boundary_matrix[i][j]) low[j] = i; //Updating low[j]
                    }
                    is_reducable = (low[j]!=low[l]);
                    if(!is_reducable){
                        // low[j] staying equal to low[l] means the j-th column is zero, thus a cycle
                        //cout << "Cycle at column " << j << endl;
                        low[j] = -1;
                        break;
                    }
                }
            }
        }
        if (low[j] == -1){
            if (cycle[F[j].dim] == -1) {
                cycle[F[j].dim] = j;
                barcode b;
                b.birth = F[j].val;
                b.dim = F[j].dim;
                b.death = numeric_limits<float>::infinity(); 
                persistence_barcodes.push_back(b);
            }
        }else{
            barcode b;
            b.dim = F[j].dim-1;
            b.birth = F[low[j]].val;
            b.death = F[j].val;
            persistence_barcodes.push_back(b);
        }    
    }

    return persistence_barcodes;
}
#ifdef BARCODE_FILTRATION_MAIN
int main(int argc, char** argv) {

    if (argc != 1) {
        cout << "Syntax: " << argv[0] << " <filtration_file>" << endl;
    }

    //string name = argv[1];
    string name = "/home/majd/Studies/INF559/TD4/resources/my_data_file.txt"; 
    vector<simplex> F = read_filtration(name);
    vector<barcode> persistence_barcodes = barcode_filtration(F);
    for (vector<barcode>::iterator it = persistence_barcodes.begin(); it != persistence_barcodes.end(); it++) {
        cout << it->dim << " " << it->birth << " " << it->death << endl;
    }
    return 0;    
}
#endif

