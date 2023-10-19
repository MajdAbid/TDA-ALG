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
    sort(F.begin(), F.end(), [](simplex a, simplex b) {return a.val < b.val;});

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
    vector<int> low(N, __INT_MAX__); 
    for(int j = 0; j != N; j++){
        for(int i = 0; i != N; i++){
            if(boundary_matrix[i][j] == true)low[j] = i;
        }
        cout<< "low = " << low[j] << ", j = " << j << endl;

    }

    //Reducing boundary matrix
    for(int j = 0; j != N; j++){
        if (low[j] == __INT_MAX__) continue;
        bool is_reducable = true;
        while(is_reducable){
            is_reducable = false;
            for(int l = 0; l != j; l++){
                if(low[j] == low[l]){
                    boundary_matrix[low[j]][j] = false;
                    for(int i = 0; i != low[l]; i++){
                        boundary_matrix[i][j] = boundary_matrix[i][j] ^ boundary_matrix[i][l]; //XOR since we work in Z/2Z
                        if(boundary_matrix[i][j]) low[j] = i; //Updating low[j]
                    }
                    is_reducable = (low[j]!=low[l]);
                    if(!is_reducable){
                        // low[j] stays equal to low[l] means the j-th column is zero, thus a cycle
                        //cout << "Cycle at column " << j << endl;
                        low[j] = __INT_MAX__;
                        break;
                    }
                }
            }
        }
        barcode b;
        b.dim = F[j].dim-1;
        if (low[j] == __INT_MAX__){
            b.birth = F[j].val;
            b.death = std::numeric_limits<float>::infinity();
        }
        else{
            b.birth = F[low[j]].val;
            b.death = F[j].val;
        }
        persistence_barcodes.push_back(b);    
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

