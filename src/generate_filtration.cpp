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

#include <map>
#include <set>
#include <string>

using namespace std;



/*
Mobius Strip :  {0,4},{2,6},{3,7},{11,15},{10,14}
Projective Plane : {0,4},{2,6},{1,5},{3,7},{11,15},{10,14},{8,12},{9,13}
Torus : {0,2,4,6},{1,5},{3,7},{11,14},{8,13},{10,15},{9,12}
Klein Bottle : {0,2,4,6},{1,5},{3,7},{11,15},{8,13},{10,14},{9,12}

*/

vector<simplex> generate_filtration(string topo_space_name){
    
    map<string, int> name_to_index = {
    {"Moebius Band", 0},
    {"Projective Plane", 1},
    {"Torus", 2},
    {"Klein Bottle", 3}
    };

    // Gluins scheme for the square space
    map<int, set<set<int>>> glue_map = {
    {0, {{0,4},{2,6},{3,7},{11,15},{10,14}}},
    {1, {{0,4},{2,6},{1,5},{3,7},{11,15},{10,14},{8,12},{9,13}}},
    {2, {{0,2,4,6},{1,5},{3,7},{11,14},{8,13},{10,15},{9,12}}},
    {3, {{0,2,4,6},{1,5},{3,7},{11,15},{8,13},{10,14},{9,12}}}
    };

    // Read the filtration of the square from the file
    string name = "/home/majd/Studies/INF559/TD4/resources/square_filtration.txt"; 
    vector<simplex> F = read_filtration(name);
    set<set<int>> glue = glue_map[name_to_index[topo_space_name]];
    cout<<"hi"<<endl;

    cout << "Glue pairs:" << endl;
    for (auto glue_pair : glue) {
        for (auto vertex : glue_pair) {
            cout << vertex << " ";
        }
        cout << endl;
    }
    for (auto glue_pair : glue) {
        int min = *glue_pair.begin();
        for (auto vertex : glue_pair) {
            if (vertex < min) {
                min = vertex;
            }
        }
        for (auto vertex : glue_pair) {
            if (vertex != min) {
                for (auto& simplex : F) {
                    if (simplex.vert.find(vertex) != simplex.vert.end()) {
                        simplex.vert.erase(vertex);
                        simplex.vert.insert(min);
                    }
                }
            }
        }
    }
    for (auto simplex : F) {
        cout << simplex.val << " ";
        cout << simplex.dim << " ";

        for (auto vertex : simplex.vert) {
            cout << vertex << " ";
        }
        cout << ": " << simplex.val << endl;
    }
    return F;
}

#ifdef GENERATE_FILTRATION_MAIN

int main(int argc, char** argv) {
    string topo_space_name;
    cout << "Enter the name for glue_map: ";
    cin >> topo_space_name;
    //cout << topo_space_name << endl;
    vector<simplex> F = generate_filtration(topo_space_name);
    
    return 0;
}
#endif
