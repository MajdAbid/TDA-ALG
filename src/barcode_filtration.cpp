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
#include "generate_filtration.cpp"
using namespace std;

typedef struct barcode{
    int dim;
    float birth;
    float death;
}barcode;
const bool DEBUG = false;

// Defining a sparse matrix
struct SparseMatrix {
    vector<unordered_map<int, bool>> data;
    void set(int i, int j, bool val) {
        data[i][j] = val;
    }
    bool get(int i, int j) const {
        if (data[i].size() == 0) {
            return false;
        }
        auto jt = data[i].find(j);
        if (jt == data[i].end()) {
            return false;
        }
        return jt->second;
    }
};
// Coputing the sparce matrix of the boundary operator
SparseMatrix compute_boundary_matrix(int N, vector<simplex> &F, vector<int> &indices){
    SparseMatrix boundary_matrix;
    boundary_matrix.data = vector<unordered_map<int, bool>>(N, unordered_map<int, bool>());
    for(int j = 0; j != indices[F.back().dim]; j++){
        for(int l = indices[F[j].dim+1]; l != indices[F[j].dim+2]; l++){
            // if an element is contained in another, we add it to the boundary matrix
            bool is_contained = true;
            for(auto vertex : F[j].vert ){
                if(F[l].vert.find(vertex) == F[l].vert.end()) {
                    is_contained = false;
                    break;
                }
                //if(includes(F[j].vert.begin(),F[j].vert.end(), F[l].vert.begin(), F[l].vert.end())) continue;
            }
            if (is_contained) {
                boundary_matrix.set(j,l, true);
            }        }
    }
    return boundary_matrix;
}

// Separating simplices by dimension
vector<vector<simplex>> separate_by_dimension(const vector<simplex>& F,int max_dim = -1) {
    if(max_dim == -1){
    for (const auto& simplex : F) {
        max_dim = max(max_dim, simplex.dim);
    }
    }
    vector<vector<simplex>> F_by_dim(max_dim + 1);
    for (const auto& simplex : F) {
        F_by_dim[simplex.dim].push_back(simplex);
    }
    return F_by_dim;
}


vector<int> get_dimension_indices(const vector<simplex>& F) {
    vector<int> indices(F.back().dim + 2, -1);
    for (int i = 0; i < F.size(); i++) {
        if (indices[F[i].dim] == -1) {
            indices[F[i].dim] = i;
        }
    }
    indices[F.back().dim + 1] = F.size();
    return indices;
}

// Computing the persistence barcodes
vector<barcode> barcode_filtration(vector<simplex> F){

    // Declaring variables
    vector<barcode> persistence_barcodes;
    int N = F.size(); //Number if simplices

    // Sorting the filtration according to val
    sort(F.begin(), F.end(), [](simplex a, simplex b) {return (a.dim < b.dim || (a.dim == b.dim & a.val < b.val));});

    // Getting Max dimension
    int max_dim = -1;
    for (const auto& simplex : F) {
        max_dim = max(max_dim, simplex.dim);
    }
    // Separating simplices by dimension
    //vector<vector<simplex>> F_by_dim = separate_by_dimension(F, max_dim);
    // Initializing cycle vector


    // Printing the sorted filtration
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
    // Getting indices of first simplex of each dimension
    std::vector<int> indices = get_dimension_indices(F);
    // Computing boundary matrix
    SparseMatrix boundary_matrix = compute_boundary_matrix(N, F,indices);




    //Computing low vector of boundary matrix
    vector<int> low(N, -1); 
    for(int i = indices[0]; i != indices[max_dim]; i++){
        for(int j = indices[F[i].dim+1]; j != indices[F[i].dim+2]; j++){
            if(boundary_matrix.get(i, j) == true) low[j] = i;
        }
    }


    //Reducing boundary matrix
    vector<int> cycle(max_dim+1,-1); // cycle[i] = index of first detected cycle of a given dimension

    for(int j = 0; j != N; j++){
        bool is_reducable = (low[j] != -1);
        while (is_reducable){
            is_reducable = false;
            for(int l = indices[F[j].dim]; l != j; l++){
                if(F[l].dim != F[j].dim) continue;
                if(low[j] == low[l]){
                    boundary_matrix.get(low[j],j);
                    for(int i = indices[F[j].dim-1]; i != low[l]; i++){
                        boundary_matrix.set(i, j, boundary_matrix.get(i, l)^boundary_matrix.get(i, j));  //XOR since we work in Z/2Z
                        if(boundary_matrix.get(i, j)) low[j] = i; //Updating low[j]
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
        // If a cycle is detected, we add it to the persistence barcodes
        if (low[j] == -1){
            // if it is the first cycle of a given dimension, we add it to the cycle vector and mark it as j and a barcode
            // else drop it
            if (cycle[F[j].dim] == -1) {
                cycle[F[j].dim] = j;
                barcode b;
                b.birth = F[j].val;
                b.dim = F[j].dim;
                b.death = numeric_limits<float>::infinity(); 
                persistence_barcodes.push_back(b);
            }
        }else{
            // if we detected a new unique form (low(j),j), we mark a death of a cycle starting from low(j) 
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

    cout << "Please enter the filename of the filtration file from the ./include direction: ";
    string filename;
    cin >> filename;

    //string name = argv[1];
    string name = "/home/majd/Studies/INF559/TD4/resources/";
    name.append(filename); 
    vector<simplex> F = read_filtration(name);
    //vector<simplex> F = generate_filtration("Moebius Band");

    vector<barcode> persistence_barcodes = barcode_filtration(F);
    for (vector<barcode>::iterator it = persistence_barcodes.begin(); it != persistence_barcodes.end(); it++) {
        cout << it->dim << " " << it->birth << " " << it->death << endl;
    }
    return 0;    
}
#endif

