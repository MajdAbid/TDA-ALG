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

#include <tuple>
using namespace std;

vector<tuple<float,float,float>> barcode_filtration(vector<simplex>){
    return;
}

int main(int argc, char** argv) {

    if (argc != 2) {
        cout << "Syntax: " << argv[0] << " <filtration_file>" << endl;
        return 0;
    }
        string name = argv[1];
        vector<simplex> F = read_filtration(name);
        vector<tuple<float,float,float>> barcode = barcode_filtration(F);
        for (vector<tuple<float,float,float>>::iterator it = barcode.begin(); it != barcode.end(); it++) {
            cout << get<0>(*it) << " " << get<1>(*it) << " " << get<2>(*it);
            it->get<0>();
        }
        cout << "]}" << endl;
    }


}