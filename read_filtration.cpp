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
#include "Eigen/SparseCore"

typedef Eigen::SparseMatrix<bool> SpMat;


using namespace std;

typedef set<int> vertices;


struct simplex {
    int dim;
    float val;
    vertices vert;
};

bool comp(simplex s1, simplex s2) {
    return (s1.val < s2.val);
}


vector<simplex> read_filtration(string name) {

    vector<simplex> F;
    F.clear();
    char input_file[100];
    sprintf(input_file, "%s", (char *) name.c_str());
    ifstream input(input_file);

    if (input) {
        string line;
        while (getline(input, line)) {
            simplex s;
            s.vert.clear();
            int i = 0, start = 0, end;
            do {
                end = line.find_first_of(' ', start);
                string temp = line.substr(start, end);

                // first value is the function value
                if (isdigit(temp[0]) and i == 0)
                    s.val = atof(temp.c_str());

                // second value is the dimension
                if (isdigit(temp[0]) and i == 1)
                    s.dim = atoi(temp.c_str());

                // the rest is the vertices
                if (isdigit(temp[0]) and i > 1)
                    s.vert.insert(atoi(temp.c_str()));

                i += 1;
                start = end + 1;

            } while (start);

            F.push_back(s);
        }
    }
    else { cout << "Failed to read file " << name << endl; }
    return F;

};

bool included(vertices v1, vertices v2) {

    for (set<int>::iterator i = v1.begin(); i != v1.end(); ++i) {
        bool iIsInV2 = false;
        for (set<int>::iterator j = v2.begin(); j != v2.end(); ++j) {
            if (*i == *j) {
                iIsInV2 = true;
                continue;
            }
        }
        if (!iIsInV2) {
            return false;
        }
    }

    return true;
}

SpMat getMatrixFromVector(vector<simplex> &F) {

    sort(F.begin(), F.end(), comp);
    int numSimplex = F.size();
    SpMat B(numSimplex, numSimplex);

    //Boucle pour parcourir les simplex
    for (int i = 0; i < numSimplex; i++) {

        if (F[i].dim == 0) { continue; }

        for (int j = 0; j < numSimplex; j++) {

            if (F[j].dim != F[i].dim - 1) { continue; }

            if (included(F[j].vert, F[i].vert)) {
                B.insert(j, i) = true;
            }
        }
    }

    return B;

}

int main(int argc, char **argv) {

    if (argc != 2) {
        cout << "Syntax: " << argv[0] << " <filtration_file>" << endl;
        return 0;
    }


    string name = argv[1];

    cout << "Reading filtration..." << endl;
    vector<simplex> F = read_filtration(name);
    cout << "Done." << endl;
    SpMat B = getMatrixFromVector(F);
    //Affichage
    for (vector<simplex>::iterator sit = F.begin(); sit != F.end(); sit++) {
        cout << "{val=" << sit->val << "; dim="
        << sit->dim << "; [";
        for (vertices::iterator vit = sit->vert.begin();
             vit != sit->vert.end(); vit++) {
            cout << *vit;
            vertices::iterator vvit = vit;
            vvit++;
            if (vvit != sit->vert.end())
                cout << ", ";
        }
        cout << "]}" << endl;
    }


    cout << B << endl;
    cout << "Theo dit coucou c'est B" << endl;

    return 0;
}
