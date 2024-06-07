#include <math.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <tuple>
#include <valarray>
#include <time.h>
#include <chrono>
#include "../types.h"

using namespace std;

vector<Point> gen(Point a, Point b, double acc){
    Point p1 = a.henon1(1.4, 0.3);
    Point p2 = b.henon1(1.4, 0.3);

    if (distance(p1,p2) > acc){
        Point m = mid(a, b);
        vector<Point> g1 = gen(a, m, acc);
        vector<Point> g2 = gen(m, b, acc);
        g1.insert(g1.end(), g2.begin()+1, g2.end());
        return g1;
    }
    else{
        return vector<Point> {p1,p2};
    }
}

int main(int argc, char** argv){
    double start_x = atof(argv[1]);
    double start_y = atof(argv[2]);
    double end_x = atof(argv[3]);
    double end_y = atof(argv[4]);
    double par_a = atof(argv[5]);
    double par_b = atof(argv[6]);
    double acc = atof(argv[7]);
    double itercount = atof(argv[8]);

    vector<Point> gv1;

    if (itercount == 1){
        vector<Point> V1{Point(start_x, start_y), Point(end_x, end_y)};
        gv1 = gen(V1[0], V1[1], acc);
    }
    
    else{
        gv1 = pointsFromFile("file.osip");
        vector<Point> newgv1 = gen(gv1[0], gv1[1], acc);
        for(int ii=2; ii < gv1.size(); ii++){
            vector<Point> temp1 = gen(gv1[ii], gv1[ii+1], acc);
            newgv1.insert(newgv1.end(), temp1.begin()+1, temp1.end());
        }
        gv1 = newgv1;
    }

    double N1 = gv1.size();
    double entropy = log(N1)/itercount;
    ofstream file, res;
    file.open("file.osip", ios::trunc);
    res.open("res.osip", ios::trunc);
    res << entropy;
    
    // запись в файл
    for(int iii = 0; iii < gv1.size(); iii++){
        file << gv1[iii].x << " " << gv1[iii].y << " ";
    }

    return 0;
}
