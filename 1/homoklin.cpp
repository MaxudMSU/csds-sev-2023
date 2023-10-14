#include <math.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <tuple>
#include <valarray>
#include <time.h>
#include <chrono>
#include "../diffur.h" 

using namespace std;

vector<Point> gen(Point a, Point b, double acc){
    Point p1 = a.homoMap(1.35);
    Point p2 = b.homoMap(1.35);

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

vector<Point> revGen(Point a, Point b, double acc){
    Point p1 = a.homoMapReverse(1.35);
    Point p2 = b.homoMapReverse(1.35);

    if (distance(p1,p2) > acc){
        Point m = mid(a, b);
        vector<Point> g1 = revGen(a, m, acc);
        vector<Point> g2 = revGen(m, b, acc);
        g1.insert(g1.end(), g2.begin()+1, g2.end());
        return g1;
    }
    else{
        return vector<Point> {p1,p2};
    }
}

pair<Point,int> coef(Point p0, Point p1, Point p2, Point p3){
    if (p0.x == 0){
        return make_pair(Point(0,0),0);
    }
    double temp1 = p1.x - p0.x;
    double temp2 = p1.y - p0.y;
    double a = (p2.x - p0.x) / temp1; double b = (p3.x - p2.x) / temp1;
    double c = (p2.y - p0.y) / temp2; double d = (p3.y - p2.y) / temp2;

    double t2 = (c - a) / (b - d);
    double t1 = a + b * t2;

    if ((t1 >= 0) && (t1 <= 1) && (t2 >= 0) && (t2 <= 1)){
        return make_pair(Point(p0.x + temp1*t1, p0.y + temp2*t1),1);
    }
    else{
        return make_pair(Point(0,0),0);
    }
}

tuple<vector<Point>,vector<Point>, Point, double> intersection(vector<Point> v1, vector<Point> v2){
    vector<Point> newV1, newV2;
    for (int i = 0; i < v1.size(); i++){
        for (int j = 0; j < v2.size(); j++){
            pair<Point,int> saver = coef(v1[i],v1[i+1],v2[j],v2[j+1]);
            if ((saver.second) && ((i + 1) != v1.size())){
                double ang = angle(saver.first, v1[i+1], v2[i+1]);
                for (int k = 0; k < i+1; k++){
                    newV1.push_back(v1[k]);
                }
                newV1.push_back(saver.first);
                for (int m = 0; m < j+1; m++){
                    newV2.push_back(v2[m]);
                }
                newV2.push_back(saver.first);
                return make_tuple(newV1, newV2, saver.first, ang);
            }
        }
    }
    return make_tuple(v1,v2, Point(0,0), 0);
}

vector<Point> intersection2(vector<Point> v1, vector<Point> v2){
    vector<Point> intersections;
    for (int i = 0; i < v1.size(); i++){
        for (int j = 0; j < v2.size(); j++){
            pair<Point,int> saver = coef(v1[i],v1[i+1],v2[j],v2[j+1]);
            if ((saver.second) && ((i + 1) != v1.size())){
                intersections.push_back(saver.first); 
            }
        }
    }
    return intersections;
}

tuple<vector<Point>,vector<Point>,Point, double> intersection1(vector<Point> v1, vector<Point> v2, Point homo){
    vector<Point> newV1, newV2;
    for (int i = 0; i < v1.size(); i++){
        for (int j = 0; j < v2.size(); j++){
            pair<Point,int> saver = coef(v1[i],v1[i+1],v2[j],v2[j+1]);
            if (homo.isClose(saver.first, 0.1)){
                double ang = angle(saver.first, v1[i+1], v2[i+1]);
                for (int k = 0; k < i+1; k++){
                    newV1.push_back(v1[k]);
                }
                newV1.push_back(saver.first);
                for (int m = 0; m < j+1; m++){
                    newV2.push_back(v2[m]);
                }
                newV2.push_back(saver.first);
                return make_tuple(newV1, newV2, saver.first, ang);
            }
            
        }
    }
    return make_tuple(v1,v2, homo, 0);
}

int main(int argc, char** argv){
    double start_x = atof(argv[1]);
    double start_y = atof(argv[2]);
    double v1_end_x = atof(argv[3]);
    double v1_end_y = atof(argv[4]);
    double v2_end_x = atof(argv[5]);
    double v2_end_y = atof(argv[6]);
    double parameter = atof(argv[7]);
    double accuracy = atof(argv[8]);
    double itercount = atof(argv[9]);
    // cout << itercount << endl;
    
    init_Window(); //костыль, без которого работает неправильно(до сих пор не понимаю почему)

    Point homoklin;
    vector<Point> newgv1;
    vector<Point> newgv2;
    double anglee;
    // cout << "----------------" << endl;
    auto begin = chrono::steady_clock::now();
    if (itercount == 1){
        
        vector<Point> V1{Point(start_x, start_y), Point(v1_end_x, v1_end_y)}, V2{Point(start_x,start_y),Point(v2_end_x, v2_end_y)};

        vector<Point> gv1 = gen(V1[0], V1[1], accuracy);

        vector<Point> gv2 = revGen(V2[0], V2[1], accuracy);

        tuple<vector<Point>,vector<Point>, Point, double> u = intersection(gv1, gv2);

        // кривые ДО первой точки пересечения, с ними нужно делать интерсект в цикле
        newgv1 = get<0>(u);
        newgv2 = get<1>(u);
        homoklin = get<2>(u);
        anglee = get<3>(u);
        
    }
    // time_t start = time(NULL);
    
    else{
        newgv1 = pointsFromFile("fil1");
        newgv2 = pointsFromFile("fil2");
        homoklin = newgv1.back();
        vector<Point> newnewgv1 = gen(newgv1[0], newgv1[1], accuracy);
        vector<Point> newnewgv2 = revGen(newgv2[0], newgv2[1], accuracy);
        for(int ii=2; ii < newgv1.size(); ii++){
            vector<Point> temp1 = gen(newgv1[ii], newgv1[ii+1], accuracy);
            newnewgv1.insert(newnewgv1.end(), temp1.begin()+1, temp1.end());
        }
        for(int jj=2; jj < newgv2.size(); jj++){
            vector<Point> temp2 = revGen(newgv2[jj], newgv2[jj+1], accuracy);
            newnewgv2.insert(newnewgv2.end(), temp2.begin()+1, temp2.end());
        }
        // homoklin.print();
        tuple<vector<Point>,vector<Point>,Point, double> uu = intersection1(newnewgv1, newnewgv2, homoklin);
        newgv1 = get<0>(uu);
        // newgv1 = newnewgv1;
        newgv2 = get<1>(uu);
        homoklin = get<2>(uu);
        anglee = get<3>(uu);
        // newgv2 = newnewgv2;
    }
    auto end = chrono::steady_clock::now();
    auto elapsed_s = chrono::duration_cast<chrono::seconds>(end - begin);
    double N1 = newgv1.size(), N2 = newgv2.size();
    double entropy1 = log(N1)/itercount, entropy2 = log(N2)/itercount;
    ofstream fileeeeee, fil, res;
    fileeeeee.open("fil1", ios::trunc);
    fil.open("fil2", ios::trunc);
    res.open("res", ios::trunc);
    res << fmax(entropy1, entropy2) << " " << anglee;
    
    // запись в файл
    for(int iii = 0; iii < newgv1.size(); iii++){
        fileeeeee << newgv1[iii].x << " " << newgv1[iii].y << " ";
    }
    for(int j = 0; j < newgv2.size(); j++){
        fil << newgv2[j].x << " " << newgv2[j].y << " ";
    }
    fileeeeee.close();
    fil.close();
}

